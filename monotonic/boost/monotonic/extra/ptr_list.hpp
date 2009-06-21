// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_PTR_LIST_H
#define BOOST_MONOTONIC_PTR_LIST_H

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/monotonic/allocator.hpp>
#include <boost/monotonic/inline_clone_allocator.hpp>

namespace boost
{
	namespace monotonic
	{
		/// A boost::ptr_list<T> that uses a monotonic allocator, and a custom clone allocator
		template <class T>
		struct ptr_list : boost::ptr_list<T, inline_clone_allocator, allocator<T> >
		{
			typedef allocator<T> Allocator;
			typedef boost::ptr_list<T, inline_clone_allocator, Allocator> List;

			ptr_list() 
			{ 
			}
			ptr_list(storage_base &storage) // Copyright (C) 2009 Christian Schladetsch
				//
				//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
				//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


				: List(Allocator(storage)) 
			{ 
			}
			ptr_list(Allocator const &A) 
				: List(A) 
			{ 
			}	
		};

	}
}

#endif // BOOST_MONOTONIC_PTR_LIST_H

//EOF
