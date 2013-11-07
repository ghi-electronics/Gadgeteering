#ifndef SYSTEM_PROXY_HPP_INCLUDED
#define SYSTEM_PROXY_HPP_INCLUDED

#define SWIG

#ifdef SWIG
#include "System.hpp"

/*
    \brief: System Sleep bridge handler

    Causes the current thread to pause execution for the specified amount of miliseconds.
*/
void GADGETEERING_PROXY_SLEEP(unsigned long time)
{
    GHI::System::Sleep(time);
}

/*
    \brief: System SleepMicro bridge handler

    Causes the current thread to pause execution for the specified amount of microseconds.
*/
void GADGETEERING_PROXY_SLEEP_MICRO(unsigned long time)
{
    GHI::System::SleepMicro(time);
}

/*
    \brief: System TimeElapsed bridge handler

    Returns the number of miliseconds elapsed.
*/
unsigned long GADGETEERING_PROXY_TIME_ELAPSED()
{
    return GHI::System::TimeElapsed();
}
#endif

#endif // SYSTEM_PROXY_HPP_INCLUDED
