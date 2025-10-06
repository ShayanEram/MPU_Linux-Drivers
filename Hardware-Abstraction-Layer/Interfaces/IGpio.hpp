#ifndef IGPIO_HPP
#define IGPIO_HPP

#include <string>

class IGpio 
{
public:
    enum class Direction { IN, OUT };

    virtual ~IGpio() = default;

    virtual void setValue(bool value) = 0;
    virtual bool getValue() const = 0;
    virtual void toggleValue() = 0;
};

#endif // IGPIO_HPP