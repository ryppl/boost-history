//
// null_mutex.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_CHANNEL_NULL_MUTEX_HPP
#define BOOST_CHANNEL_NULL_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/utility.hpp>

namespace boost {
  namespace channel {
    namespace detail {

      template <typename Mutex>
      class null_scoped_lock : private noncopyable
      {
      public:
	typedef Mutex mutex_type;

	explicit null_scoped_lock(Mutex& mx, bool initially_locked=true)
	{
	}
	~null_scoped_lock()
	{
	}

	void lock()
	{
	}
	void unlock()
	{
	}

	bool locked() const { return false; }
	operator const void*() const { return 0; }

      private:
      };

      class null_mutex
	: private boost::noncopyable
      {
      public:
	typedef null_scoped_lock<null_mutex> scoped_lock;

	// Constructor.
	null_mutex()
	{
	}

	// Destructor.
	~null_mutex()
	{
	}

      };

    } // namespace detail
  } // namespace channel
} // namespace boost

#endif
