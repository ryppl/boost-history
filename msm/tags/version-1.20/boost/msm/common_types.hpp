// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MSM_COMMON_TYPES_H
#define BOOST_MSM_COMMON_TYPES_H

#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>

namespace boost { namespace msm
{
// used for disable_if
template <int> struct dummy { dummy(int) {} };
// return value for transition handling
typedef enum
{
    HANDLED_FALSE=0,
    HANDLED_TRUE =1,
    HANDLED_GUARD_REJECT=2
} HandledEnum;
typedef ::boost::tuple<HandledEnum,const std::vector<int>*> execute_return;

// wrapper for mpl::for_each as showed in the C++ Template Metaprogramming ch. 9
template <class T>
struct wrap{};

}}

#endif //BOOST_MSM_COMMON_TYPES_H

