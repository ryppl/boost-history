//
//  generic_ptr/pointer_traits.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_POINTER_TRAITS_HPP_INCLUDED
#define BOOST_GENERIC_PTR_POINTER_TRAITS_HPP_INCLUDED

#include <boost/get_pointer.hpp>
#include <boost/mpl/identity.hpp>

namespace boost
{
	namespace generic_ptr
	{
		template<typename T> struct pointer_traits
		{
			typedef typename T::value_type value_type;
			typedef typename T::pointer pointer;
			typedef typename T::reference reference;
		};

		template<typename T> struct pointer_traits<T*>
		{
			typedef T value_type;
			typedef T * pointer;
			typedef T & reference;
		};

		template<> struct pointer_traits<void*>
		{
			typedef void value_type;
			typedef void * pointer;
			typedef void reference;
		};

		template<> struct pointer_traits<const void*>
		{
			typedef void value_type;
			typedef const void * pointer;
			typedef void reference;
		};

		template<> struct pointer_traits<volatile void*>
		{
			typedef void value_type;
			typedef volatile void * pointer;
			typedef void reference;
		};

		template<> struct pointer_traits<const volatile void*>
		{
			typedef void value_type;
			typedef const volatile void * pointer;
			typedef void reference;
		};

		template<typename T>
		T * get_plain_old_pointer(T * p)
		{
			return p;
		}

		template<typename GenericPtr>
		typename pointer_traits<GenericPtr>::value_type *
				get_plain_old_pointer(GenericPtr gp)
		{
			using boost::get_pointer;
			return get_plain_old_pointer(get_pointer(gp));
		}

		// two-argument cast overloads for raw pointers (really belongs in boost/pointer_cast.hpp)
		template<typename T, typename U>
		T* static_pointer_cast(U *r, boost::mpl::identity<T>)
		{
			return static_cast<T*>(r);
		}

		template<typename T, typename U>
		T* const_pointer_cast(U *r, boost::mpl::identity<T>)
		{
			return const_cast<T*>(r);
		}

		template<typename T, typename U>
		T* dynamic_pointer_cast(U *r, boost::mpl::identity<T>)
		{
			return dynamic_cast<T*>(r);
		}

		template<typename GenericPtr, typename ValueType>
		struct rebind
		{
			typedef typename GenericPtr::template rebind<ValueType>::other other;
		};

		template<typename T, typename ValueType>
		struct rebind<T*, ValueType>
		{
			typedef ValueType * other;
		};

	} // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_POINTER_TRAITS_HPP_INCLUDED
