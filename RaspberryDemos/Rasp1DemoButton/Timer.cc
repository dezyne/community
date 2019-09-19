#include <iostream>
#include "Timer.hh"
#include "TimerID.hh"

Timer::Timer(const dzn::locator& locator) :
	skel::Timer(locator),
	m_pump(locator.get<dzn::pump>()),
	m_timerID(locator.get<TimerID>().createID())
{
}

void Timer::api_create(int miliseconds)
{
	m_pump.handle(m_timerID, miliseconds, api.out.timeout);
}

void Timer::api_cancel()
{
	m_pump.remove(m_timerID);
}
