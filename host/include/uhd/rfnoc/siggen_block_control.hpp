//
// Copyright 2020 Ettus Research, a National Instruments Brand
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <uhd/config.hpp>
#include <uhd/rfnoc/noc_block_base.hpp>
#include <uhd/utils/math.hpp>
#include <complex>

namespace uhd { namespace rfnoc {

enum class siggen_waveform { CONSTANT, SINE_WAVE, NOISE };

/*! Siggen Control Class
 *
 * \ingroup rfnoc_blocks
 *
 * The Siggen Block is an RFNoC block that acts as a simple function
 * generating source block. The block supports three functions: generating a
 * constant value, generating a sinusoidal wave with a configurable amplitude
 * and phase increment between samples (but with a random initial phase
 * offset), and a noise source.
 */
class UHD_API siggen_block_control : public noc_block_base
{
public:
    RFNOC_DECLARE_BLOCK(siggen_block_control);

    static const uint32_t REG_BLOCK_SIZE;
    static const uint32_t REG_ENABLE_OFFSET;
    static const uint32_t REG_SPP_OFFSET;
    static const uint32_t REG_WAVEFORM_OFFSET;
    static const uint32_t REG_GAIN_OFFSET;
    static const uint32_t REG_CONSTANT_OFFSET;
    static const uint32_t REG_PHASE_INC_OFFSET;
    static const uint32_t REG_CARTESIAN_OFFSET;

    /*! Set the function generator stream enable flag
     *
     * Enables or disables the stream of function generator data from the
     * given port on the block.
     *
     * \param enable Stream enable/disable flag
     * \param port The port on the block to enable or disable
     */
    virtual void set_enable(const bool enable, const size_t port) = 0;

    /*! Get the function generator stream enable flag
     *
     * Returns the current enable flag for function generator stream data from
     * the given port on the block.
     *
     * \param port The port on the block to get the flag from
     * \returns Function generator stream data enable/disable flag
     */
    virtual bool get_enable(const size_t port) const = 0;

    /*! Set the function generator waveform type
     *
     * Sets the waveform type that the given port on the block should generate
     * when enabled.
     *
     * \param type Function generator waveform type
     * \param port The port on the block whose waveform type to set
     */
    virtual void set_waveform(const siggen_waveform type, const size_t port) = 0;

    /*! Get the function generator waveform type
     *
     * Returns the current waveform type for the given port on the block.
     *
     * \param port The port on the block whose waveform type to return
     * \returns Waveform type
     */
    virtual siggen_waveform get_waveform(const size_t port) const = 0;

    /*! Set the amplitude value for noise and sine wave data
     *
     * Sets the maximum amplitude of the data generated by the given port
     * on the block. The hardware can generate function data with an
     * amplitude between -1.0 and (2^15-1)/(2^15), or approximately 0.99997,
     * so the provided value must reside within this range. However, for
     * convenience, clients may pass 1.0 to get the maximum amplitude that
     * can be generated.
     *
     * Setting the amplitude applies only to the noise and to the sine wave
     * functions. The value generated when in constant mode is the value that
     * is configured via the set_constant() function. Calling this function
     * when in constant mode will range check the amplitude value, but will
     * otherwise have no impact on the generated signal.
     *
     * \param amplitude Amplitude of sine wave and noise data
     * \param port The port on the block whose amplitude value to set
     */
    virtual void set_amplitude(const double amplitude, const size_t port) = 0;

    /*! Get the amplitude value for noise and sine wave data
     *
     * Returns the current maximum amplitude of the data generated by
     * the given port on the block. The value only applies to the noise and
     * sine wave functions. Calling this function when in constant mode will
     * return 1.0.
     *
     * \param port The port on the block whose amplitude value to return
     * \returns Amplitude value
     */
    virtual double get_amplitude(const size_t port) const = 0;

    /*! Set the constant value to generate in constant mode
     *
     * Sets the value that should be generated by the function generator on
     * the given block when the block is configured in constant mode. The
     * hardware can output a complex value where the real and imaginary
     * components can be between -1.0 and (2^15-1)/(2^15), or approximately
     * 0.99997, so the provided value must reside within this range. However,
     * for convenience, clients may pass 1.0 for either component to get the
     * maximum positive value that can be generated.
     *
     * \param constant The constant value to generate
     * \param port The port on the block whose constant value to set
     */
    virtual void set_constant(const std::complex<double> constant, const size_t port) = 0;

    /*! Get the constant value to generate in constant mode
     *
     * Returns the current constant value to be generated by the function
     * generator on the given block when the block is configured in constant
     * mode.
     *
     * \param port The port on the block whose constant value to return
     * \returns Constant value to generate
     */
    virtual std::complex<double> get_constant(const size_t port) const = 0;

    /*! Set the phase increment between samples in sine wave mode
     *
     * Sets the phase increment between successive samples generated by the
     * function generator on the given block when the block is configured in
     * sine wave mode. The value must be in the range [-pi, pi].
     *
     * \param phase_inc The phase increment between samples
     * \param port The port on the block whose phase increment to set
     */
    virtual void set_sine_phase_increment(const double phase_inc, const size_t port) = 0;

    /*! Get the phase increment between samples in sine wave mode
     *
     * Returns the current phase increment between successive samples generated
     * by the function generator on the given block when the block is
     * configured in sine wave mode.
     *
     * \param port The port on the block whose phase increment to return
     * \returns Phase increment between samples
     */
    virtual double get_sine_phase_increment(const size_t port) const = 0;

    /*! Set the number of samples per packet to generate
     *
     * Sets the number of samples per packet to be generated by the function
     * generator on the given block. The value will be coerced such that
     * outgoing packets will not exceed the graph MTU.
     *
     * \param spp Number of samples per packet to generate
     * \param port The port on the block whose samples per packet to set
     */
    virtual void set_samples_per_packet(const size_t spp, const size_t port) = 0;

    /*! Get the number of samples per packet to generate
     *
     * Returns the current number of samples per packet generated by the
     * function generator on the given block.
     *
     * \param port The port on the block whose samples per packet value to get
     * \returns Number of samples per packet to generate
     */
    virtual size_t get_samples_per_packet(const size_t port) const = 0;

    /*! Configure the sinusoidal waveform generator given frequency and rate
     *
     * Convenience function to configure the current phase increment between
     * successive samples generated by the sinusoidal function generator
     * given the desired frequency and assumed sample rate.
     *
     * \param frequency The desired frequency of the sinusoid
     * \param sample_rate The assumed sample rate
     * \param port The port on the block whose phase increment to set
     */
    inline void set_sine_frequency(
        const double frequency, const double sample_rate, const size_t port)
    {
        if (sample_rate <= 0.0) {
            throw uhd::value_error("sample_rate must be > 0.0");
        }
        const double phase_inc = (frequency / sample_rate) * 2.0 * uhd::math::PI;
        if (phase_inc < -uhd::math::PI || phase_inc > uhd::math::PI) {
            throw uhd::value_error("frequency must be in [-samp_rate/2, samp_rate/2]");
        }
        set_sine_phase_increment(phase_inc, port);
    }
};

}} // namespace uhd::rfnoc
