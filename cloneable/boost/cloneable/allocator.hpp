// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_ALLOCATOR_HPP
#define BOOST_CLONEABLE_ALLOCATOR_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/detail/allocation.hpp>

namespace boost
{
	namespace cloneable
	{
		/// a cloning allocator
		struct allocator
		{
			template <class Base>
			static Base* allocate_clone( const Base& object )
			{
				// use default allocator
				return object.clone();
			}

			template <class Base>
			static void deallocate_clone( const Base* clone )
			{
				if (!object)
					return;
				// use default allocator
				return const_cast<Base *>(object->deallocate());
			}

			template <class Base, class Alloc>
			static Base* allocate_clone(const Base& object, Alloc &alloc )
			{
				return object.clone(alloc);
			}

			template <class Base, class Alloc>
			static void deallocate_clone(const Base *object, Alloc &alloc )
			{
				if (!object)
					return;
				const_cast<Base &>(*object).deallocate(alloc);
			}
		};

		template <class T, class Alloc>
		T *create(Alloc &alloc)
		{
			typename Alloc::template rebind<T>::other al(alloc);
			T *ptr = al.allocate(1);
			al.construct(ptr);
			return ptr;
		}

		template <class T>
		T *create(abstract_allocator &alloc)
		{
			abstract_allocator::pointer ptr = alloc.allocate_bytes(sizeof(T), aligned_storage<sizeof(T)>::alignment);
			new (ptr) T();
			return reinterpret_cast<T *>(ptr);
		}
		template <class T, class A0>
		T *create(abstract_allocator &alloc, A0 a0)
		{
			abstract_allocator::pointer ptr = alloc.allocate_bytes(sizeof(T), aligned_storage<sizeof(T)>::alignment);
			new (ptr) T(a0);
			return reinterpret_cast<T *>(ptr);
		}
		template <class T, class A0, class A1>
		T *create(abstract_allocator &alloc, A0 a0,  A1 a1)
		{
			abstract_allocator::pointer ptr = alloc.allocate_bytes(sizeof(T), aligned_storage<sizeof(T)>::alignment);
			new (ptr) T(a0, a1);
			return reinterpret_cast<T *>(ptr);
		}

		template <class T, class Alloc, class A0>
		T *create(Alloc &alloc, A0 a0)
		{
			typename Alloc::template rebind<T>::other al(alloc);
			T *ptr = al.allocate(1);
			new (ptr) T(a0);
			return ptr;
		}
		template <class T, class Alloc, class A0, class A1>
		T *create(Alloc &alloc, A0 a0, A1 a1)
		{
			typename Alloc::template rebind<T>::other al(alloc);
			T *ptr = al.allocate(1);
			new (ptr) T(a0, a1);
			return ptr;
		}
		template <class T, class Alloc>
		void release(T *ptr, Alloc &alloc)
		{
			typename Alloc::template rebind<T>::other al(alloc);
			al.destroy(ptr);
			al.deallocate(ptr, 1);
		}

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_ALLOCATOR_HPP

//EOF
