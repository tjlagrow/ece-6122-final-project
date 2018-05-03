#include <cstring>
#include "PerformanceData.h"

static const int pad = -11;

/**
 * Constructor
 */
PerformanceData::PerformanceData()
{
	snprintf(m_strFrames, sizeof(m_strFrames), "%*s: 0", pad, "frames");
	snprintf(m_strElapsed, sizeof(m_strElapsed), "%*s: 0.000", pad, "elapsed (s)");
	snprintf(m_strFps, sizeof(m_strFps), "%*s: 0.000", pad, "frames/sec");
	m_timeCreated = getTimespec();
	m_timeStart = m_timeCreated;
	m_timeNow = m_timeCreated;
	m_framesTotal = 0;
	m_framesCurrent = 0;
}

/**
 * Returns the current time in timespec format
 * @return Returns the current time in timespec format
 */
struct timespec PerformanceData::getTimespec()
{
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (time);
}

/**
 * Returns the current time in double format
 * @return Returns the current time in double format
 */
double PerformanceData::getTimeDouble()
{
	struct timespec time = getTimespec();
	return (convertTimespecToDouble(time));
}

/**
 * Convert the timespec to double
 * @param time The timespec struct to convert
 * @return Returns the double representation of the timespec
 */
double PerformanceData::convertTimespecToDouble(struct timespec &time)
{
	return (time.tv_sec + 1e-9 * time.tv_nsec);
}

/**
 * Increment the frames counter
 */
void PerformanceData::incrementFrames()
{
	m_framesTotal++;
	m_framesCurrent++;
}

/**
 * Return the number of frames
 * @return Return the number of frames
 */
const unsigned int &PerformanceData::getFramesCurrent() const
{
	return (m_framesCurrent);
}

/**
 * Return the number of frames
 * @return Return the number of frames
 */
const unsigned int &PerformanceData::getFramesTotal() const
{
	return (m_framesTotal);
}

/**
 * Resets the performance counters
 */
void PerformanceData::reset()
{
	m_timeStart = getTimespec();
	m_timeNow = m_timeStart;
	m_framesCurrent = 0;
}

/**
 * Updates the current timestamp and returns the elapsed time since
 * reset() was called
 * @return Returns the elapsed time in seconds since reset()
 */
double PerformanceData::getElapsedTime()
{
	m_timeNow = getTimespec();
	return (convertTimespecToDouble(m_timeNow)- convertTimespecToDouble(m_timeStart));
}

/**
 * Updates the current timestamp and returns the elapsed time since
 * class creation was called
 * @return Returns the elapsed time in seconds since class creation
 */
double PerformanceData::getElapsedTimeSinceCreation()
{
	m_timeNow = getTimespec();
	return (convertTimespecToDouble(m_timeNow)- convertTimespecToDouble(m_timeCreated));
}

/**
 * Set all the performance statistics
 */
void PerformanceData::updateStats()
{
	memset(m_strFrames, 0, sizeof(m_strFrames));
	memset(m_strElapsed, 0, sizeof(m_strElapsed));
	memset(m_strFps, 0, sizeof(m_strFps));
	snprintf(m_strFrames, sizeof(m_strFrames), "%*s: %u",
		pad, "frames", m_framesCurrent);
	snprintf(m_strElapsed, sizeof(m_strElapsed), "%*s: %0.3f",
		pad, "elapsed (s)", getElapsedTimeSinceCreation());
	snprintf(m_strFps, sizeof(m_strFps), "%*s: %0.3f",
		pad, "frames/sec", m_framesCurrent / getElapsedTime());
}

/**
 * Returns the string of the frame counter since the last time
 * updateStats() was called
 * @return
 */
const char *PerformanceData::getFramesStr() const
{
	return m_strFrames;
}

/**
 * Returns the string of the elapsed time since creation
 * @return Returns the elapsed time since creation string
 */
const char *PerformanceData::getElapsedStr() const
{
	return m_strElapsed;
}

/**
 * Returns the string of the frames per second since the last time
 * updateStats() was called
 * @return Returns the frames per second string
 */
const char *PerformanceData::getFpsStr() const
{
	return m_strFps;
}
