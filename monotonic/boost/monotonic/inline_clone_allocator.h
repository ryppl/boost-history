// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace boost
{
	namespace monotonic
	{
		/// custom clone allocator for ptr-containers using a monotonic allocator.
		/// see http://www.boost.org/doc/libs/1_38_0/libs/ptr_container/doc/reference.html for details.
		struct inline_clone_allocator
		{
			template< class U >
			static U* allocate_clone( const U& r )
			{
				// can't allocate clone without access to the monotonic allocator.
				// this is a design fault in boost::ptr_container.
				return 0;
			}

			template< class U >
			static void deallocate_clone( const U* clone )
			{
				if (clone)
					clone->U::~U();
			}
		};

	}
}

//EOF
