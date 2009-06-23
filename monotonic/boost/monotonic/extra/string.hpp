// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_STRING_HPP
#define BOOST_MONOTONIC_STRING_HPP

#include <string>
#include <boost/monotonic/allocator.hpp>
#include <boost/monotonic/container.hpp>

namespace boost
{
	namespace monotonic
	{
		//template <class Ch = char, class Tr = std::char_traits<Ch> >
		struct string
		{
			typedef char Ch;
			typedef std::char_traits<Ch> Tr;
			typedef allocator<Ch> Allocator;

			typedef std::basic_string<Ch, Tr, Allocator > Impl;
			typedef size_t size_type;
			typedef Impl::iterator iterator;
			typedef Impl::const_iterator const_iterator;
			typedef Impl::value_type value_type;

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
			string(storage_base &store)
				: impl(store)
			{
			}
			template <class U>
			string(allocator<U> &store)
				: impl(store)
			{
			}
			string(const Ch *str, storage_base &store)
				: impl(str, store)
			{
			}
			template <class II>
			string(II F, II L, storage_base &store)
				: impl(F, L, store)
			{
			}
			string &operator=(string const &other)
			{
				if (&other == this)
					return *this;
				impl = other.impl;
				return *this;
			}
			//string &operator+=(string const &other)
			//{

			//}

		};
	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_STRING_HPP

//EOF
