// INA3221_Enhanced.cpp
#include "INA3221Enhanced.h"

void INA3221Enhanced::_read(ina3221_reg_t reg, uint16_t *val)
{
    _i2c->beginTransmission(_i2c_addr);
    _i2c->write(reg);  // Register
    _i2c->endTransmission(false);

    _i2c->requestFrom((uint8_t)_i2c_addr, (uint8_t)2);

    if (_i2c->available()) {
        *val = ((_i2c->read() << 8) | _i2c->read());
    }
}

void INA3221Enhanced::_write(ina3221_reg_t reg, uint16_t *val)
{
    _i2c->beginTransmission(_i2c_addr);
    _i2c->write(reg);                 // Register
    _i2c->write((*val >> 8) & 0xFF);  // Upper 8-bits
    _i2c->write(*val & 0xFF);         // Lower 8-bits
    _i2c->endTransmission();
}


// Convierte voltios a valor del registro PV (LSB = 8 mV, bits 15-3)
uint16_t INA3221Enhanced::voltsToReg(float v) {
    return static_cast<uint16_t>((v / 0.008 + 0.5) ) << 3;
}



void INA3221Enhanced::enableUnderVoltageRegisters() {
    uint16_t pvHi = voltsToReg(HV);
    uint16_t pvLo = voltsToReg(LV);
    //Configuration   0x00    0x4127  Habilita solo CH1 y modo continuo
    _write(INA3221_REG_CONF, &CF_ONLY_CH1);

    //Mask/Enable 0x0F    0x24    PVEN1=1, PVEN2/3=0 → PV solo depende de CH1
    _write(INA3221_REG_MASK_ENABLE, &PV_ONLY_CH1);

    //PV Upper Limit  0x10    0x0E78  3.7V → activa PV cuando Vbat > 3.7V
    _write(INA3221_REG_PWR_VALID_HI_LIM, &pvHi); // 3.7V

    //PV Lower Limit  0x11    0x0DB0  3.5V → desactiva PV cuando Vbat < 3.5V
    _write(INA3221_REG_PWR_VALID_LO_LIM, &pvLo); // 3.5V

    //Critical Shunt Limit CH1    0x07    0xFFFF -> desactiva CRIT
    _write(INA3221_REG_CH1_CRIT_ALERT_LIM, &CRIT_MAX_VAL);


}

void INA3221Enhanced::resetRegisters() {
    //Critical Shunt Limit CH1 0x8000  RESET CRIT => CRIT = 1
    _write(INA3221_REG_CH1_CRIT_ALERT_LIM, &CRIT_DEFAULT_VAL);
    //Configuration   0x00    0x7127  REST CONFIGURATION => mide en todos los canales
    _write(INA3221_REG_CONF, &CF_DEFAULT);
    //Mask/Enable 0x0F    0x0002  RESET PV MASK=> PV depende de CH1, CH2, CH3 
    _write(INA3221_REG_MASK_ENABLE, &PV_DEFAULT);
   
}

void INA3221Enhanced::preventBrownout(float batteryVoltage){
    if(batteryVoltage<LV){
        enableUnderVoltageRegisters();
    }
}