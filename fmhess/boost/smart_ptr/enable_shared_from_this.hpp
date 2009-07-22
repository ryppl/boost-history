#ifndef BOOST_SMART_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
#define BOOST_SMART_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED

// Implementation of enable_shared_from_this as wrapper around
// enable_generic_shared_from_this.
// Written primarily to help test generic_shared using unmodified
// shared_ptr tests.

//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/generic_ptr/enable_shared_from_this.hpp>

namespace boost
{
	template<typename T>
	class enable_shared_from_this: public generic_ptr::enable_shared_from_this<T*>
	{
		typedef generic_ptr::enable_shared_from_this<T*> base_type;
		public:
		boost::shared_ptr<T> shared_from_this()
		{
			return base_type::shared_from_this();
		}
		boost::shared_ptr<const T> shared_from_this() const
		{
			return base_type::shared_from_this();
		}
	};
}

#endif  // #ifndef BOOST_SMART_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
