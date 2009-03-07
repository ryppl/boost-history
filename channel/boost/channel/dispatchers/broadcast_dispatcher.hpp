//
// broadcast_dispatcher.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


//// broadcast dispatcher

#ifndef BROADCAST_DISPATCHER_HPP
#define BROADCAST_DISPATCHER_HPP

#include <map>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/channel/platforms/synch_policy.hpp>
#include <boost/channel/message.hpp>
#include <boost/channel/name.hpp>
#include <boost/channel/named_in_out.hpp>
#include <boost/channel/dispatchers/dispatcher_base.hpp>
#include <boost/channel/dispatchers/push_dispatcher_base.hpp>

namespace boost {
  namespace channel {

    namespace detail {

      //the following is for 3 simple "push" dispatchers: broadcast, roundrobin, union
      template<typename name_space, typename platform>
      struct broadcast_algo {
	typedef	typename name_space::id_type id_type;
	typedef message<id_type> msg_type;
	typedef push_recver_base<name_space,platform> recver_type;
	typedef named_in<name_space, recver_type> named_in_type;

	template<typename bindings_type>
	void operator() (bindings_type &bindings,
                         boost::shared_ptr<msg_type> msg) {
          if (!bindings.empty()) {
            for(typename bindings_type::iterator iter = bindings.begin();
                iter != bindings.end(); iter++) {
              named_in_type *named_in = (named_in_type *)(*iter);
              recver_type *recver = (recver_type *)named_in;
              recver->push(msg);
            }
          }
	}
      };

    }

    template <typename name_space, typename platform>
    struct broadcast_dispatcher {
      typedef detail::push_sender_base<name_space,platform,detail::broadcast_algo> sender;
      typedef detail::push_recver_base<name_space,platform> recver;
    };

  }
}

#endif
