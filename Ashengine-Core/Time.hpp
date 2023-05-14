#pragma once
#include <chrono>

class TimeSpan
{
public:

	TimeSpan() = default;

	int64_t Nanoseconds() const;
	int64_t Microsecond() const;
	int64_t Milliseconds() const;
	int64_t Seconds() const;
	int64_t Minutes() const;
	int64_t Hours() const;

private:

	friend class TimePoint;

	std::chrono::nanoseconds m_Duration;
};

class TimePoint
{
public:

	TimePoint() = default;

	TimeSpan operator-(const TimePoint& a_TimePoint) const;

private:

	friend class Time;

	std::chrono::high_resolution_clock::time_point m_TimePoint;
};

class Time
{
public:

	static TimePoint Now();
};