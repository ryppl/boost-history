// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/monotonic/storage_base.h>
#include <boost/monotonic/storage.h>
#include <boost/assert.hpp>
//#include <boost/swap.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>

namespace boost
{
	namespace monotonic
	{
		/// forward declaration
		template <class> 
		class allocator;

		/// specialization for void
		template <> 
		class allocator<void>
		{
		public:
			typedef void* pointer;
			typedef const void* const_pointer;

			typedef void value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef allocator<U> other; 
			};
		};

		/// a monotonic allocator always increases in size
		template <class T> 
		class allocator 
		{
		public:
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef T *pointer;
			typedef const T *const_pointer;
			typedef T &reference;
			typedef const T &const_reference;
			typedef T value_type;

			/// rebind storage to a another type
			template <class U> 
			struct rebind 
			{ 
				typedef allocator<U> other; 
			};

			/// the storage used by the allocator
			storage_base *storage;

		public:
			allocator() throw() 
				: storage(0)
			{
			}

			allocator(storage_base &P) throw() 
				: storage(&P)
			{
			}

			allocator(const allocator& Q) throw() 
				: storage(Q.storage)
			{
			}

			template <class U> 
			allocator(const allocator<U> &Q) throw()
			{
				typedef BOOST_DEDUCED_TYPENAME allocator<T>::template rebind<U> Other;
				typedef BOOST_DEDUCED_TYPENAME Other::other OtherStorage;
				storage = OtherStorage(*Q.storage).storage;
			}

			~allocator() throw()
			{
			}

			pointer address(reference x) const
			{
				return &x;
			}

			const_pointer address(const_reference x) const
			{
				return &x;
			}

			BOOST_STATIC_CONSTANT(size_t, alignment = boost::aligned_storage<sizeof(T)>::alignment);

			pointer allocate(size_type num, allocator<void>::const_pointer /*hint*/ = 0)
			{
				BOOST_ASSERT(num > 0);
				BOOST_ASSERT(storage != 0);
				return static_cast<T *>(storage->allocate(num*sizeof(T), alignment));
			}

			void deallocate(pointer p, size_type n)
			{
				// do nothing
			}

			size_type max_size() const throw()
			{
				BOOST_ASSERT(storage != 0);
				return storage->max_size();
			}

			void construct(pointer p)
			{
				new (p) T();
			}

			void construct(pointer p, const T& val)
			{
				new (p) T(val);
			}

			void destroy(pointer p)
			{
				if (!p)
					return;
				destroy(p, boost::has_trivial_destructor<T>());
			}

			void destroy(pointer p, const boost::false_type& )
			{
				(*p).~T();
			}

			void destroy(pointer, const boost::true_type& )
			{ 
			}

			void swap(allocator<T> &other)
			{
				std::swap(storage, other.storage);
			}

			friend bool operator==(allocator<T> const &A, allocator<T> const &B) { return A.storage == B.storage; }
			friend bool operator!=(allocator<T> const &A, allocator<T> const &B) { return A.storage != B.storage; }
		};
	}
}

//EOF
