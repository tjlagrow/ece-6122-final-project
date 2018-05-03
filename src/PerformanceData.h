#ifndef ECE_6122_FINAL_PROJECT_PERFORMANCEDATA_H
#define ECE_6122_FINAL_PROJECT_PERFORMANCEDATA_H


#include <ctime>
#include <cstdio>

class PerformanceData
{
public:
	PerformanceData();

	void incrementFrames();
	const unsigned int &getFrames() const;

	void updateStats();

	void reset();
	double getElapsedTime();
	double getElapsedTimeSinceCreation();

	const char *getFramesStr() const;
	const char *getElapsedStr() const;
	const char *getFpsStr() const;

private:
	inline struct timespec getTimespec();
	inline double getTimeDouble();
	inline double convertTimespecToDouble(struct timespec &time);

	unsigned int m_frames;
	char m_strFrames[32];
	char m_strElapsed[32];
	char m_strFps[32];
	struct timespec m_timeCreated;
	struct timespec m_timeStart;
	struct timespec m_timeNow;
};


#endif //ECE_6122_FINAL_PROJECT_PERFORMANCEDATA_H
