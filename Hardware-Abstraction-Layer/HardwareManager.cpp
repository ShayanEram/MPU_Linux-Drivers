#include "HardwareManager.hpp"

IGpio* HardwareManager::getGpio(DeviceId id) 
{
    return getFromMap(gpios, id);
}

IPwm* HardwareManager::getPwm(DeviceId id) 
{
    return getFromMap(pwms, id);
}

II2C* HardwareManager::getI2C(DeviceId id) 
{
    return getFromMap(i2cs, id);
}

ISpi* HardwareManager::getSpi(DeviceId id) 
{
    return getFromMap(spis, id);
}

IUART* HardwareManager::getUart(DeviceId id) 
{
    return getFromMap(uarts, id);
}
