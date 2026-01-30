// INA3221Enhanced.h
#pragma once
#include <INA3221.h>

class INA3221Enhanced : public INA3221 {
public:
    INA3221Enhanced();  // Constructor
    void setSingleChannelPwrValidLowLimit(int channel, int16_t busLimitMv, int16_t shuntLimitMv);
};