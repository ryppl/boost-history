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
		template <size_t N> 
		struct region_allocator<void, N>
		{
			typedef void* pointer;
			typedef const void* const_pointer;

			typedef void value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef region_allocator<U, N> other; 
			};
		};

		storage_base &get_storage();
		storage_base *set_storage(storage_base &);

		/// each region is distinct from other regions
		template <class T, size_t Region> 
		struct region_allocator : allocator_base<T, region_allocator<T, Region> >
		{
			typedef allocator_base<T, region_allocator<T, Region> > Parent;
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
				typedef region_allocator<U, Region> other; 
			};

			region_allocator() throw() 
				: Parent(boost::monotonic::get_region_storage<Region>()) { }

		public:
			//private:
			template <class Storage> struct local;

			region_allocator(storage_base &store) throw() 
				: Parent(store) { }

		public:
			region_allocator(const region_allocator& alloc) throw() 
				: Parent(alloc) { }

			template <class U, size_t N> 
			region_allocator(const region_allocator<U,N> &alloc) throw()
				: Parent(alloc) { }

			friend bool operator==(region_allocator<T,Region> const &A, region_allocator<T,Region> const &B) 
			{ 
				return static_cast<Parent const &>(A) == static_cast<Parent const &>(B);
			}

			friend bool operator!=(region_allocator<T,Region> const &A, region_allocator<T,Region> const &B) 
			{ 
				return static_cast<Parent const &>(A) != static_cast<Parent const &>(B);
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // #define BOOST_MONOTONIC_REGION_ALLOCATOR_HPP

//EOF
