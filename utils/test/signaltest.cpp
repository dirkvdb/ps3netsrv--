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

#include "utils/signal.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace utils;
using namespace testing;
using namespace std::placeholders;

template <typename ArgType>
class ReceiverMock
{
public:
    MOCK_METHOD0_T(onItem, void());
    MOCK_METHOD1_T(onItem1, void(ArgType));
    MOCK_METHOD2_T(onItem2, void(ArgType, ArgType));
    MOCK_METHOD3_T(onItem3, void(ArgType, ArgType, ArgType));
};

class SignalTest : public Test
{
protected:
    ReceiverMock<const int&>        m_Mock1;
    ReceiverMock<const int&>        m_Mock2;
};

TEST_F(SignalTest, ConnectDisconnect)
{
    Signal<void ()> sig;
    EXPECT_CALL(m_Mock1, onItem()).Times(0);
    EXPECT_CALL(m_Mock2, onItem()).Times(0);

    sig();
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);

    sig.connect(std::bind(&ReceiverMock<const int&>::onItem, &m_Mock1), &m_Mock1);
    sig.connect(std::bind(&ReceiverMock<const int&>::onItem, &m_Mock2), &m_Mock2);

    EXPECT_CALL(m_Mock1, onItem()).Times(1);
    EXPECT_CALL(m_Mock2, onItem()).Times(1);

    sig();
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);

    EXPECT_CALL(m_Mock1, onItem()).Times(0);
    EXPECT_CALL(m_Mock2, onItem()).Times(1);
    
    sig.disconnect(&m_Mock1);
    sig();
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);
    
    EXPECT_CALL(m_Mock1, onItem()).Times(0);
    EXPECT_CALL(m_Mock2, onItem()).Times(0);

    sig.disconnect(&m_Mock2);

    sig();
}

TEST_F(SignalTest, ConnectDisconnect1)
{
    Signal<void (const int&)> sig;
    EXPECT_CALL(m_Mock1, onItem1(_)).Times(0);
    EXPECT_CALL(m_Mock2, onItem1(_)).Times(0);

    sig(0);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);

    sig.connect(std::bind(&ReceiverMock<const int&>::onItem1, &m_Mock1, _1), &m_Mock1);
    sig.connect(std::bind(&ReceiverMock<const int&>::onItem1, &m_Mock2, _1), &m_Mock2);

    EXPECT_CALL(m_Mock1, onItem1(1)).Times(1);
    EXPECT_CALL(m_Mock2, onItem1(1)).Times(1);

    sig(1);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);

    EXPECT_CALL(m_Mock1, onItem1(_)).Times(0);
    EXPECT_CALL(m_Mock2, onItem1(2)).Times(1);
    
    sig.disconnect(&m_Mock1);
    sig(2);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);
    
    EXPECT_CALL(m_Mock1, onItem1(_)).Times(0);
    EXPECT_CALL(m_Mock2, onItem1(_)).Times(0);

    sig.disconnect(&m_Mock2);

    sig(2);
}

TEST_F(SignalTest, ConnectDisconnect2)
{
    Signal<void (const int&, const int&)> sig;
    EXPECT_CALL(m_Mock1, onItem2(_, _)).Times(0);
    EXPECT_CALL(m_Mock2, onItem2(_, _)).Times(0);

    sig(0, 0);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);

    sig.connect(std::bind(&ReceiverMock<const int&>::onItem2, &m_Mock1, _1, _2), &m_Mock1);
    sig.connect(std::bind(&ReceiverMock<const int&>::onItem2, &m_Mock2, _1, _2), &m_Mock2);

    EXPECT_CALL(m_Mock1, onItem2(1, 5)).Times(1);
    EXPECT_CALL(m_Mock2, onItem2(1, 5)).Times(1);

    sig(1, 5);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);

    EXPECT_CALL(m_Mock1, onItem2(_, _)).Times(0);
    EXPECT_CALL(m_Mock2, onItem2(2, 7)).Times(1);
    
    sig.disconnect(&m_Mock1);
    sig(2, 7);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);
    
    EXPECT_CALL(m_Mock1, onItem2(_, _)).Times(0);
    EXPECT_CALL(m_Mock2, onItem2(_, _)).Times(0);

    sig.disconnect(&m_Mock2);

    sig(2, 7);
}

TEST_F(SignalTest, ConnectDisconnect3)
{
    Signal<void (const int&, const int&, const int&)> sig;
    EXPECT_CALL(m_Mock1, onItem3(_, _, _)).Times(0);
    EXPECT_CALL(m_Mock2, onItem3(_, _, _)).Times(0);

    sig(0, 0, 0);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);

    sig.connect(std::bind(&ReceiverMock<const int&>::onItem3, &m_Mock1, _1, _2, _3), &m_Mock1);
    sig.connect(std::bind(&ReceiverMock<const int&>::onItem3, &m_Mock2, _1, _2, _3), &m_Mock2);

    EXPECT_CALL(m_Mock1, onItem3(1, 5, 9)).Times(1);
    EXPECT_CALL(m_Mock2, onItem3(1, 5, 9)).Times(1);

    sig(1, 5, 9);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);

    EXPECT_CALL(m_Mock1, onItem3(_, _, _)).Times(0);
    EXPECT_CALL(m_Mock2, onItem3(2, 7, 0)).Times(1);
    
    sig.disconnect(&m_Mock1);
    sig(2, 7, 0);
    Mock::VerifyAndClearExpectations(&m_Mock1);
    Mock::VerifyAndClearExpectations(&m_Mock2);
    
    EXPECT_CALL(m_Mock1, onItem3(_, _, _)).Times(0);
    EXPECT_CALL(m_Mock2, onItem3(_, _, _)).Times(0);

    sig.disconnect(&m_Mock2);

    sig(2, 7, 0);
}

TEST_F(SignalTest, PointerArgument)
{
    int integer = 0;

    ReceiverMock<int*>  mock;

    Signal<void (int*)> sig;
    EXPECT_CALL(mock, onItem1(&integer)).Times(1);

    sig.connect(std::bind(&ReceiverMock<int*>::onItem1, &mock, _1), &mock);
    sig(&integer);
}
