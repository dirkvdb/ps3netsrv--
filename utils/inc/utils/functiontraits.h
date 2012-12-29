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

#ifndef UTILS_FUNCTION_TRAITS
#define UTILS_FUNCTION_TRAITS

#include <type_traits>    

namespace utils
{

namespace details
{
    

template<typename Function> struct function_traits_helper;

template<typename R>
struct function_traits_helper<R (*)(void)>
{
    static const unsigned arity = 0;
    typedef R resultType;
};

template<typename R, typename T1>
struct function_traits_helper<R (*)(T1)>
{
    static const unsigned arity = 1;
    typedef R   resultType;
    typedef T1  arg1Type;
};

template<typename R, typename T1, typename T2>
struct function_traits_helper<R (*)(T1, T2)>
{
    static const unsigned arity = 2;
    typedef R   resultType;
    typedef T1  arg1Type;
    typedef T2  arg2Type;
};

template<typename R, typename T1, typename T2, typename T3>
struct function_traits_helper<R (*)(T1, T2, T3)>
{
    static const unsigned arity = 3;
    typedef R   resultType;
    typedef T1  arg1Type;
    typedef T2  arg2Type;
    typedef T3  arg3Type;
};

template<typename R, typename T1, typename T2, typename T3, typename T4>
struct function_traits_helper<R (*)(T1, T2, T3, T4)>
{
    static const unsigned arity = 4;
    typedef R   resultType;
    typedef T1  arg1Type;
    typedef T2  arg2Type;
    typedef T3  arg3Type;
    typedef T4  arg4Type;
};

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
struct function_traits_helper<R (*)(T1, T2, T3, T4, T5)>
{
    static const unsigned arity = 5;
    typedef R   resultType;
    typedef T1  arg1Type;
    typedef T2  arg2Type;
    typedef T3  arg3Type;
    typedef T4  arg4Type;
    typedef T5  arg5Type;
};

}

template<typename Function>
struct function_traits : public details::function_traits_helper<typename std::add_pointer<Function>::type>
{
};

}

#endif
