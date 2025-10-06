#ifndef UART_HPP
#define UART_HPP

#include "IUART.hpp"

class UART : public IUART
{
public:
    UART(const std::string& device, int baudrate = B115200);
    ~UART();

    void writeBytes(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> readBytes(size_t length) override;

private:
    int _fd;
    std::string _dev;
};

#endif // UART_HPP