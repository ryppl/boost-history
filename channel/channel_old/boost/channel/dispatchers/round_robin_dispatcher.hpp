//
// round_robin_dispatcher.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


//// pull dispatchers:
//// define the whole message passing process:
//// including:
////   . sending, buffering, recving
////   . provide both sender/recver sides

#ifndef ROUND_ROBIN_DISPATCHER_HPP
#define ROUND_ROBIN_DISPATCHER_HPP

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

      //we need random-access iterator for roundrobin?
      //here a temp fix: convert set<name*> to vector<name*>
      //for a name_space targeted for round_robin dispatching app,
      //we should change name_space container type to vector instead of set
      //or make name_space container type configurable?
      template<typename name_space, typename platform>
      class round_robin_algo {
      public:
	typedef	typename name_space::id_type id_type;
	typedef message<id_type> msg_type;
	typedef push_recver_base<name_space,platform> recver_type;
	typedef named_in<name_space, recver_type> named_in_type;

	round_robin_algo(): last_(-1) {}
	template<typename bindings_type>
	void operator() (bindings_type &bindings,
                         boost::shared_ptr<msg_type> msg) {
          if (!bindings.empty()) {
            std::vector<typename bindings_type::value_type> v(bindings.begin(), bindings.end());
            int sz = v.size();
            last_ = (last_+1) % sz;
            named_in_type *named_in = (named_in_type *)(v[last_]);
            recver_type *recver = (recver_type *)named_in;
            recver->push(msg);
          }
	}
      private:
	int last_;
      };

    }

    template <typename name_space, typename platform>
    struct round_robin_dispatcher {
      typedef detail::push_sender_base<name_space,platform,detail::round_robin_algo> sender;
      typedef detail::push_recver_base<name_space,platform> recver;
    };

  }
}

#endif
