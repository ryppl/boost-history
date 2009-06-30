// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_ALLOCATOR_HPP
#define BOOST_HETEROGENOUS_ALLOCATOR_HPP

#include <boost/heterogenous/detail/prefix.hpp>

namespace boost
{
	namespace heterogenous
	{
		/// a cloning allocator
		struct allocator
		{
			template <class Base>
			static Base* allocate_clone( const Base& object )
			{
				throw;
			}

			template <class Base>
			static void deallocate_clone( const Base* clone )
			{
				throw;
			}

			template <class Base, class Alloc>
			static Base* allocate_clone(const Base& object, Alloc &alloc )
			{
				// calling copy_construct must be disabled at compile-time for types that are not boost::is_convertible<cloneable::base<U> *, U*>
				return object.copy_construct(object, alloc);
			}

			template <class Base, class Alloc>
			static void deallocate_clone(const Base *object, Alloc &alloc )
			{
				if (!object)
					return;
				object->deallocate(const_cast<Base &>(*object), alloc);
			}
		};

	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_ALLOCATOR_HPP

//EOF
