/* Boost bigint_storage_fixed.hpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BIGINT_BIGINT_STORAGE_FIXED_HPP
#define BOOST_BIGINT_BIGINT_STORAGE_FIXED_HPP

#include <vector>

namespace boost { namespace detail {
template <size_t N> struct bigint_storage_fixed
{
	template <typename T> class type
	{
		T data[N / sizeof(T)];
		size_t count;
	
	public:
	    type(): count(0)
	    {
	    }

	    size_t _max_size()
	    {
	    	return (N / sizeof(T));
	    }
		
		void resize(size_t size)
		{
			if (size > _max_size()) throw std::bad_alloc();
			
			count = size;
		}

		size_t size() const
		{
			return count;
		}

		bool empty() const
		{
			return count == 0;
		}

		const T* begin() const
		{
			return data;
		}
		
		T* begin()
		{
			return data;
		}

		const T* end() const
		{
			return data + count;
		}
		
		T* end()
		{
			return data + count;
		}

		const T& operator[](size_t index) const
		{
			return begin()[index];
		}

		T& operator[](size_t index)
		{
			return begin()[index];
		}
	};
};
} }  // namespace boost::detail

#endif // BOOST_BIGINT_BIGINT_STORAGE_FIXED_HPP
