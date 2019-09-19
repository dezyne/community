#pragma once

#include <iostream>

struct TimerID
{
	size_t createID() { return ++m_nextID; }
	size_t m_nextID = 0;
};
