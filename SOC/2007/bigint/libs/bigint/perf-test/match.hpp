/* Boost match.hpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * Implementation taken from pugxml library
 */

#ifndef MATCH_HPP
#define MATCH_HPP

namespace detail
{
	int strcmpwild(const char* src, const char* dst);

	// Character set pattern match.
	inline int strcmpwild_cset(const char** src, const char** dst)
	{
		int find = 0, excl = 0, star = 0;
		
		if (**src == '!')
		{
			excl = 1;
			++(*src);
		}
		
		while (**src != ']' || star == 1)
		{
			if (find == 0)
			{
				if (**src == '-' && *(*src-1) < *(*src+1) && *(*src+1) != ']' && star == 0)
				{
					if (**dst >= *(*src-1) && **dst <= *(*src+1))
					{
						find = 1;
						++(*src);
					}
				}
				else if (**src == **dst) find = 1;
			}
			++(*src);
			star = 0;
		}

		if (excl == 1) find = (1 - find);
		if (find == 1) ++(*dst);
	
		return find;
	}

	// Wildcard pattern match.
	inline int strcmpwild_astr(const char** src, const char** dst)
	{
		int find = 1;
		++(*src);
		while ((**dst != 0 && **src == '?') || **src == '*')
		{
			if(**src == '?') ++(*dst);
			++(*src);
		}
		while (**src == '*') ++(*src);
		if (**dst == 0 && **src != 0) return 0;
		if (**dst == 0 && **src == 0) return 1;
		else
		{
			if (strcmpwild(*src,*dst))
			{
				do
				{
					++(*dst);
					while(**src != **dst && **src != '[' && **dst != 0) 
						++(*dst);
				}
				while ((**dst != 0) ? strcmpwild(*src,*dst) : 0 != (find=0));
			}
			if (**dst == 0 && **src == 0) find = 1;
			return find;
		}
	}

	// Compare two strings, with globbing, and character sets.
	int strcmpwild(const char* src, const char* dst)
	{
		int find = 1;
		for(; *src != 0 && find == 1 && *dst != 0; ++src)
		{
			switch (*src)
			{
				case '?': ++dst; break;
				case '[': ++src; find = strcmpwild_cset(&src,&dst); break;
				case '*': find = strcmpwild_astr(&src,&dst); --src; break;
				default : find = (int) (*src == *dst); ++dst;
			}
		}
		while (*src == '*' && find == 1) ++src;
		return (find == 1 && *dst == 0 && *src == 0) ? 0 : 1;
	}
}

inline bool match(const char* string, const char* pattern)
{
	return detail::strcmpwild(pattern, string) == 0;
}

#endif
