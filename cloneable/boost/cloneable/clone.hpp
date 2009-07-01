// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_CLONE_HPP
#define BOOST_CLONEABLE_CLONE_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/traits.hpp>

namespace boost
{
	namespace cloneable
	{
		namespace impl
		{
			template <bool>
			struct clone
			{
				template <class T>
				static T *given(const T &original)
				{
					return new T(original); 
				}
				template <class T, class Alloc>
				static T *given(const T &original, Alloc &alloc)
				{
					return create<T>(alloc, original);
				}
			};
			template <>
			struct clone<true>
			{
				template <class T>
				static T *given(const T &original)
				{
					return original.clone_as<T>();
				}
				template <class T, class Alloc>
				static T *given(const T &original, Alloc &alloc)
				{
					return original.clone_as<T>(alloc);
				}
			};
		}

		template <class T>
		T *clone(const T &original)
		{
			return impl::clone<is_cloneable<T>::value>::given(original);
		}

		template <class T, class Alloc>
		T *clone(const T &original, Alloc &alloc)
		{
			return impl::clone<is_cloneable<T>::value>::given(original, alloc);
		}

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_CLONE_HPP

//EOF
