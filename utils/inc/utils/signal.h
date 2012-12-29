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

#ifndef UTILS_SIGNAL_H
#define UTILS_SIGNAL_H

#include <map>
#include <mutex>
#include <functional>

#include "functiontraits.h"

namespace utils
{

namespace details
{
    template <typename SlotFunction>
    class SignalBase
    {
    public:
        virtual ~SignalBase()
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Slots.clear();
        }
    
        void connect(SlotFunction func, const void* receiver)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Slots.insert(std::make_pair(receiver, func));
        }

        void disconnect(const void* receiver)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto iter = m_Slots.find(receiver);
            if (iter != m_Slots.end())
            {
                m_Slots.erase(iter);
            }
        }

    protected:
        std::map<const void*, SlotFunction>     m_Slots;
        std::mutex                              m_Mutex;
    };    

    template <typename SlotFunction>
    class Signal0 : public SignalBase<SlotFunction>
    {
        typedef SignalBase<SlotFunction> base;

    public:
        void operator()()
        {
            std::lock_guard<std::mutex> lock(base::m_Mutex);
            for (auto& func : base::m_Slots)
            {
                func.second();
            }
        }
    };

    template <typename Argument1Type, typename SlotFunction>
    class Signal1 : public SignalBase<SlotFunction>
    {
        typedef SignalBase<SlotFunction> base;

    public:
        void operator()(Argument1Type arg1)
        {
            std::lock_guard<std::mutex> lock(base::m_Mutex);
            for (auto& func : base::m_Slots)
            {
                func.second(arg1);
            }
        }
    };

    template <typename Argument1Type, typename Argument2Type, typename SlotFunction>
    class Signal2 : public SignalBase<SlotFunction>
    {
        typedef SignalBase<SlotFunction> base;

    public:
        void operator()(Argument1Type arg1, Argument2Type arg2)
        {
            std::lock_guard<std::mutex> lock(base::m_Mutex);
            for (auto& func : base::m_Slots)
            {
                func.second(arg1, arg2);
            }
        }
    };

    template <typename Argument1Type, typename Argument2Type, typename Argument3Type, typename SlotFunction>
    class Signal3 : public SignalBase<SlotFunction>
    {
        typedef SignalBase<SlotFunction> base;

    public:
        void operator()(Argument1Type arg1, Argument2Type arg2, Argument3Type arg3)
        {
            std::lock_guard<std::mutex> lock(base::m_Mutex);
            for (auto& func : base::m_Slots)
            {
                func.second(arg1, arg2, arg3);
            }
        }
    };

    template<int Arity, typename Signature, typename SlotFunction>
    class GetSignalImpl;

    template<typename Signature, typename SlotFunction>
    class GetSignalImpl<0, Signature, SlotFunction>
    {
    public:
        typedef Signal0<SlotFunction> type;
    };

    template<typename Signature, typename SlotFunction>
    class GetSignalImpl<1, Signature, SlotFunction>
    {
        typedef function_traits<Signature> traits;

    public:
        typedef Signal1<typename traits::arg1Type, SlotFunction> type;
    };

    template<typename Signature, typename SlotFunction>
    class GetSignalImpl<2, Signature, SlotFunction>
    {
        typedef function_traits<Signature> traits;

    public:
        typedef Signal2<typename traits::arg1Type, typename traits::arg2Type, SlotFunction> type;
    };

    template<typename Signature, typename SlotFunction>
    class GetSignalImpl<3, Signature, SlotFunction>
    {
        typedef function_traits<Signature> traits;

    public:
        typedef Signal3<typename traits::arg1Type, typename traits::arg2Type, typename traits::arg3Type, SlotFunction> type;
    };
}

template <typename Signature, typename SlotFunction = std::function<Signature>>
class Signal : public details::GetSignalImpl<function_traits<Signature>::arity, Signature, SlotFunction>::type
{
};

}

#endif