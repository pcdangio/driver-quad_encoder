#ifndef QUAD_ENCODER___RPI_DRIVER_H
#define QUAD_ENCODER___RPI_DRIVER_H

#include <quad_encoder/driver.hpp>

namespace quad_encoder {

class rpi_driver
    : public quad_encoder::driver
{
public:
    rpi_driver(uint32_t cpr, uint8_t pin_a, uint8_t pin_b);
    ~rpi_driver();

    // CONTROL
    void start();
    void stop();

private:
    // PINS
    const uint8_t m_pin_a;
    const uint8_t m_pin_b;

    // CALLBACKS
    static void interrupt_a(int32_t pin, int32_t level, uint32_t timestamp, void* data);
    static void interrupt_b(int32_t pin, int32_t level, uint32_t timestamp, void* data);
};

}

#endif