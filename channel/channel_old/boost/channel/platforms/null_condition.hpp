//
// null_condition.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_CHANNEL_NULL_CONDITION_HPP
#define BOOST_CHANNEL_NULL_CONDITION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)
#include <boost/thread.hpp>
#include <boost/utility.hpp>

namespace boost {
  namespace channel {
    namespace detail {

      class null_condition: private boost::noncopyable {
      public:
	null_condition() {
	}
	~null_condition() {
	}

	void notify_one() {
	}
	void notify_all() {
	}

	template<typename L>
	void wait(L& lock) {
	}

	template<typename L, typename Pr>
	void wait(L& lock, Pr pred) {
	}

	template<typename L>
	bool timed_wait(L& lock, const xtime& xt) {
          return false;
	}

	template<typename L, typename Pr>
	bool timed_wait(L& lock, const xtime& xt, Pr pred) {
          return false;
	}

      };

    } // namespace detail
  } // namespace channel
} // namespace boost

#endif
