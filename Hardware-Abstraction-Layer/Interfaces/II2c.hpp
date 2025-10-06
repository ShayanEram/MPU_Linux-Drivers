#ifndef II2C_HPP
#define II2C_HPP

#include <vector>
#include <cstdint>

class II2C 
{
public:
    virtual ~II2C() = default;

    // Write raw bytes to the device
    virtual void writeBytes(const std::vector<uint8_t>& data) = 0;
    
    // Write a single register
    virtual void writeRegister(uint8_t reg, uint8_t value) = 0;
    
    // Read raw bytes from the device
    virtual std::vector<uint8_t> readBytes(size_t length) = 0;
    
    // Read a single register
    virtual uint8_t readRegister(uint8_t reg) = 0;
};

#endif // II2C_HPP