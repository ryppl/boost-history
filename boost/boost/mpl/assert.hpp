
#ifndef BOOST_MPL_ASSERT_HPP_INCLUDED
#define BOOST_MPL_ASSERT_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/test/assert.hpp>

#define BOOST_MPL_ASSERT(pred)                      MPL_ASSERT(pred)
#define BOOST_MPL_ASSERT_NOT(pred)                  MPL_ASSERT_NOT(pred)
#define BOOST_MPL_ASSERT_MSG(c, msg, types)         MPL_ASSERT_MSG(c, msg, types)
#define BOOST_MPL_ASSERT_RELATION(x, rel, y)        MPL_ASSERT_RELATION(x, rel, y)

#endif // BOOST_MPL_ASSERT_HPP_INCLUDED
