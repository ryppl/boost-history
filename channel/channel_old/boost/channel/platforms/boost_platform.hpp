//
// boost_platform.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_PLATFORM_HPP
#define BOOST_PLATFORM_HPP

#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/channel/platforms/null_mutex.hpp>
#include <boost/channel/platforms/null_condition.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace boost {
  namespace channel {

    class boost_platform {
    public:
      //synch primitive
      typedef detail::null_mutex null_mutex;
      typedef detail::null_condition null_condition;
      //typedef boost::mutex mutex;
      typedef boost::recursive_mutex mutex;
      typedef boost::condition condition;
      //timeout
      typedef boost::posix_time::time_duration timeout_type;
      //log
      static void log(std::string info) {
        std::cout << "log << " << info << " >>" << std::endl;
      }
    };

  }
}

#endif
