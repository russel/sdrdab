/*
 * resampler.h
 *
 *      Author: Kacper Patro patro.kacper@gmail.com
 */

#ifndef SRC_RESAMPLER_H_
#define SRC_RESAMPLER_H_

#include <samplerate.h>
#include <stdint.h>
#include <cstddef>

/**
 * @namespace ResamplerCallbacks
 * @brief This namespace provides necessary for Resampler class callbacks (libsamplerate callbacks)
 */
namespace ResamplerCallbacks {

/**
 * libsamplerate callback. Check libsamplerate documentation for more
 */
extern "C" long WriteCall(void *cb_data, float **data);

}

/**
 * @class Resampler
 * @brief Resampling - facade for libsamplerate
 *
 * Resampling should be fast, reliable and configurable, depending on needs
 *
 * @author Kacper Patro patro.kacper@gmail.com
 * @copyright Public domain
 * @pre libsamplerate
 */
class Resampler {
public:
	/**
	 * Resampler constructor
	 * @param[in] conv Resampler type, check out libsamplerate doc for valid values
	 * @param[in] chann Number of interleaved channels, for monophonic data should be one
	 */
	Resampler(int conv, int chann);
	virtual ~Resampler();

	/**
	 * Sets input buffer parameters
	 * @param[in] source Pointer to source buffer
	 * @param[in] length Number of items in source buffer
	 */
	void SetSourceBuffer(float *source, size_t length);

	/**
	 * Resamples data
	 * @param[out] destination_buffer Pointer to destination buffer
	 * @param[in] length Maximum number of items to write into destination buffer
	 * @param[in] ratio Resampling ratio
	 * @return Number of items written into destination buffer
	 */
	long Resample(float *destination_buffer, size_t length, float ratio);

	/**
	 * @struct Data
	 * @brief This struct contains Resampler input buffer parameters
	 */
	struct Data {
		size_t current_count;	/**< Number of frames read */
		size_t total;	/**< Total number of frames */
		float *in_data_ptr;	/**< Pointer to input data */
		int channels;	/**< Number of channels */
	};

private:
	Data data_;	/**< Internal Data element */
	int converter_type_;	/**< Converter type, check libsamplerate documentation for more */

	SRC_STATE *src_state_;	/**< Internal libsamplerate element state */

};

#endif /* SRC_RESAMPLER_H_ */
