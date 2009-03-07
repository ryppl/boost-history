//
// name.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_CHANNEL_SOURCE

#include <boost/channel/name.hpp>

short
boost::channel::name_base::scope_checking_tbl_[][name_base::scope_number * name_base::member_number] = {
  {1, 0, 1, 0, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {1, 0, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};
