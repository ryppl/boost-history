//
// in_place_executor.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef IN_PLACE_EXECUTOR_HPP
#define IN_PLACE_EXECUTOR_HPP

#include <boost/channel/executors/executor_base.hpp>

namespace boost {
  namespace channel {

    //the simplest: execute async_task in place: ie. in current thread & calling context
    class in_place_executor {
    public:
      template<typename task_type>
      void execute(task_type task) {
        task();
      }
    };

  }
}

#endif

