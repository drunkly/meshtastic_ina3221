#include "configuration.h"

#if HAS_TELEMETRY && !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR && __has_include(<INA3221.h>)

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "INA3221Sensor.h"
#include "TelemetrySensor.h"
#include "INA3221Enhanced.h"

INA3221Sensor::INA3221Sensor() : TelemetrySensor(meshtastic_TelemetrySensorType_INA3221, "INA3221"){};

int32_t INA3221Sensor::runOnce()
{
    LOG_INFO("Init sensor: %s", sensorName);
    if (!hasSensor()) {
        return DEFAULT_SENSOR_MINIMUM_WAIT_TIME_BETWEEN_READS;
    }
    if (!status) {
        ina3221.begin(nodeTelemetrySensorsMap[sensorType].second);
        ina3221.setShuntRes(100, 100, 100); // 0.1 Ohm shunt resistors
        //TODO: setup INA3221 PV WARN CRI alerts: CH1=BAT, CH2=DC-DC, CH3=SOL
        //float LOW_VOLTAGE_THRESHOLD = 3.4;       // voltios
        //uint16_t LVT = (uint16_t)(LOW_VOLTAGE_THRESHOLD * 1000); // pasa a mV

        //float HIGH_CURRENT_TRIGGER = 300.0;               // mA
        //float shuntR = 0.1;                     // ohmios
        //float currentA = HIGH_CURRENT_TRIGGER / 1000.0;    // pasa a amperios
        //float vShunt = currentA * shuntR;       // V = I*R
        //uint16_t HCT = (uint16_t)(vShunt / 0.00004);  // cada bit = 40uV
        
        //ina3221.writeRegister(INA3221_PV_LIMIT_REGISTER_CH1, LVT);
        //ina3221.writeRegister(INA3221_PV_LIMIT_REGISTER_CH2, 0x7FFF); // valor por defecto que desactiva alerta
        //ina3221.writeRegister(INA3221_PV_LIMIT_REGISTER_CH3, 0x7FFF); 

        //ina3221.writeRegister(INA3221_CRI_REGISTER_CH1, 0x7FFF);
        //ina3221.writeRegister(INA3221_CRI_REGISTER_CH2, valueFor500mA);
        //ina3221.writeRegister(INA3221_CRI_REGISTER_CH3, 0x7FFF);

        // Habilitar solo CH1 para PV
        //ina3221.writeRegister(PV_ENABLE_REGISTER, 0b001); // bit2 = CH1

        // Habilitar solo CH2 para CRI
        //ina3221.writeRegister(CRI_ENABLE_REGISTER, 0b010); // bit0 = CH2
        status = true;
    } else {
        status = true;
    }
    return initI2CSensor();
};

void INA3221Sensor::setup() {}

struct _INA3221Measurement INA3221Sensor::getMeasurement(ina3221_ch_t ch)
{
    struct _INA3221Measurement measurement;

    measurement.voltage = ina3221.getVoltage(ch);
    measurement.current = ina3221.getCurrent(ch);

    return measurement;
}

struct _INA3221Measurements INA3221Sensor::getMeasurements()
{
    struct _INA3221Measurements measurements;

    // INA3221 has 3 channels starting from 0
    for (int i = 0; i < 3; i++) {
        measurements.measurements[i] = getMeasurement((ina3221_ch_t)i);
    }

    return measurements;
}

bool INA3221Sensor::getMetrics(meshtastic_Telemetry *measurement)
{
    switch (measurement->which_variant) {
    case meshtastic_Telemetry_environment_metrics_tag:
        return getEnvironmentMetrics(measurement);

    case meshtastic_Telemetry_power_metrics_tag:
        return getPowerMetrics(measurement);
    }

    // unsupported metric
    return false;
}

bool INA3221Sensor::getEnvironmentMetrics(meshtastic_Telemetry *measurement)
{
    struct _INA3221Measurement m = getMeasurement(ENV_CH);

    measurement->variant.environment_metrics.has_voltage = true;
    measurement->variant.environment_metrics.has_current = true;

    measurement->variant.environment_metrics.voltage = m.voltage;
    measurement->variant.environment_metrics.current = m.current;

    return true;
}

bool INA3221Sensor::getPowerMetrics(meshtastic_Telemetry *measurement)
{
    struct _INA3221Measurements m = getMeasurements();

    measurement->variant.power_metrics.has_ch1_voltage = true;
    measurement->variant.power_metrics.has_ch1_current = true;
    measurement->variant.power_metrics.has_ch2_voltage = true;
    measurement->variant.power_metrics.has_ch2_current = true;
    measurement->variant.power_metrics.has_ch3_voltage = true;
    measurement->variant.power_metrics.has_ch3_current = true;

    measurement->variant.power_metrics.ch1_voltage = m.measurements[INA3221_CH1].voltage;
    measurement->variant.power_metrics.ch1_current = m.measurements[INA3221_CH1].current;
    measurement->variant.power_metrics.ch2_voltage = m.measurements[INA3221_CH2].voltage;
    measurement->variant.power_metrics.ch2_current = m.measurements[INA3221_CH2].current;
    measurement->variant.power_metrics.ch3_voltage = m.measurements[INA3221_CH3].voltage;
    measurement->variant.power_metrics.ch3_current = m.measurements[INA3221_CH3].current;

    return true;
}

uint16_t INA3221Sensor::getBusVoltageMv()
{
    return lround(ina3221.getVoltage(BAT_CH) * 1000);
}

int16_t INA3221Sensor::getCurrentMa()
{
    return lround(ina3221.getCurrent(BAT_CH));
}

#endif