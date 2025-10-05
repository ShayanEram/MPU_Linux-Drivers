#ifndef GPIO_HPP
#define GPIO_HPP

#include <gpiod.h>
#include <stdexcept>
#include <string>

class GPIO 
{
public:
    enum class Direction { IN, OUT };
    
    GPIO(const std::string& chipPath, int lineOffset, Direction direction, int defaultValue = 0);
    ~GPIO();

    void setValue(bool value);
    bool getValue() const;
    void toggleValue();

private:
    struct gpiod_chip* chip;
    struct gpiod_line* line;
    int offset;
    int ret;
}

#endif // GPIO_HPP