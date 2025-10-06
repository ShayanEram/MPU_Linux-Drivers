#ifndef SPI_HPP
#define SPI_HPP

#include "ISpi.hpp"

#include <string>

class Spi : public ISpi
{
public:
    
    Spi(const std::string& device, uint32_t speed = 500000, uint8_t mode = SPI_MODE_0, uint8_t bits = 8);
    ~Spi();

    std::vector<uint8_t> transfer(const std::vector<uint8_t>& tx) override;
    void writeBytes(const std::vector<uint8_t>& tx) override;
    std::vector<uint8_t> readBytes(size_t length) override;

private:

    int _fd;
    std::string _device;
};

#endif // SPI_HPP