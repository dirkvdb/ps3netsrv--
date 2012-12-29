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

#ifndef UTILS_SUBSCRIBER_H
#define UTILS_SUBSCRIBER_H

#include <string>

namespace utils
{

template <typename Subject>
class ISubscriber
{
public:
    virtual ~ISubscriber() {}

    virtual void onItem(Subject subject, void* pData = nullptr) = 0;
    virtual void finalItemReceived(void* pData = nullptr) {}
    virtual void onError(const std::string& message) {}
};

}

#endif
