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

#include "utils/log.h"

namespace utils
{

#ifdef CONSOLE_SUPPORTS_COLOR
    const std::string log::red         = "\033[31m";
    const std::string log::green       = "\033[32m";
    const std::string log::yellow      = "\033[33m";
    const std::string log::purple      = "\033[35m";
    const std::string log::standard    = "\033[39m";
#else
    const std::string log::red;
    const std::string log::green;
    const std::string log::yellow;
    const std::string log::purple;
    const std::string log::standard;
#endif

std::mutex log::m_Mutex;
std::ofstream* log::m_LogFile = nullptr;

}
