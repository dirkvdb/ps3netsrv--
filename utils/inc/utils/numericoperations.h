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

#ifndef UTILS_NUMERIC_OPERATIONS_H
#define UTILS_NUMERIC_OPERATIONS_H

#include <sstream>
#include <string>

namespace utils
{
namespace numericops
{

template<typename T>
inline void clip(T& value, const T& lowerLimit, const T& upperLimit)
{
    if (value < lowerLimit)
    {
        value = lowerLimit;
    }
    else if (value > upperLimit)
    {
        value = upperLimit;
    }
}

template<typename T>
inline void clipLow(T& value, const T& lowerLimit)
{
    if (value < lowerLimit)
    {
        value = lowerLimit;
    }
}

template<typename T>
inline void clipHigh(T& value, const T& upperLimit)
{
    if (value > upperLimit)
    {
        value = upperLimit;
    }
}

template<typename T>
inline void toString(T value, std::string& aString)
{
    std::stringstream ss;
    ss << value;
    aString = ss.str();
}

template<typename T>
inline std::string toString(T value)
{
    std::string result;
    toString(value, result);
    return result;
}

}
}

#endif
