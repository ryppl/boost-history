// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_ALLOCATOR_BASE_HPP
#define BOOST_MONOTONIC_ALLOCATOR_BASE_HPP

//#define BOOST_MONOTONIC_USE_POOLS

#include <boost/assert.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>

#include <boost/monotonic/static_storage.hpp>
#include <boost/monotonic/container.hpp>

#ifdef BOOST_MONOTONIC_USE_POOLS
#	include <boost/monotonic/storage_pool.hpp>
#endif

namespace boost
{
	namespace monotonic
	{
		namespace detail
		{
			template <bool is_mono_container>
			struct Construct
			{
				template <class T, class Alloc>
				static void Given(T *ptr, Alloc *allocator)
				{
					new (ptr) T();
				}
				template <class T, class Alloc>
				static void Given(T *ptr, T const &val, Alloc *allocator)
				{
					new (ptr) T(val);
				}
			};
			template <>
			struct Construct<true>
			{
				template <class T, class Alloc>
				static void Given(T *ptr, Alloc *allocator)
				{
					new (ptr) T(*allocator);
				}
				template <class T, class Alloc>
				static void Given(T *ptr, T const &val, Alloc *allocator)
				{
					// unfortunately, there is no requirement for a container to 
					// have a copy-ctor that also passes an allocator.
					new (ptr) T(*allocator);
					*ptr = val;
				}
			};

			template <bool is_monotonic_container, class T>
			struct Create
			{
				template <class Storage>
				static T Given(Storage &)
				{
					return T();
				}
			};
			template <class T>
			struct Create<true, T>
			{
				template <class Storage>
				static T Given(Storage &storage)
				{
					return T();//storage);
				}
			};
		}
		namespace detail
		{
			template <size_t A, size_t B>
			struct miniumum
			{
				BOOST_STATIC_CONSTANT(size_t, value = A < B ? A : B);
			};
			template <size_t A, size_t B>
			struct maximum
			{
				BOOST_STATIC_CONSTANT(size_t, value = B < A ? A : B);
			};
		}

		/// common to other monotonic allocators for type T of type Derived
		template <class T, class Derived>
		struct allocator_base
		{
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef T *pointer;
			typedef const T *const_pointer;
			typedef T &reference;
			typedef const T &const_reference;
			typedef T value_type;

			BOOST_STATIC_CONSTANT(size_t, PoolSize = 1000);
			BOOST_STATIC_CONSTANT(size_t, alignment = boost::aligned_storage<sizeof(T)>::alignment);
			BOOST_STATIC_CONSTANT(size_t, MinSize = 72);
			BOOST_STATIC_CONSTANT(size_t, AlignedSize = alignment + sizeof(T));

			BOOST_STATIC_CONSTANT(size_t, Size = sizeof(T));
			BOOST_STATIC_CONSTANT(size_t, NodeSize1 = (Size + MinSize)/MinSize);
			BOOST_STATIC_CONSTANT(size_t, NodeSize = NodeSize1*MinSize);

			//typedef storage_pool<T, NodeSize> Pool;

		//private:
			storage_base *storage;
			//static Pool pool;

		public:
			allocator_base(storage_base &store) throw() 
				: storage(&store) { }

			allocator_base(const allocator_base& alloc) throw() 
				: storage(alloc.storage) { }

			template <class U, class D> 
			allocator_base(const allocator_base<U,D> &alloc) throw()
				: storage(alloc.storage) { }

			pointer address(reference x) const
			{
				return &x;
			}

			const_pointer address(const_reference x) const
			{
				return &x;
			}

			pointer allocate(size_type num, const void * /*hint*/ = 0)
			{
				BOOST_ASSERT(num > 0);
				BOOST_ASSERT(storage != 0);
#ifdef BOOST_MONOTONIC_USE_POOLS
				if (pointer ptr = pool.allocate(num))
				{
					return ptr;
				}
				pool.reserve_pool(storage, std::max<size_t>(8, std::max(num*2, pool.capacity()*2)));
				return pool.allocate(num);
#else
				return reinterpret_cast<T *>(storage->allocate(num*sizeof(T), alignment));
#endif
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
				new (ptr) T();//detail::Construct<detail::is_monotonic<T>::value>::Given(ptr, static_cast<Derived *>(this));
			}

			void construct(pointer ptr, const T& val)
			{
				new (ptr) T(val);//detail::Construct<detail::is_monotonic<T>::value>::Given(ptr, val, static_cast<Derived *>(this));
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

			friend bool operator==(allocator_base<T,Derived> const &A, allocator_base<T,Derived> const &B) 
			{ 
				return A.storage == B.storage;
			}

			friend bool operator!=(allocator_base<T,Derived> const &A, allocator_base<T,Derived> const &B) 
			{ 
				return A.storage != B.storage;
			}
		};

/*
		template <class T, class Derived>
		typename allocator_base<T, Derived>::Pool allocator_base<T, Derived>::pool;
*/

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_ALLOCATOR_BASE_HPP

//EOF
