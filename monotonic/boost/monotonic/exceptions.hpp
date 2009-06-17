// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_EXCEPTIONS_HPP
#define BOOST_MONOTONIC_EXCEPTIONS_HPP

#include <boost/monotonic/config.hpp>
#include <boost/exception.hpp>

namespace boost
{
	namespace monotonic
	{
		/// a request was made for a reference to storage that does not exist
		struct no_storage : std::exception
		{
			no_storage() { }
			no_storage(const char *text) : std::exception(text) { }
		};
	
	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_EXCEPTIONS_HPP

//EOF
