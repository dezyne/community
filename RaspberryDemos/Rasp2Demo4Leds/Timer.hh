#pragma once

#include <dzn/pump.hh>
#include <hal.hh>

class Timer : public skel::Timer
{
public:
	Timer(const dzn::locator& locator);

private:
	void api_create(int miliseconds) override;
    void api_cancel() override;

private:
    dzn::pump& m_pump;
	size_t m_timerID;
};
