// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cassert>
#include <boost/array.hpp>
#include <boost/aligned_storage.hpp>

// define this to use boost::auto_buffer<> rather than boost::array for monotonic::inline_storage
//#define BOOST_MONOTONIC_USE_AUTOBUFFER
// this currently does not work, because resizing the underlying buffer breaks
// containers that may reference the previously used memory

#ifdef BOOST_MONOTONIC_USE_AUTOBUFFER
#	include <boost/auto_buffer.hpp>
#endif

namespace boost
{
	namespace monotonic
	{
		/// storage for an allocator that is on the stack or heap
		template <size_t N>
		struct inline_storage : storage_base
		{

#ifdef BOOST_MONOTONIC_USE_AUTOBUFFER
			typedef boost::auto_buffer<char, boost::store_n_bytes<N> > buffer_type;
#else
			typedef boost::array<char, N> buffer_type;
#endif

		private:
			buffer_type buffer;			///< the storage
			size_t cursor;				///< pointer to current index within storage for next allocation

		public:
			inline_storage() : cursor(0)
			{
			}

			buffer_type const &get_buffer()  const
			{
				return buffer;
			}
			const char *begin() const
			{
				return &buffer[0];
			}
			const char *end() const
			{
				return &buffer[N - 1];
			}
			void reset()
			{
				cursor = 0;
			}

			size_t get_cursor() const
			{
				return cursor;
			}

			void set_cursor(size_t c)
			{
				cursor = c;
			}

			/// allocate storage, given alignment requirement
			void *allocate(size_t num_bytes, size_t alignment)
			{
				// ensure we return a point on an aligned boundary
				size_t extra = cursor & (alignment - 1);
				if (extra > 0)
					extra = alignment - extra;
				size_t required = num_bytes + extra;
				BOOST_ASSERT(cursor + required <= N);
#ifdef BOOST_MONOTONIC_USE_AUTOBUFFER
				buffer.uninitialized_resize(buffer.size() + required);
#endif
				char *ptr = &buffer[cursor];
				cursor += required;
				return ptr + extra;
			}

			/// no work is done to deallocate storage
			void deallocate(void * /*p*/, size_t /*n*/)
			{
			}

			/// the maximum size is determined at compile-time
			size_t max_size() const
			{
				return N;
			}

			size_t remaining() const
			{
				return N - cursor;
			}
		};
	}
}

//EOF
