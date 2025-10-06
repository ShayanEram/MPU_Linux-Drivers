#ifndef HARDWARE_MANAGER_HPP
#define HARDWARE_MANAGER_HPP

#include "IGpio.hpp"
#include "IPwm.hpp"
#include "II2C.hpp"
#include "ISpi.hpp"
#include "IUart.hpp"

#include <map>
#include <memory>
#include <stdexcept>
#include "DeviceId.hpp"

class HardwareManager 
{
public:
    // --- Adders ---
    template<typename T, typename... Args>
    void addGpio(DeviceId id, Args&&... args) {
        gpios[id] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void addPwm(DeviceId id, Args&&... args) {
        pwms[id] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void addI2C(DeviceId id, Args&&... args) {
        i2cs[id] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void addSpi(DeviceId id, Args&&... args) {
        spis[id] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void addUart(DeviceId id, Args&&... args) {
        uarts[id] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    // --- Getters ---
    IGpio* getGpio(DeviceId id);
    IPwm* getPwm(DeviceId id);
    II2C* getI2C(DeviceId id);
    ISpi* getSpi(DeviceId id);
    IUART* getUart(DeviceId id);

private:
    std::map<DeviceId, std::unique_ptr<IGpio>> gpios;
    std::map<DeviceId, std::unique_ptr<IPwm>> pwms;
    std::map<DeviceId, std::unique_ptr<II2C>> i2cs;
    std::map<DeviceId, std::unique_ptr<ISpi>> spis;
    std::map<DeviceId, std::unique_ptr<IUART>> uarts;

    template<typename MapType>
    auto* getFromMap(MapType& m, DeviceId id) {
        auto it = m.find(id);
        if (it == m.end()) {
            throw std::runtime_error("No device registered for this DeviceId");
        }
        return it->second.get();
    }
};

#endif // HARDWARE_MANAGER_HPP