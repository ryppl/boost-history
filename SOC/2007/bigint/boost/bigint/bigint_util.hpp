/* Boost bigint.hpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BIGINT_BIGINT_UTIL_HPP
#define BOOST_BIGINT_BIGINT_UTIL_HPP

namespace boost { namespace detail { namespace bigint {

	inline bool isspace(char ch)
	{
		return ::isspace(ch);
	}
	
	inline bool isspace(wchar_t ch)
	{
		return ::iswspace(ch);
	}
	
	inline size_t length(const char* str)
	{
		return ::strlen(str);
	}
	
	inline size_t length(const wchar_t* str)
	{
		return ::wcslen(str);
	}
	
	inline size_t get_bit_count(size_t count, int base)
	{
		// right now it returns more bits than needed - perhaps later this will be improved
		
		size_t bits_in_base = 0;
		
		while (base > 1)
		{
			++bits_in_base;
			base = (base + 1) / 2;
		}
		
		return count * bits_in_base;
	}

} } } // namespace boost::detail::bigint

#endif // BOOST_BIGINT_BIGINT_UTIL_HPP
