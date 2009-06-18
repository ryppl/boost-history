// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_LOCAL_HPP
#define BOOST_MONOTONIC_LOCAL_HPP

#include <boost/monotonic/storage.hpp>

namespace boost
{
	namespace monotonic
	{
		template <class Storage>
		struct local : storage_base
		{

		private:
			Storage store;
			storage_base *old;

		public:
			local()
			{
				old = set_storage(store);
			}
			~local()
			{
				if (old)
					set_storage(*old);
				else
					default_storage();
			}

			void reset()
			{
				store.reset();
			}
			void release()
			{
				store.release();
			}

			template <class T>
			allocator<T> make_allocator()
			{
				return allocator<T>(store);
			}

			// the number of bytes to allocate, and the alignment to use
			void *allocate(size_t num_bytes, size_t alignment)
			{
				return store.allocate(num_bytes, alignment);
			}

			size_t max_size() const
			{
				return store.max_size();
			}

			size_t used() const
			{
				return store.used();
			}

			size_t remaining() const
			{
				return store.remaining();
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_LOCAL_HPP

//EOF
