// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_SHARED_ALLOCATOR_H
#define BOOST_MONOTONIC_SHARED_ALLOCATOR_H

#include <boost/monotonic/allocator.hpp>

namespace boost
{
	namespace monotonic
	{
		template <class>
		struct shared_allocator;

		template <> 
		struct shared_allocator<void> : allocator<void>
		{
		};

		template <class T> 
		struct shared_allocator : allocator<T>
		{
			typedef allocator<T> Parent;

			shared_allocator() throw() 
				: Parent(&static_shared_storage)
			{
			}

			shared_allocator(shared_storage_base &store) throw() 
				: Parent(&store)
			{
			}

			shared_allocator(const shared_allocator& alloc) throw() 
				: Parent(alloc.get_storage())
			{
			}

			template <class U> 
			shared_allocator(const shared_allocator<U> &alloc) throw()
				: Parent(alloc.get_storage()) 
			{
			}

			shared_storage_base *get_storage() const
			{
				return static_cast<shared_storage_base *>(alloc.get_storage());
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_ALLOCATOR_H

//EOF
