#include "Uart.hpp"

#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

UART::UART(const std::string& device, int baudrate = B115200) : _fd(-1), _dev(device)
{
    _fd = open(_dev.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (_fd < 0) {
        throw std::runtime_error("Failed to open UART device: " + _dev);
    }

    struct termios tty{};
    if (tcgetattr(_fd, &tty) != 0) {
        close(_fd);
        throw std::runtime_error("Failed to get UART attributes");
    }

    cfsetospeed(&tty, baudrate);
    cfsetispeed(&tty, baudrate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_iflag &= ~IGNBRK;                     // disable break processing
    tty.c_lflag = 0;                            // no signaling chars, no echo
    tty.c_oflag = 0;                            // no remapping, no delays
    tty.c_cc[VMIN]  = 0;                        // non-blocking read
    tty.c_cc[VTIME] = 5;                        // 0.5s read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);     // no SW flow control
    tty.c_cflag |= (CLOCAL | CREAD);            // enable receiver
    tty.c_cflag &= ~(PARENB | PARODD);          // no parity
    tty.c_cflag &= ~CSTOPB;                     // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;                    // no HW flow control

    if (tcsetattr(_fd, TCSANOW, &tty) != 0) {
        close(_fd);
        throw std::runtime_error("Failed to set UART attributes");
    }
}

UART::~UART() {
    if (_fd >= 0) close(_fd);
}

void UART::writeBytes(const std::vector<uint8_t>& data) {
    if (::write(_fd, data.data(), data.size()) != (ssize_t)data.size()) {
        throw std::runtime_error("UART write failed");
    }
}

std::vector<uint8_t> UART::readBytes(size_t length) {
    std::vector<uint8_t> buf(length);
    ssize_t n = ::read(_fd, buf.data(), length);
    if (n < 0) {
        throw std::runtime_error("UART read failed");
    }
    buf.resize(n);
    return buf;
}