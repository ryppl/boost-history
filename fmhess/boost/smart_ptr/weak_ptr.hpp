#ifndef BOOST_SMART_PTR_WEAK_PTR_HPP_INCLUDED
#define BOOST_SMART_PTR_WEAK_PTR_HPP_INCLUDED

// Implementation of weak_ptr as wrapper around generic_weak.
// Written primarily to help test generic_shared/generic_weak using unmodified
// shared_ptr tests.

//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/smart_ptr/generic_weak.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace boost
{
	template<typename T>
	class weak_ptr: public generic_weak<T*>
	{
		typedef generic_weak<T*> base_type;
	public:
		weak_ptr() {}
		weak_ptr(base_type const & b): base_type(b) {}
		template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )
		weak_ptr( weak_ptr<Y> const & r, typename detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )
#else
		weak_ptr( weak_ptr<Y> const & r )
#endif
		:base_type(static_cast<generic_weak<Y*> const &>(r))
		{}
    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )
		weak_ptr( shared_ptr<Y> const & r, typename detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )
#else
		weak_ptr( shared_ptr<Y> const & r )
#endif
		:base_type(static_cast<generic_shared<Y*> const &>(r))
		{}
		shared_ptr<T> lock() const
		{
			return base_type::lock();
		}
	};
}

#endif  // #ifndef BOOST_SMART_PTR_WEAK_PTR_HPP_INCLUDED
