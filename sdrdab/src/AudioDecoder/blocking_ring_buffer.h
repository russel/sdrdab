/*
 * blocking_ring_buffer.h
 *
 *  Created on: May 24, 2015
 *      Author: Kacper Patro patro.kacper@gmail.com
 */

#ifndef SRC_BLOCKING_RING_BUFFER_H_
#define SRC_BLOCKING_RING_BUFFER_H_

#include "RingBuffer/ring_buffer.h"
#include <stdint.h>
#include <pthread.h>

/**
 * @class BlockingRingBuffer
 * @brief Based on Paweł Szulc implementation, this buffer blocks when empty and overrides oldest data when overflowed
 *
 * @author Kacper Patro patro.kacper@gmail.com
 * @copyright Public domain
 * @pre
 */
class BlockingRingBuffer {
public:
	/**
	 * Constructor of BlockingRingBuffer
	 * @param[in] threshold Numer of items below which buffer will block
	 * @param[in] length Length of internal ring buffer
	 */
	BlockingRingBuffer(size_t threshold, size_t length);
	virtual ~BlockingRingBuffer();

	/**
	 * Reads from internal ring buffer, could block when empty
	 * @param[out] dest_buffer Pointer to destination buffer
	 * @param[in] how_many Number of elements to save into destination buffer
	 * @return Number of elements written into destination buffer
	 */
	size_t ReadFrom(uint8_t *dest_buffer, size_t how_many);

	/**
	 * Writes into internal ring buffer, could override oldest data when full
	 * @param[in] source_buffer Pointer to source buffer
	 * @param[in] how_many Number of elements to read from source buffer
	 * @return Number of elements written into internal buffer
	 *
	 * @todo source_buffer should be const pointer
	 */
	size_t WriteInto(uint8_t *source_buffer, size_t how_many);

	/**
	 * Checks free space
	 * @return Free space of internal buffer
	 */
	size_t FreeSpace() const;

	/**
	 * Checks data stored
	 * @return Number of items stored
	 */
	size_t DataStored() const;

	/**
	 * Checks last frame property
	 * @return True when set, false otherwise
	 */
	bool last_frame() const;

	/**
	 * Sets last frame property
	 * @param to
	 */
	void set_last_frame(bool to);

	/**
	 * Checks force reading property
	 * @return True when set, false otherwise
	 */
	bool force_read() const;

	/**
	 * Sets force reading property
	 * @param to
	 */
	void set_force_read(bool to);

	/**
	 * Resets buffer state
	 */
	void Reset();

	/**
	 * Forces count_condition_not_empty_ signal. Useful when forcing instant read without threshold. force_read_ must be set to true to take effect
	 */
	void ForceSignal();

private:
	pthread_mutex_t count_mutex_;	/**< Mutex for conditional variable */
	pthread_cond_t count_condition_not_empty_;	/**< Conditional variable, affects blocking */

	size_t threshold_;	/**< Threshold below which buffer will block */
	bool force_read_;	/**< Forces instant read */
	bool last_frame_;	/**< Indicates that last frame has been already written into internal buffer */

	RingBuffer<uint8_t> buffer_;	/**< RingBuffer object */

};

#endif /* SRC_BLOCKING_RING_BUFFER_H_ */
