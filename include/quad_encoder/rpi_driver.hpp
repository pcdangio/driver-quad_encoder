/// \file quad_encoder/rpi_driver.hpp
/// \brief Defines the quad_encoder::rpi_driver class.
#ifndef QUAD_ENCODER___RPI_DRIVER_H
#define QUAD_ENCODER___RPI_DRIVER_H

#include <quad_encoder/driver.hpp>

namespace quad_encoder {

/// \brief A Raspberry Pi driver for a Quad Encoder.
/// \details This driver uses the PIGPIO library.
class rpi_driver
    : public quad_encoder::driver
{
public:
    // CONSTRUCTORS
    /// \brief Creates a new rpi_driver instance.
    /// \param cpr The counts per revolution (CPR) of the quad encoder.
    /// \param pin_a The Broadcom / BCM GPIO pin number attached to signal A of the quad encoder.
    /// \param pin_b The Broadcom / BCM GPIO pin number attached to signal B of the quad encoder.
    rpi_driver(uint32_t cpr, uint8_t pin_a, uint8_t pin_b);
    ~rpi_driver();

    // CONTROL
    /// \brief Starts the rpi_driver's monitoring of the quad encoder.
    /// \exception runtime_error if either of the provided GPIO pins are invalid.
    /// \exception runtime_error if the start operation fails.
    void start();
    /// \brief Stops the rpi_driver's monitoring of the quad encoder.
    void stop();

private:
    // PINS
    /// \brief The BCM GPIO pin attached to signal A of the quad encoder.
    const uint8_t m_pin_a;
    /// \brief The BCM GPIO pin attached to signal B of the quad encoder.
    const uint8_t m_pin_b;

    // HIDDEN BASE METHODS
    using driver::initialize_state;
    using driver::tick_a;
    using driver::tick_b;

    // CALLBACKS
    /// \brief The edge-detection interrupt for pin A.
    /// \param pin The BCM GPIO pin number that an edge was detected for.
    /// \param level The type of edge detected.
    /// \param timestamp The timestamp of the detected edge.
    /// \param data A pointer to the rpi_driver instance.
    static void interrupt_a(int32_t pin, int32_t level, uint32_t timestamp, void* data);
    /// \brief The edge-detection interrupt for pin A.
    /// \param pin The BCM GPIO pin number that an edge was detected for.
    /// \param level The type of edge detected.
    /// \param timestamp The timestamp of the detected edge.
    /// \param data A pointer to the rpi_driver instance.
    static void interrupt_b(int32_t pin, int32_t level, uint32_t timestamp, void* data);
};

}

#endif