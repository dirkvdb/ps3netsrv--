//    Copyright (C) 2012 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef UTILS_TIME_OPERATIONS_H
#define UTILS_TIME_OPERATIONS_H

#include "types.h"

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN 1
    #include <windows.h>
    #include <Mmsystem.h>
    #undef max
    #undef DELETE
#else
    #include <sys/time.h>
    #include <unistd.h>
#endif

#include <cassert>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace utils
{
namespace timeops
{
    inline uint64_t getTimeInMilliSeconds()
    {
#ifdef WIN32
    return static_cast<uint64_t>(timeGetTime());
#else
    struct timeval timeValue;
    
    int32_t error = gettimeofday(&timeValue, nullptr);
    assert(!error);
    
    return (timeValue.tv_sec * 1000) + (timeValue.tv_usec / 1000);
#endif
}

    inline void sleepMs(uint32_t ms)
    {
#ifdef WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
    }

    inline std::string getTimeString()
    {
        struct timeval timeValue;
    
        int32_t error = gettimeofday(&timeValue, nullptr);
        assert(!error);

        struct tm* timePtr = gmtime(&timeValue.tv_sec);
        char timeString[128];
        strftime(timeString, 127, "%H:%M:%S.", timePtr);
        
        std::stringstream ss;
        ss << timeString << std::setw(3) << std::setfill('0') << (timeValue.tv_usec / 1000);

        return ss.str();
    }
}
}

#endif
