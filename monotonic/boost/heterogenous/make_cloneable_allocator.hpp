// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_MAKE_CLONEABLE_ALLOCATOR_HPP
#define BOOST_HETEROGENOUS_MAKE_CLONEABLE_ALLOCATOR_HPP

#include <boost/type_traits/is_convertible.hpp>
#include <boost/heterogenous/detail/prefix.hpp>
#include <boost/heterogenous/allocator.hpp>

namespace boost
{
	namespace heterogenous
	{
		namespace impl
		{
			template <class Alloc>
			struct cloneable_allocator : Alloc, boost::abstract_allocator
			{
				typedef typename Alloc::template rebind<char>::other CharAlloc;

				abstract_allocator::pointer allocate_bytes(size_t num_bytes, size_t alignment)
				{
					CharAlloc alloc;
					// todo: alignment; this is done already for monotonic, copy that here
					return alloc.allocate(num_bytes);
				}

				void deallocate_bytes(abstract_allocator::pointer ptr)
				{
					CharAlloc alloc;
					alloc.deallocate(ptr, 1);
				}
			};

			template <class Alloc, bool>
			struct make_cloneable_allocator
			{		
				typedef cloneable_allocator<Alloc> type;
			};

			template <class Alloc>
			struct make_cloneable_allocator<Alloc, true>
			{
				typedef Alloc type;
			};
		}

		template <class Alloc>
		struct make_cloneable_allocator
		{
			typedef boost::is_convertible<Alloc *, boost::abstract_allocator *> is_convertible;
			BOOST_STATIC_CONSTANT(bool, is_cloneable = is_convertible::value);
			typedef typename impl::make_cloneable_allocator<Alloc, is_cloneable>::type type;
		};

	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_MAKE_CLONEABLE_ALLOCATOR_HPP

//EOF
