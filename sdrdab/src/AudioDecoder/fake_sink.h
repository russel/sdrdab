/*
 * fake_sink.h
 *
 *  Created on: May 13, 2015
 *      Author: Kacper Patro patro.kacper@gmail.com
 */

#ifndef SRC_FAKE_SINK_H_
#define SRC_FAKE_SINK_H_

#include "abstract_sink.h"
#include "player.h"
#include <gst/gst.h>
#include <stdint.h>

/**
 * @namespace FakeSinkCallbacks
 * @brief This namespace provides necessary for FakeSink class callbacks (mostly GStreamer callbacks)
 */
namespace FakeSinkCallbacks {

/**
 * GStreamer callback called when unlinking sink from pipeline. Check GStreamer documentation for more
 */
extern "C" GstPadProbeReturn UnlinkCallFakeSink(GstPad *, GstPadProbeInfo *, gpointer);

/**
 * GStreamer callback called when buffer present on sink. Check GStreamer documentation for more
 */
extern "C" void SinkHandoffCall(GstElement *, GstBuffer *, GstPad *, gpointer);

}

/**
 * @class FakeSink
 * @brief This class is used in unittests, provides specific necessary information about stream. Could be used multiple times in one pipeline
 *
 * @author Kacper Patro patro.kacper@gmail.com
 * @copyright Public domain
 * @pre
 */
class FakeSink: public AbstractSink {
public:
	FakeSink();
	virtual ~FakeSink();

	/**
	 * Get number of bytes received at the end of pipeline
	 * @return Number of bytes received
	 */
	uint32_t bytes_returned() const;

	void InitSink(void *player_data);
	const char *name() const;
	void Finish();
	bool linked() const;

	/**
	 * Gets number of linked sink elements
	 * @return Number of current linked sink elements
	 */
	uint32_t num_src_pads() const;

	/**
	 * Gets current playback speed, relative to 1.0
	 * @return Current playback speed
	 */
	float playback_speed() const;

	friend GstPadProbeReturn FakeSinkCallbacks::UnlinkCallFakeSink(GstPad *, GstPadProbeInfo *, gpointer);
	friend void FakeSinkCallbacks::SinkHandoffCall(GstElement *, GstBuffer *, GstPad *, gpointer);

private:
	/**
	 * @struct Data
	 * @brief This struct contains specific for FakeSink class elements
	 */
	struct Data {
		FakeSink *abstract_sink;	/**< Pointer to "this" sink element */

		GstElement *queue;	/**< Queue element for GStreamer */
		GstElement *sink;	/**< Sink element for GStreamer */
		GstPad *teepad;	/**< TeePad element for GStreamer */

		gboolean removing;	/**< True, when sink is being removed from pipeline */
		bool linked;	/**< True, when sink is linked in pipeline */

		Player::Data *player_data;	/**< Pointer to core Player data */
	};

	Data *data_;	/**< Pointer to internal Data element */

	uint32_t bytes_returned_;	/**< Number of bytes received at output */

	static uint16_t count_;	/**< Number of FakeSink instances */

	/**
	 * Adds bytes to bytes counter
	 * @param[in] bytes Number of bytes to add
	 */
	void AddBytes(uint32_t bytes);

};

#endif /* SRC_FAKE_SINK_H_ */
