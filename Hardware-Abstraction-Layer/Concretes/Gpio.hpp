#ifndef GPIO_HPP
#define GPIO_HPP

#include "IGpio.hpp"

class GPIO : public IGpio
{
public:
    
    GPIO(const std::string& chipPath, int lineOffset, Direction direction, int defaultValue = 0);
    ~GPIO();

    void setValue(bool value) override;
    bool getValue() const override;
    void toggleValue() override;

private:
    struct gpiod_chip* chip;
    struct gpiod_line* line;
    int offset;
    int ret;
}

#endif // GPIO_HPP