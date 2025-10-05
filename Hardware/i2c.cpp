#include "i2c.hpp"

I2C::I2C(int busNumber, int deviceAddress) : _fd(-1), _bus(busNumber), _addr(deviceAddress)
{
    std::string deviceName = "/dev/i2c-" + std::to_string(_bus);
    _fd = open(deviceName.c_str(), O_RDWR);
    if(_fd < 0)
    {
        throw std::runtime_error("Failed to open I2C bus " + deviceName);
    }

    if(ioctl(_fd, I2C_SLAVE, _addr) < 0)
    {
        close(_fd);
        throw std::runtime_error("Failed to set I2C address " + std::to_string(_addr));
    }
}

I2C::~I2C()
{
    if(_fd >= 0)
    {
        close(_fd);
    }
}

void I2C::writeBytes(const std::vector<uint8_t>& data)
{
    if (::write(_fd, data.data(), data.size()) != static_cast<ssize_t>(data.size())) 
    {
        throw std::runtime_error("I2C write failed");
    }
}

void I2C::writeRegister(uint8_t reg, uint8_t value) 
{
    uint8_t buf[2] = { reg, value };
    if (::write(_fd, buf, 2) != 2) {
        throw std::runtime_error("I2C register write failed");
    }
}

std::vector<uint8_t> I2C::readBytes(size_t length) 
{
    std::vector<uint8_t> buf(length);
    if (::read(_fd, buf.data(), length) != static_cast<ssize_t>(length))
    {
        throw std::runtime_error("I2C read failed");
    }
    return buf;
}

uint8_t I2C::readRegister(uint8_t reg) 
{
    if (::write(_fd, &reg, 1) != 1) 
    {
        throw std::runtime_error("I2C register select failed");
    }
    uint8_t val;
    if (::read(_fd, &val, 1) != 1) 
    {
        throw std::runtime_error("I2C register read failed");
    }
    return val;
}