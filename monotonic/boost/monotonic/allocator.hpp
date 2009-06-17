// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_ALLOCATOR_H
#define BOOST_MONOTONIC_ALLOCATOR_H

#include <boost/assert.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>

#include <boost/monotonic/storage.h>
#include <boost/monotonic/static_storage.h>

namespace boost
{
	namespace monotonic
	{
		template <> 
		struct allocator<void>
		{
			typedef void* pointer;
			typedef const void* const_pointer;

			typedef void value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef allocator<U> other; 
			};
		};

		template <class T> 
		struct allocator 
		{
			BOOST_STATIC_CONSTANT(size_t, alignment = boost::aligned_storage<sizeof(T)>::alignment);
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef T *pointer;
			typedef const T *const_pointer;
			typedef T &reference;
			typedef const T &const_reference;
			typedef T value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef allocator<U> other; 
			};

		private:
			storage_base *storage;

		public:
			allocator() throw() 
				: storage(&static_storage)
			{
			}

			allocator(storage_base &store) throw() 
				: storage(&store)
			{
			}

			allocator(const allocator& alloc) throw() 
				: storage(alloc.get_storage())
			{
			}

			template <class U> 
			allocator(const allocator<U> &alloc) throw()
				: storage(alloc.get_storage()) 
			{
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

			pointer allocate(size_type num, allocator<void>::const_pointer /*hint*/ = 0)
			{
				BOOST_ASSERT(num > 0);
				BOOST_ASSERT(storage != 0);
				return static_cast<pointer>(storage->allocate(num*sizeof(value_type), alignment));
			}

			void deallocate(pointer, size_type)
			{
				// do nothing
			}

			size_type max_size() const throw()
			{
				if (!storage)
					return 0;
				//return storage->max_size()/(sizeof(T) + alignment);
				return storage->max_size()/sizeof(value_type);
			}

			void construct(pointer ptr)
			{
				new (ptr) value_type();
			}

			void construct(pointer ptr, const T& val)
			{
				new (ptr) value_type(val);
			}

			void destroy(pointer ptr)
			{
				if (!ptr)
					return;
				destroy(ptr, boost::has_trivial_destructor<value_type>());
			}

			void destroy(pointer ptr, const boost::false_type& )
			{
				(*ptr).~value_type();
			}

			void destroy(pointer, const boost::true_type& )
			{ 
			}

			void swap(allocator<value_type> &other)
			{
				std::swap(storage, other.storage);
			}

			storage_base *get_storage() const
			{
				return storage;
			}

			friend bool operator==(allocator<T> const &A, allocator<T> const &B) 
			{ 
				return A.storage == B.storage; 
			}

			friend bool operator!=(allocator<T> const &A, allocator<T> const &B) 
			{ 
				return A.storage != B.storage; 
			}
		};
	
	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_ALLOCATOR_H

//EOF
