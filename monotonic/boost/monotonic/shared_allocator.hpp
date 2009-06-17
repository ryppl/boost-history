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
			shared_allocator() throw() 
				: storage(&static_shared_storage)
			{
			}

			shared_allocator(shared_storage_base &store) throw() 
				: storage(&store)
			{
			}

			shared_allocator(const shared_allocator& alloc) throw() 
				: storage(alloc.get_storage())
			{
			}

			template <class U> 
			shared_allocator(const shared_allocator<U> &alloc) throw()
				: storage(alloc.get_storage()) 
			{
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_ALLOCATOR_H

//EOF
