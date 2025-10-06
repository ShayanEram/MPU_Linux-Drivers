#include "Pwm.hpp"

#include <fstream>
#include <stdexcept>

PWM::PWM(int chipIndex, int channelIndex) : _chip(chipIndex), _channel(channelIndex)
{
    _chipPath = "/sys/class/pwm/pwmchip" + std::to_string(_chip) + "/";
    _basePath = _chipPath + "pwm" + std::to_string(_channel) + "/";

    std::ofstream exportFile(_chipPath + "export");
    if (!exportFile.is_open()) 
    {
        throw std::runtime_error("Failed to open export file");
    }
    exportFile << _channel;
    exportFile.close();
}

PWM::~PWM() 
{
    std::ofstream unexport_file(_chipPath + "unexport");
    if (unexport_file.is_open()) {
        unexport_file << _channel;
        unexport_file.close();
    }
}

void PWM::setPeriodNs(int periodNs) 
{
    writeFile("period", std::to_string(periodNs));
}

void PWM::setDutyCycleNs(int dutyCycleNs) 
{
    writeFile("duty_cycle", std::to_string(dutyCycleNs));
}

void PWM::enable(bool enable) 
{
    writeFile("enable", enable ? "1" : "0");
}

void PWM::writeFile(const std::string& file, const std::string& value) 
{
    std::ofstream ofs(_basePath + file);
    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open " + _basePath + file);
    }
    ofs << value;
    ofs.close();
}
