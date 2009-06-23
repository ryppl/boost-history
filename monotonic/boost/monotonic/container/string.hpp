// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_STRING_HPP
#define BOOST_MONOTONIC_STRING_HPP

#include <string>
#include <boost/monotonic/allocator.hpp>

namespace boost
{
	namespace monotonic
	{
		/// a string that uses a monotonic allocator in the given region
		template <class Region = default_region_tag>
		struct string
		{
			typedef char Ch;
			typedef std::char_traits<Ch> Tr;
			typedef allocator<Ch, Region> Allocator;
			typedef std::basic_string<Ch, Tr, Allocator> Impl;
			typedef size_t size_type;
			typedef typename Impl::iterator iterator;
			typedef typename Impl::const_iterator const_iterator;
			typedef typename Impl::value_type value_type;

		private:
			Impl impl;

		public:
			string()
			{
			}
			string(string const &other)
				: impl(other.impl)
			{
			}
			template <class U>
			string(allocator<U, Region> &alloc)
				: impl(alloc)
			{
			}
			string(const Ch *str)
				: impl(str)
			{
			}
			template <class U>
			string(const Ch *str, allocator<U, Region> &alloc)
				: impl(str, alloc)
			{
			}
			template <class II>
			string(II F, II L)
				: impl(F, L)
			{
			}
			template <class II, class U>
			string(II F, II L, allocator<U, Region> &alloc)
				: impl(F, L, alloc)
			{
			}

			string &operator+=(Ch const *str)
			{
				impl += str;
				return *this;
			}

		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_STRING_HPP

//EOF
