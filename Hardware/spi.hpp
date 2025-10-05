#ifndef SPI_HPP
#define SPI_HPP

#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <vector>
#include <cstring>

class SPI 
{
public:
    
    SPI(const std::string& device, uint32_t speed = 500000, uint8_t mode = SPI_MODE_0, uint8_t bits = 8);
    ~SPI();

    std::vector<uint8_t> transfer(const std::vector<uint8_t>& tx);
    void writeBytes(const std::vector<uint8_t>& tx);
    std::vector<uint8_t> readBytes(size_t length);

private:

    int _fd;
    std::string _device;
};

#endif // SPI_HPP