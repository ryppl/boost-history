#ifndef BOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED
#define BOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED

// Implementation of shared_ptr as wrapper around generic_ptr::shared.
// Written primarily to help test generic_ptr::shared using unmodified
// shared_ptr tests.

//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/generic_ptr/shared.hpp>

namespace boost
{
	template<typename T> class enable_shared_from_this;
	template<typename T> class shared_ptr;
	template<typename T> class weak_ptr;

	namespace detail
	{
		// enable_shared_from_this support

		template< class X, class Y, class T > inline void sp_enable_shared_from_this( boost::shared_ptr<X> const * ppx, Y const * py, boost::enable_shared_from_this< T > const * pe )
		{
			generic_ptr::detail::sp_enable_shared_from_this(static_cast<generic_ptr::shared<X*> const *>(ppx), &py, pe);
		}

		#ifdef _MANAGED

		// Avoid C4793, ... causes native code generation

		struct sp_any_pointer
		{
			template<class T> sp_any_pointer( T* ) {}
		};

		inline void sp_enable_shared_from_this( sp_any_pointer, sp_any_pointer, sp_any_pointer )
		{
		}

		#else // _MANAGED

		inline void sp_enable_shared_from_this( ... )
		{
		}

		#endif // _MANAGED
	}

	template<typename T>
	class shared_ptr: public generic_ptr::shared<T*>
	{
		typedef generic_ptr::shared<T*> base_type;
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
#if !defined( BOOST_NO_SFINAE )
		shared_ptr( shared_ptr<Y> const & r, typename enable_if<is_convertible<Y*, T*> >::type * = 0 )
#else
		shared_ptr( shared_ptr<Y> const & r )
#endif
		: base_type(static_cast<generic_ptr::shared<Y*> const &>(r))
		{}
		template<typename Y>
		shared_ptr(shared_ptr<Y> const & r, T *p): base_type(static_cast<const generic_ptr::shared<Y*> &>(r), p)
		{}
		template<typename Y>
		explicit shared_ptr(std::auto_ptr<Y> & r): base_type(r)
		{}
#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )
		template<class Ap>
		explicit shared_ptr( Ap r, typename generic_ptr::detail::sp_enable_if_auto_ptr<Ap, int>::type = 0 ):
			base_type(r)
		{}
#endif // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
		template<class Y>
		explicit shared_ptr(weak_ptr<Y> const & r): base_type(static_cast<generic_ptr::weak<Y*> const &>(r))
		{}
#ifndef BOOST_NO_RVALUE_REFERENCES
    shared_ptr( shared_ptr && r ): base_type(std::move<base_type>(r))
    {}
    template<class Y>
#if !defined( BOOST_NO_SFINAE )
    shared_ptr( shared_ptr<Y> && r, typename enable_if<is_convertible<Y*, T*> >::type * = 0 )
#else
    shared_ptr( shared_ptr<Y> && r )
#endif
    : base_type(std::move<generic_ptr::shared<Y*> >(r))
    {}
    shared_ptr & operator=( shared_ptr && r )
    {
        return static_cast<shared_ptr&>(base_type::operator=(std::move<base_type>(r)));
    }
    template<class Y>
    shared_ptr & operator=( shared_ptr<Y> && r )
    {
        return static_cast<shared_ptr&>(base_type::operator=(std::move<generic_ptr::shared<Y*> >(r)));
    }
#endif // BOOST_NO_RVALUE_REFERENCES
		template<typename Y>
		shared_ptr & operator=( std::auto_ptr<Y> & r )
		{
			return static_cast<shared_ptr&>(static_cast<base_type&>(*this) = r);
		}
#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )
		template<typename Ap>
		typename generic_ptr::detail::sp_enable_if_auto_ptr< Ap, shared_ptr & >::type operator=( Ap r )
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
			return static_cast<shared_ptr&>(static_cast<base_type&>(*this) = static_cast<const generic_ptr::shared<Y*> &>(r));
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
			base_type::reset(static_cast<generic_ptr::shared<Y*> const &>(r), p);
		}
	};

	template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U> const & r)
	{
		return generic_ptr::static_pointer_cast<T>(r);
	}

	template<class T, class U> shared_ptr<T> const_pointer_cast(shared_ptr<U> const & r)
	{
		return generic_ptr::const_pointer_cast<T>(r);
	}

	template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const & r)
	{
		return generic_ptr::dynamic_pointer_cast<T>(r);
	}

	template<typename T>
	T* get_pointer(const shared_ptr<T> &sp)
	{
		return get_pointer(static_cast<const generic_ptr::shared<T*> &>(sp));
	}
	template<class D, class T> D * get_deleter(shared_ptr<T> const & p)
	{
		return generic_ptr::get_deleter<D>(p);
	}

#if !defined(BOOST_SP_NO_ATOMIC_ACCESS)
	template<class T> inline bool atomic_is_lock_free( shared_ptr<T> const * p )
	{
		return generic_ptr::atomic_is_lock_free(p);
	}
	template<class T> shared_ptr<T> atomic_load( shared_ptr<T> const * p )
	{
			return generic_ptr::atomic_load(p);
	}
	template<class T> inline shared_ptr<T> atomic_load_explicit( shared_ptr<T> const * p, memory_order mo )
	{
			return generic_ptr::atomic_load_explicit( p, mo );
	}
	template<class T> void atomic_store( shared_ptr<T> * p, shared_ptr<T> r )
	{
		generic_ptr::atomic_store(p, r);
	}
	template<class T> inline void atomic_store_explicit( shared_ptr<T> * p, shared_ptr<T> r, memory_order mo )
	{
		generic_ptr::atomic_store_explicit(p, r, mo);
	}
	template<class T> shared_ptr<T> atomic_exchange( shared_ptr<T> * p, shared_ptr<T> r )
	{
		return generic_ptr::atomic_exchange(p, r);
	}

	template<class T> shared_ptr<T> atomic_exchange_explicit( shared_ptr<T> * p, shared_ptr<T> r, memory_order mo )
	{
		return generic_ptr::atomic_exchange_explicit( p, r, mo );
	}

	template<class T> bool atomic_compare_exchange( shared_ptr<T> * p, shared_ptr<T> * v, shared_ptr<T> w )
	{
		return generic_ptr::atomic_compare_exchange(p, v, w);
	}

	template<class T> inline bool atomic_compare_exchange_explicit( shared_ptr<T> * p, shared_ptr<T> * v, shared_ptr<T> w, memory_order success, memory_order failure )
	{
			return generic_ptr::atomic_compare_exchange_explicit( p, v, w, success, failure );
	}

#endif

} // namespace boost

#endif  // #ifndef BOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED
