// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_ABSTRACT_ALLOCATOR_HPP
#define BOOST_CLONEABLE_ABSTRACT_ALLOCATOR_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/forward_declarations.hpp>
//#include <boost/cloneable/allocation.hpp>

namespace boost
{
	namespace cloneable
	{
		/// base class for (wrapped) allocators to be used with cloneable::cloneable<>
		/// by cloneable containers
		struct abstract_allocator
		{
			typedef char *pointer;

			virtual pointer allocate_bytes(size_t num_bytes, size_t alignment) = 0;

			virtual void deallocate_bytes(pointer, size_t alignment) = 0;

			static size_t calc_padding(pointer ptr, size_t alignment)
			{
				ptrdiff_t index = ptr - pointer(0);
				size_t extra = index & (alignment - 1);	// assumes 2^n alignment!
				if (extra > 0)
					extra = alignment - extra;
				return extra;
			}

			// TODO
			//template <class T>
			//T *create()
			//{
			//	return cloneable::create<T>(*this);
			//}
			//template <class T, class A0>
			//T *create(A0 a0)
			//{
			//	return cloneable::create<T>(*this, a0);
			//}
			//template <class T, class A0, class A1>
			//T *create(A0 a0, A1 a1)
			//{
			//	return cloneable::create<T>(*this, a0, a1);
			//}
		};

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_ABSTRACT_ALLOCATOR_HPP

//EOF
