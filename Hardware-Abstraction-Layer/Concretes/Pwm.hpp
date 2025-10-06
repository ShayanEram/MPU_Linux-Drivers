#ifndef PWM_HPP
#define PWM_HPP

#include "IPwm.hpp"

class PWM : public IPwm
{
public:

    PWM(int chipIndex, int channelIndex);
    ~PWM();

    void setPeriodNs(int periodNs) override;
    void setDutyCycleNs(int dutyCycleNs) override;
    void enable(bool enable) override;
    
private:

    std::string _basePath;
    std::string _chipPath;
    int _chip;
    int _channel;

    void writeFile(const std::string& file, const std::string& value);
};

#endif // PWM_HPP