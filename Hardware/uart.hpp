#ifndef UART_HPP
#define UART_HPP

#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <vector>

class UART 
{
public:
    UART(const std::string& device, int baudrate = B115200);
    ~UART();

    void writeBytes(const std::vector<uint8_t>& data);
    std::vector<uint8_t> readBytes(size_t length);

private:
    int _fd;
    std::string _dev;
};


#endif // UART_HPP