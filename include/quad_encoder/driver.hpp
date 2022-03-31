/// \file quad_encoder/driver.hpp
/// \brief Defines the quad_encoder::driver class.
#ifndef QUAD_ENCODER___DRIVER_H
#define QUAD_ENCODER___DRIVER_H

#include <stdint.h>

/// \brief Contains all code related to the quadrature encoder driver.
namespace quad_encoder {

/// \brief A base driver for a quadrature encoder sensor.
class driver
{
public:
    // CONSTRUCTORS
    /// \brief Creates a new driver instance.
    /// \param cpr The counts per revolution of the encoder.
    driver(uint32_t cpr);
    virtual ~driver();

    // ACCESS
    /// \brief Sets the home position of the axis to the current position.
    void set_home();
    /// \brief Gets the relative position of the axis to the home position in radians.
    /// \param reset Resets the position to zero after reading it. DEFAULT = FALSE
    /// \returns The current position in radians.
    double get_position(bool reset = false);
    /// \brief Gets the amount of pulses missed by the encoder.
    /// \returns The number of pulses missed.
    uint64_t pulses_missed();

protected:
    // STATE UPDATE
    /// \brief Initializes the encoder's state.
    /// \param level_a The state of signal A.
    /// \param level_b The state of signal B.
    void initialize_state(bool a, bool b);
    /// \brief Increments the encoder's state with an A signal pulse.
    /// \param level The new logic level of the A signal.
    void tick_a(bool level);
    /// \brief Increments the encoder's state with an B signal pulse.
    /// \param level The new logic level of the B signal.
    void tick_b(bool level);

private:
    // PARAMETERS
    /// \brief The counts per revolution of the quad encoder.
    const uint32_t m_cpr;

    // STATE
    /// \brief The matrix for determing transitions between states.
    const int8_t m_transition_matrix[4][4];
    /// \brief The last measured encoder state.
    int8_t m_prior_state;
    /// \brief The current position of the encoder, in pulse count.
    int64_t m_current_position;
    /// \brief The number of missed pulses counted.
    uint64_t m_pulses_missed;
    /// \brief Updates the encoder with a new state.
    /// \param new_state The new state to update with.
    void update_state(int8_t new_state);
};

}

#endif