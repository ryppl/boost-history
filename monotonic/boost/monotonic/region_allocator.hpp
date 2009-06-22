// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_REGION_ALLOCATOR_HPP
#define BOOST_MONOTONIC_REGION_ALLOCATOR_HPP

#include <boost/monotonic/allocator_base.hpp>

namespace boost
{
	namespace monotonic
	{
		template <> 
		struct region_allocator<void>
		{
			typedef void* pointer;
			typedef const void* const_pointer;

			typedef void value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef region_allocator<U> other; 
			};
		};

		storage_base &get_storage();
		storage_base *set_storage(storage_base &);

		template <size_t Region, class T> 
		struct region_allocator : allocator_base<T, region_allocator<T> >
		{
		};

	} // namespace monotonic

} // namespace boost

#endif // #define BOOST_MONOTONIC_REGION_ALLOCATOR_HPP

//EOF
