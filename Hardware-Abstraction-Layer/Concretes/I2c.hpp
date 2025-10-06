#ifndef I2C_HPP
#define I2C_HPP

#include "II2c.hpp"

#include <string>

class I2C : public II2C
{
public:

    I2C(int busNumber, int deviceAddress);
    ~I2C();

    void writeBytes(const std::vector<uint8_t>& data) override;
    void writeRegister(uint8_t reg, uint8_t value) override;
    std::vector<uint8_t> readBytes(size_t length) override;
    uint8_t readRegister(uint8_t reg) override;

private:

    int _fd;
    int _bus;
    int _addr;
};

#endif // I2C_HPP