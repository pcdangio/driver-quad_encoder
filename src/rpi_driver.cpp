#include <quad_encoder/rpi_driver.hpp>

#include <pigpio.h>

#include <stdexcept>
#include <string>
#include <limits>

using namespace quad_encoder;

// CONSTRUCTORS
rpi_driver::rpi_driver(uint32_t cpr, uint8_t pin_a, uint8_t pin_b)
    : driver(cpr),
      m_pin_a(pin_a),
      m_pin_b(pin_b)
{}

// CONTROL
void rpi_driver::start()
{
    // Initialize GPIO pins and attach interrupts.

    // Set up pin A and handle it's result.
    // NOTE: this method will internally set the mode for the pin.
    switch(gpioSetISRFuncEx(rpi_driver::m_pin_a, EITHER_EDGE, std::numeric_limits<int32_t>::max(), &rpi_driver::interrupt_a, this))
    {
        case 0:
        {
            // Do nothing, succeeded.
            break;
        }
        case PI_BAD_GPIO:
        {
            throw std::runtime_error("invalid gpio pin: " + std::to_string(rpi_driver::m_pin_a));
        }
        case PI_BAD_ISR_INIT:
        {
            throw std::runtime_error("initialize interrupt failed for pin: " + std::to_string(rpi_driver::m_pin_a));
        }
        default:
        {
            throw std::runtime_error("unknown error");
        }
    }

    // Set up pin B and handle it's result code.
    switch(gpioSetISRFuncEx(rpi_driver::m_pin_b, EITHER_EDGE, std::numeric_limits<int32_t>::max(), &rpi_driver::interrupt_b, this))
    {
        case 0:
        {
            // Do nothing, succeeded.
            break;
        }
        case PI_BAD_GPIO:
        {
            throw std::runtime_error("invalid gpio pin: " + std::to_string(rpi_driver::m_pin_b));
        }
        case PI_BAD_ISR_INIT:
        {
            throw std::runtime_error("initialize interrupt failed for pin: " + std::to_string(rpi_driver::m_pin_b));
        }
        default:
        {
            throw std::runtime_error("unknown error");
        }
    }

    // Initialize quad encoder state.
    // NOTE: GPIO has already been verified by this point.
    rpi_driver::initialize_state(static_cast<bool>(gpioRead(rpi_driver::m_pin_a)), static_cast<bool>(gpioRead(rpi_driver::m_pin_b)));
}
void rpi_driver::stop()
{
    // Disable pin interrupts.
    gpioSetISRFuncEx(rpi_driver::m_pin_a, EITHER_EDGE, std::numeric_limits<int32_t>::max(), nullptr, nullptr);
    gpioSetISRFuncEx(rpi_driver::m_pin_b, EITHER_EDGE, std::numeric_limits<int32_t>::max(), nullptr, nullptr);
}

// CALLBACKS
void rpi_driver::interrupt_a(int32_t pin, int32_t level, uint32_t timestamp, void* data)
{
    // Cast rpi_driver pointer to the base driver pointer.
    quad_encoder::driver* driver = reinterpret_cast<quad_encoder::driver*>(data);

    // Handle level.
    switch(level)
    {
        case 0:
        {
            driver->tick_a(false);
            break;
        }
        case 1:
        {
            driver->tick_a(true);
            break;
        }
        default:
        {
            // Do nothing.
        }
    }
}
void rpi_driver::interrupt_b(int32_t pin, int32_t level, uint32_t timestamp, void* data)
{
    // Cast rpi_driver pointer to the base driver pointer.
    quad_encoder::driver* driver = reinterpret_cast<quad_encoder::driver*>(data);

    // Handle level.
    switch(level)
    {
        case 0:
        {
            driver->tick_b(false);
            break;
        }
        case 1:
        {
            driver->tick_b(true);
            break;
        }
        default:
        {
            // Do nothing.
        }
    }
}