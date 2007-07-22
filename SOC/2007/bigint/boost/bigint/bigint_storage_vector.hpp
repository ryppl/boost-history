/* Boost bigint_storage_vector.hpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BIGINT_BIGINT_STORAGE_VECTOR_HPP
#define BOOST_BIGINT_BIGINT_STORAGE_VECTOR_HPP

#include <vector>

namespace boost { namespace detail {
template <typename T> class bigint_storage_vector
{
	std::vector<T> data;

public:
	void resize(size_t size)
	{
		data.resize(size);
	}

	size_t size() const
	{
		return data.size();
	}

	bool empty() const
	{
		return data.empty();
	}

	const T* begin() const
	{
		return data.empty() ? 0 : &*data.begin();
	}
		
	T* begin()
	{
		return data.empty() ? 0 : &*data.begin();
	}

	const T* end() const
	{
		return data.empty() ? 0 : &*(data.end()-1)+1;
	}
		
	T* end()
	{
		return data.empty() ? 0 : &*(data.end()-1)+1;
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
} }  // namespace boost::detail

#endif // BOOST_BIGINT_BIGINT_STORAGE_VECTOR_HPP
