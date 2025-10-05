#ifndef PWM_HPP
#define PWM_HPP

#include <string>
#include <fstream>
#include <stdexcept>

class PWM 
{
public:

    PWM(int chipIndex, int channelIndex);
    ~PWM();

    void setPeriodNs(int periodNs) ;
    void setDutyCycleNs(int dutyCycleNs);
    void enable(bool enable) ;
    
private:

    std::string _basePath;
    std::string _chipPath;
    int _chip;
    int _channel;

    void writeFile(const std::string& file, const std::string& value);
};

#endif // PWM_HPP