// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_ALLOCATOR_HPP
#define BOOST_MONOTONIC_ALLOCATOR_HPP

#include <boost/monotonic/allocator_base.hpp>

namespace boost
{
	namespace monotonic
	{
		template <class Region> 
		struct allocator<void, Region>
		{
			typedef void* pointer;
			typedef const void* const_pointer;

			typedef void value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef allocator<U, Region> other; 
			};
		};

		template <class T, class Region> 
		struct allocator : allocator_base<T, allocator<T, Region> >
		{
			typedef allocator_base<T, allocator<T, Region> > Parent;
			using typename Parent::size_type;
			using typename Parent::difference_type;
			using typename Parent::pointer;
			using typename Parent::const_pointer;
			using typename Parent::reference;
			using typename Parent::const_reference;
			using typename Parent::value_type;

			template <class U> 
			struct rebind 
			{ 
				typedef allocator<U, Region> other; 
			};

			allocator() throw() 
				: Parent(boost::monotonic::get_storage()) { }

		public:
		//private:
			template <class Storage> struct local;

			allocator(storage_base &store) throw() 
				: Parent(store) { }

		public:
			allocator(const allocator& alloc) throw() 
				: Parent(alloc) { }

			template <class U, class OtherRegion> 
			allocator(const allocator<U, OtherRegion> &alloc) throw()
				: Parent(alloc) { }

			friend bool operator==(allocator<T,Region> const &A, allocator<T,Region> const &B) 
			{ 
				return static_cast<Parent const &>(A) == static_cast<Parent const &>(B);
			}

			friend bool operator!=(allocator<T,Region> const &A, allocator<T,Region> const &B) 
			{ 
				return static_cast<Parent const &>(A) != static_cast<Parent const &>(B);
			}
		};
	
	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_ALLOCATOR_HPP

//EOF
