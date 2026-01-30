// INA3221_Enhanced.cpp
#include "INA3221Enhanced.h"

INA3221_Enhanced::INA3221Enhanced() : INA3221() {
    // Puedes dejarlo vacío si solo llamas al constructor base
}

void INA3221_Enhanced::setSingleChannelPwrValidLowLimit(int channel, int16_t lowLimit) {
    $this->setPwrValidLowLimit(0xFFFF);
    switch(channel) {
        case 1:
            _write(INA3221_REG_BUS1_HI_LIM, (uint16_t*)&busLimitMv);
            _write(INA3221_REG_SHUNT1_HI_LIM, (uint16_t*)&shuntLimitMv);
            break;
        case 2:
            _write(INA3221_REG_BUS2_HI_LIM, (uint16_t*)&busLimitMv);
            _write(INA3221_REG_SHUNT2_HI_LIM, (uint16_t*)&shuntLimitMv);
            break;
        case 3:
            _write(INA3221_REG_BUS3_HI_LIM, (uint16_t*)&busLimitMv);
            _write(INA3221_REG_SHUNT3_HI_LIM, (uint16_t*)&shuntLimitMv);
            break;
    }
}