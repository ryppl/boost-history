//
// boost/join/executor.hpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_ASIO_EXECUTOR_HPP
#define BOOST_JOIN_ASIO_EXECUTOR_HPP

#include <boost/asio.hpp>
#include <boost/join/join.hpp>

namespace boost {
  namespace join {

    //integration with Boost.Asio
    //submit async tasks to asio's completion_event queue to be executed by main thread
    class asio_executor {
    public:
      boost::asio::io_service& io_service_;
      asio_executor(boost::asio::io_service& io_service): io_service_(io_service) {}
      template <typename task_type>
      void operator()(task_type task) {
        io_service_.post(task); 
      }
    };

  }
}

#endif
