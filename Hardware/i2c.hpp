#ifndef I2C_HPP
#define I2C_HPP

#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <vector>

class I2C 
{
public:

    I2C(int busNumber, int deviceAddress);
    ~I2C();

    void writeBytes(const std::vector<uint8_t>& data);
    void writeRegister(uint8_t reg, uint8_t value) ;
    std::vector<uint8_t> readBytes(size_t length);
    uint8_t readRegister(uint8_t reg);

private:

    int _fd;
    int _bus;
    int _addr;
};




















#endif // I2C_HPP