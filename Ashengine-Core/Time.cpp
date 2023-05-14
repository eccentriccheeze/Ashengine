#include "Time.hpp"

int64_t TimeSpan::Nanoseconds() const
{
	return m_Duration.count();
}

int64_t TimeSpan::Microsecond() const
{
	return std::chrono::duration_cast<std::chrono::microseconds>(m_Duration).count();
}

int64_t TimeSpan::Milliseconds() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(m_Duration).count();
}

int64_t TimeSpan::Seconds() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(m_Duration).count();
}

int64_t TimeSpan::Minutes() const
{
	return std::chrono::duration_cast<std::chrono::minutes>(m_Duration).count();
}

int64_t TimeSpan::Hours() const
{
	return std::chrono::duration_cast<std::chrono::hours>(m_Duration).count();
}

TimeSpan TimePoint::operator-(const TimePoint& a_TimePoint) const
{
	TimeSpan Duration;
	Duration.m_Duration = m_TimePoint - a_TimePoint.m_TimePoint;
	return Duration;
}

TimePoint Time::Now()
{
	auto TimeNow = std::chrono::high_resolution_clock::now();
	TimePoint Result;
	Result.m_TimePoint = TimeNow;
	return Result;
}