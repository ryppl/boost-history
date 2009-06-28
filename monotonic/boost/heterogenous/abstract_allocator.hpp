// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_ABSTRACT_ALLOCATOR_HPP
#define BOOST_HETEROGENOUS_ABSTRACT_ALLOCATOR_HPP

#include <boost/heterogenous/detail/prefix.hpp>

namespace boost
{
	namespace heterogenous
	{
		/// base class for (wrapped) allocators that may be used with heterogenous::common_base
		struct abstract_allocator
		{
			typedef char *pointer;

			virtual pointer allocate_bytes(size_t num_bytes, size_t alignment) = 0;

			virtual void deallocate_bytes(pointer) = 0;
		};

	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_ABSTRACT_ALLOCATOR_HPP

//EOF
