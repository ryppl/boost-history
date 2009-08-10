//
// synch_policy.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SYNCH_POLICY_HPP
#define SYNCH_POLICY_HPP

#include <boost/channel/platforms/boost_platform.hpp>

namespace boost {
  namespace channel {

    template<typename platform_t>
    class null_synch {
    public:
      typedef platform_t platform;
      typedef	typename platform_t::null_mutex mutex;
      typedef typename platform_t::null_condition condition;
      typedef typename mutex::scoped_lock scoped_lock;
    };

    template <typename platform_t>
    class mt_synch {
    public:
      typedef platform_t platform;
      typedef typename platform_t::mutex mutex;
      typedef typename platform_t::condition condition;
      typedef typename mutex::scoped_lock scoped_lock;
    };

  }
}

#endif

