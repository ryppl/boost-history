// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_RECLAIMABLE_STORAGE_HPP
#define BOOST_MONOTONIC_RECLAIMABLE_STORAGE_HPP

#include <algorithm>
#include <boost/monotonic/fixed_storage.hpp>
#include <boost/monotonic/detail/pool.hpp>
#include <boost/monotonic/detail/link.hpp>
#include <boost/unordered/unordered_set.hpp>

namespace boost
{
	namespace monotonic
	{
		namespace detail
		{
			template <>
			struct storage_type<heap_region_tag, default_access_tag>
			{
				template <size_t N, size_t M, class Al>
				struct storage
				{
					typedef monotonic::reclaimable_storage<N,M,Al> type;
				};
			};
			template <class Pair>
			struct HashPairFirst
			{
				size_t operator()(Pair const &pair) const
				{
					return reinterpret_cast<size_t>(pair.first);
				}
			};
		}

		template <size_t InlineSize, size_t MinHeapIncrement, class Al>
		struct reclaimable_storage : storage_base
		{
			typedef reclaimable_storage<InlineSize, MinHeapIncrement, Al> This;
			typedef Al Allocator;
			typedef BOOST_DEDUCED_TYPENAME Allocator::template rebind<char>::other CharAllocator;
			typedef std::pair<char *, size_t> Allocation;
			typedef boost::unordered_set<
				Allocation
				, detail::HashPairFirst<Allocation>
				, std::less<Allocation>
				, Allocator> Allocations;

		private:
			CharAllocator alloc;
			Allocations allocations;

		public:
			reclaimable_storage()
			{
			}
			reclaimable_storage(Allocator const &A)
				: alloc(A)
			{
			}
			~reclaimable_storage()
			{
				release();
			}

			void reset()
			{
			}

			void release()
			{
				reset();
				BOOST_FOREACH(Allocation const &what, allocations)
				{
					alloc.deallocate(what.first, what.second);
				}
				allocations.clear();
			}

			void *allocate(size_t num_bytes, size_t /*alignment*/ = 1)
			{
				Allocation what;
				what.first = alloc.allocate(num_bytes);//, alignment);
				what.second = num_bytes;
				allocations.insert(what);
				return what.first;
			}

			void deallocate(void *ptr)
			{
				if (ptr == 0)
					return;
				char *cptr = (char *)ptr;
				Allocations::iterator iter = allocations.find(Allocation(cptr, size_t(0)));
				BOOST_ASSERT(iter != allocations.end());
				if (iter != allocations.end())
				{
					allocations.erase(iter);
				}
				alloc.deallocate(cptr, 1);
			}

			size_t max_size() const
			{
				return (std::numeric_limits<size_t>::max)();
			}

			size_t remaining() const
			{
				return max_size();
			}

			size_t used() const
			{
				size_t total = 0;
				BOOST_FOREACH(Allocation const &what, allocations)
				{
					total += what.second;
				}
				return total;
			}

			// ------------------------------------------------------------------------

			template <class Ty>
			Ty *uninitialised_create()
			{
				return reinterpret_cast<Ty *>(allocate_bytes<sizeof(Ty)>());
			}

			template <class Ty>
			Ty &create()
			{
				Ty *ptr = uninitialised_create<Ty>();
				construct(ptr, boost::is_pod<Ty>());
				return *ptr;
			}

			template <class Ty>
			void construct(Ty *ptr, const boost::true_type& /*is_pod*/)
			{
				// do nothing
			}

			template <class Ty>
			void construct(Ty *ptr, const boost::false_type&)
			{
				new (ptr) Ty();
			}

			template <class Ty>
			Ty &create(Ty const &X)
			{
				Ty *ptr = uninitialised_create<Ty>();
				new (ptr) Ty(X);
				return *ptr;
			}

			template <class Ty>
			void destroy(Ty &object)
			{
				object.~Ty();
				deallocate(&object, 1);
			}

			template <class Ty>
			void destroy(Ty const &object)
			{
				destroy(const_cast<Ty &>(object));
			}

			template <size_t N>
			char *allocate_bytes()
			{
				return allocate_bytes(N, boost::aligned_storage<N>::alignment);
			}

			char *allocate_bytes(size_t num_bytes, size_t alignment = 1)
			{
				return reinterpret_cast<char *>(allocate(num_bytes, alignment));
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_RECLAIMABLE_STORAGE_HPP

//EOF
