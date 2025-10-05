#include "spi.hpp"

SPI::SPI(const std::string& device, uint32_t speed = 500000, uint8_t mode = SPI_MODE_0, uint8_t bits = 8)
    : _fd(-1), _device(device)
{
    _fd = open(dev.c_str(), O_RDWR);
    if (_fd < 0) {
        throw std::runtime_error("Failed to open SPI device: " + _device);
    }

    if (ioctl(_fd, SPI_IOC_WR_MODE, &mode) < 0)
        throw std::runtime_error("Failed to set SPI mode");

    if (ioctl(_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
        throw std::runtime_error("Failed to set bits per word");

    if (ioctl(_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        throw std::runtime_error("Failed to set max speed");
}

SPI::~SPI() 
{
    if (_fd >= 0) close(_fd);
}

std::vector<uint8_t> SPI::transfer(const std::vector<uint8_t>& tx) 
{
    std::vector<uint8_t> rx(tx.size());
    struct spi_ioc_transfer tr{};
    std::memset(&tr, 0, sizeof(tr));
    tr.tx_buf = reinterpret_cast<unsigned long>(tx.data());
    tr.rx_buf = reinterpret_cast<unsigned long>(rx.data());
    tr.len = tx.size();

    if (ioctl(_fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        throw std::runtime_error("SPI transfer failed");
    }
    return rx;
}

void SPI::writeBytes(const std::vector<uint8_t>& tx) {
    if (::write(_fd, tx.data(), tx.size()) != (ssize_t)tx.size()) {
        throw std::runtime_error("SPI write failed");
    }
}

std::vector<uint8_t> SPI::readBytes(size_t length) {
    std::vector<uint8_t> tx(length, 0x00);
    return transfer(tx);
}