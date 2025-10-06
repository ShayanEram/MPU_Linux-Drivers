#ifndef ISPI_HPP
#define ISPI_HPP

#include <vector>
#include <cstdint>

class ISpi 
{
public:
    virtual ~ISpi() = default;

    // Full-duplex transfer
    virtual std::vector<uint8_t> transfer(const std::vector<uint8_t>& tx) = 0;
    
    // Write only
    virtual void writeBytes(const std::vector<uint8_t>& tx) = 0;
    
    // Read only
    virtual std::vector<uint8_t> readBytes(size_t length) = 0;
};

#endif // ISPI_HPP