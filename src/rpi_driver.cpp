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
rpi_driver::~rpi_driver()
{
    // Stop the driver if it's running.
    rpi_driver::stop();
}

// CONTROL
void rpi_driver::start()
{
    // Initialize GPIO pins and attach interrupts.
    // NOTE: this method will internally set the mode for the pin.
    int32_t result = gpioSetISRFuncEx(rpi_driver::m_pin_a, EITHER_EDGE, std::numeric_limits<int32_t>::max(), &rpi_driver::interrupt_a, this);
    if(result == 0)
    {
        result = gpioSetISRFuncEx(rpi_driver::m_pin_b, EITHER_EDGE, std::numeric_limits<int32_t>::max(), &rpi_driver::interrupt_b, this);
    }
    else if(result == PI_BAD_GPIO)
    {
        throw std::runtime_error("invalid gpio pin: " + std::to_string(rpi_driver::m_pin_a));
    }
    if(result == PI_BAD_GPIO)
    {
        throw std::runtime_error("invalid gpio pin: " + std::to_string(rpi_driver::m_pin_b));
    }

    // Initialize the state of the encoder with the current pin values.
    // NOTE: If this point reached, GPIO are already verified as good.
    if(result == 0)
    {
        rpi_driver::initialize_state(static_cast<bool>(gpioRead(rpi_driver::m_pin_a)), static_cast<bool>(gpioRead(rpi_driver::m_pin_b)));
    }
    else
    {
        switch(result)
        {
            case 0:
            {
                // Do nothing, initialization succeeded.
                return;
            }
            case PI_BAD_ISR_INIT:
            {
                throw std::runtime_error("initialize interrupt failed");
            }
            default:
            {
                throw std::runtime_error("unknown error");
            }
        }
    }
}
void rpi_driver::stop()
{

}
void rpi_driver::start_pin(uint8_t pin)
{
    // Set up edge-detection interrupt for the pin.
    // NOTE: this method will internally set the mode for the pin.
    switch(gpioSetISRFuncEx(pin, EITHER_EDGE, std::numeric_limits<int32_t>::max(), &rpi_driver::interrupt_callback, this))
    {
        case 0:
        {
            // Do nothing, initialization succeeded.
            return;
        }
        case PI_BAD_GPIO:
        {
            throw std::runtime_error("invalid gpio pin: " + std::to_string(pin));
        }
        case PI_BAD_ISR_INIT:
        {
            throw std::runtime_error("initialize interrupt failed");
        }
        default:
        {
            throw std::runtime_error("unknown error");
        }
    }
}
bool rpi_driver::read_pin(uint8_t pin)
{
    switch(gpioRead(pin))
    {
        case 0:
        {
            return false;
        }
        case 1:
        {
            return true;
        }
        case PI_BAD_GPIO:
        {
            throw std::runtime_error("invalid gpio pin: " + std::to_string(pin));
        }
        default:
        {
            throw std::runtime_error("unknown error");
        }
    }
}
void rpi_driver::stop_pin(uint8_t pin)
{
    // Disable edge-detection interrupt for the pin.
    switch(gpioSetISRFuncEx(pin, EITHER_EDGE, std::numeric_limits<int32_t>::max(), nullptr, nullptr))
    {
        case 0:
        {
            // Do nothing, disable succeeded.
            return;
        }
        case PI_BAD_GPIO:
        {
            throw std::runtime_error("invalid gpio pin: " + std::to_string(pin));
        }
        case PI_BAD_ISR_INIT:
        {
            throw std::runtime_error("disable interrupt failed");
        }
        default:
        {
            throw std::runtime_error("unknown error");
        }
    }
}

// CALLBACKS
void rpi_driver::interrupt_a(int32_t pin, int32_t level, uint32_t timestamp, void* data)
{
    // Get pointer to the rpi_driver instance.
    quad_encoder::rpi_driver* driver = reinterpret_cast<quad_encoder::rpi_driver*>(data);

    // Validate level.
    if(level == 0 || level == 1)
    {
        driver->tick_a(static_cast<bool>(level));
    }
}
void rpi_driver::interrupt_b(int32_t pin, int32_t level, uint32_t timestamp, void* data)
{
    // Get pointer to the rpi_driver instance.
    quad_encoder::rpi_driver* driver = reinterpret_cast<quad_encoder::rpi_driver*>(data);

    // Validate level.
    if(level == 0 || level == 1)
    {
        driver->tick_b(static_cast<bool>(level));
    }
}