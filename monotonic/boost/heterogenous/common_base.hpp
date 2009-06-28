// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_COMMON_BASE_HPP
#define BOOST_HETEROGENOUS_COMMON_BASE_HPP

#include <boost/heterogenous/detail/prefix.hpp>
#include <boost/heterogenous/abstract_allocator.hpp>

namespace boost
{
	namespace heterogenous
	{
		/// common base for all base types for hierachies
		struct common_base
		{
			virtual ~common_base() { }

			virtual common_base *allocate(abstract_allocator &alloc) const = 0;
			virtual void deallocate(common_base *, abstract_allocator &alloc) const = 0;
			virtual common_base *create(abstract_allocator &alloc) const = 0;
			virtual common_base *copy_construct(const common_base &original, abstract_allocator &alloc) const = 0;
		};

	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_COMMON_BASE_HPP

//EOF
