// INA3221Enhanced.h
#pragma once
#include <Wire.h>
#include <INA3221.h>

class INA3221Enhanced : public INA3221 {
private:
    //VALORES PARA REGISTROS
    uint16_t CF_ONLY_CH1 =  0x4127;
    uint16_t PV_ONLY_CH1 =  0x24;
    uint16_t CRIT_MAX_VAL =  0xFFFF;

    uint16_t CF_DEFAULT =  0x7127;
    uint16_t PV_DEFAULT =  0x0002;
    uint16_t CRIT_DEFAULT_VAL =  0x8000;


    


    // Límites en voltios (fácil de leer)
    float LV = 3.5; // LOW Voltage
    float HV = 3.7; // HIGH Voltage

    //Convert volts to Registers values
    uint16_t voltsToReg(float v);


    //Extend privates
    TwoWire *_i2c;
    ina3221_addr_t _i2c_addr;

    // Reads 16 bytes from a register.
    void _read(ina3221_reg_t reg, uint16_t *val);

    // Writes 16 bytes to a register.
    void _write(ina3221_reg_t reg, uint16_t *val);

public:
    INA3221Enhanced(ina3221_addr_t addr) : INA3221(addr) {};
    void enableUnderVoltageRegisters();
    void resetRegisters();
    void preventBrownout(float batteryVoltage);
};

