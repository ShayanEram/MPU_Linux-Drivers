#ifndef IUART_HPP
#define IUART_HPP

#include <vector>
#include <cstdint>
#include <string>

class IUART 
{
public:
    virtual ~IUART() = default;

    // Write a sequence of bytes
    virtual void writeBytes(const std::vector<uint8_t>& data) = 0;

    // Read up to 'length' bytes
    virtual std::vector<uint8_t> readBytes(size_t length) = 0;
};

#endif // IUART_HPP