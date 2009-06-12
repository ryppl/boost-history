// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cassert>
#include <boost/array.hpp>

namespace boost
{
	namespace monotonic
	{
		/// storage for an allocator that is on the stack or heap
		template <size_t N>
		struct inline_storage : storage_base
		{
			// how to determine alignment requirements?
			BOOST_STATIC_CONSTANT(size_t, Alignment = 16);

		private:
			boost::array<char, N> buffer;	///< the storage
			size_t cursor;					///< pointer to current index within storage for next allocation

		public:
			inline_storage() : cursor(0)
			{
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

			/// allocate storage. ignore hint.
			void *allocate(size_t num_bytes, void const * = 0)
			{
				// ensure we return a point on an aligned boundary
				//size_t extra = num_bytes % Alignment;
				size_t extra = 0;//num_bytes & 15;
				size_t required = num_bytes + extra;
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
