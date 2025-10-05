#include "gpio.hpp"

GPIO::GPIO(const std::string& chipPath, int lineOffset, Direction direction, int defaultValue):
    chip(nullptr), line(nullptr), offset(lineOffset)
{
    chip = gpiod_chip_open(chipPath.c_str());
    if (!chip) {
        throw std::runtime_error("Failed to open GPIO chip: " + chipPath);
    }

    line = gpiod_chip_get_line(chip, offset);
    if (!line) {
        gpiod_chip_close(chip);
        throw std::runtime_error("Failed to get GPIO line offset " + std::to_string(offset));
    }

    if (direction == Direction::OUT) {
        ret = gpiod_line_request_output(line, "gpio_control", defaultValue);
    } else {
        ret = gpiod_line_request_input(line, "gpio_control");
    }

    if (ret < 0) {
        gpiod_chip_close(chip);
        throw std::runtime_error("Failed to request GPIO line");
    }
}

GPIO::~GPIO() 
{
    if (line) {
        gpiod_line_release(line);
    }
    if (chip) {
        gpiod_chip_close(chip);
    }
}

void GPIO::setValue(bool value)
{
    if (gpiod_line_set_value(line, value ? 1 : 0) < 0) {
        throw std::runtime_error("Failed to set GPIO value");
    }
}

bool GPIO::getValue() const
{
    int value = gpiod_line_get_value(line);
    if (value < 0) {
        throw std::runtime_error("Failed to get GPIO value");
    }
    return value == 0;
}

void GPIO::toggleValue()
{
    bool currentValue = getValue();
    setValue(!currentValue);
}