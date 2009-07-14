#ifndef BOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED
#define BOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED

// Implementation of shared_ptr as wrapper around generic_shared.
// Written primarily to help test generic_shared using unmodified
// shared_ptr tests.

//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/smart_ptr/generic_shared.hpp>

namespace boost
{
	namespace detail
	{
		template<typename T, typename U, typename V>
		inline void sp_enable_shared_from_this(T, U, V)
		{
		}
	}

	template<typename T>
	class shared_ptr: public generic_shared<T*>
	{
		typedef generic_shared<T*> base_type;
	public:
		shared_ptr(const base_type &base): base_type(base)
		{}
		shared_ptr() {}
		template<typename U>
		explicit shared_ptr(U *u): base_type(u)
		{}
		template<typename U, typename V>
		shared_ptr(U *u, V v): base_type(u, v)
		{}
		template<typename U, typename V, typename W>
		shared_ptr(U *u, V v, W w): base_type(u, v, w)
		{}
    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )
		shared_ptr( shared_ptr<Y> const & r, typename gs_detail::sp_enable_if_convertible<Y*,T*>::type = detail::sp_empty() )
#else
		shared_ptr( shared_ptr<Y> const & r )
#endif
		: base_type(static_cast<generic_shared<Y*> const &>(r))
		{}
		template<typename Y>
		shared_ptr(shared_ptr<Y> const & r, T *p): base_type(static_cast<const generic_shared<Y*> &>(r), p)
		{}
		template<typename Y>
		explicit shared_ptr(std::auto_ptr<Y> & r): base_type(r)
		{}
		template<typename Ap>
		explicit shared_ptr( Ap r): base_type(r)
		{}
		template<typename Y>
		shared_ptr & operator=( std::auto_ptr<Y> & r )
		{
			return static_cast<shared_ptr&>(static_cast<base_type&>(*this) = r);
		}
#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )
		template<typename Ap>
		typename boost::gs_detail::sp_enable_if_auto_ptr< Ap, shared_ptr & >::type operator=( Ap r )
		{
			return static_cast<shared_ptr&>(static_cast<base_type&>(*this) = r);
		}
#endif // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
		shared_ptr & operator=( shared_ptr const & r )
		{
			return static_cast<shared_ptr&>(static_cast<base_type&>(*this) = r);
		}
#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1400)
		template<class Y>
		shared_ptr & operator=(shared_ptr<Y> const & r) // never throws
		{
			return static_cast<shared_ptr&>(static_cast<base_type&>(*this) = static_cast<const generic_shared<Y*> &>(r));
		}
#endif
		void reset() { base_type::reset(); }
		template<class Y> void reset(Y *p) // Y must be complete
		{
			base_type::reset(p);
		}
		template<class Y, class D> void reset( Y *p, D d )
		{
			base_type::reset(p, d);
		}
		template<class Y, class D, class A> void reset( Y *p, D d, A a )
		{
			base_type::reset(p, d, a);
		}
		template<typename Y> void reset( shared_ptr<Y> const & r, T *p )
		{
			base_type::reset(static_cast<generic_shared<Y*> const &>(r), p);
		}
	};
	template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U> const & r)
	{
		return shared_ptr<T>(static_pointer_cast<generic_shared<U*> const &>(r));
	}

	template<class T, class U> shared_ptr<T> const_pointer_cast(shared_ptr<U> const & r)
	{
		return shared_ptr<T>(const_pointer_cast<generic_shared<U*> const &>(r));
	}

	template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const & r)
	{
		return shared_ptr<T>(dynamic_pointer_cast<generic_shared<U*> const &>(r));
	}
}

#endif  // #ifndef BOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED
