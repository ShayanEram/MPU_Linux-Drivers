#ifndef IPWM_HPP
#define IPWM_HPP

#include <string>

class IPwm 
{
public:
    virtual ~IPwm() = default;

    virtual void setPeriodNs(int periodNs) = 0;
    virtual void setDutyCycleNs(int dutyCycleNs) = 0;
    virtual void enable(bool enable) = 0;
};

#endif // IPWM_HPP