// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_ABSTRACT_ALLOCATOR_HPP
#define BOOST_CLONEABLE_ABSTRACT_ALLOCATOR_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/forward_declarations.hpp>

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
		};

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_ABSTRACT_ALLOCATOR_HPP

//EOF
