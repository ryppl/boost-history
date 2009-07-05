// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_EXCEPTION_HPP
#define BOOST_CLONEABLE_EXCEPTION_HPP

#include <exception>
#include <boost/cloneable/detail/prefix.hpp>

#define BOOST_CLONEABLE_EXCEPTION_0(name) \
	class name : public std::exception	\
	{															\
	public:														\
		name( const char* what = BOOST_PP_STRINGIZE(name) ) : std::exception( what )		\
		{ }														\
	}

namespace boost
{
	namespace cloneable
	{
		BOOST_CLONEABLE_EXCEPTION_0(empty_object);
		BOOST_CLONEABLE_EXCEPTION_0(empty_allocator);
		BOOST_CLONEABLE_EXCEPTION_0(no_default_construction);
		BOOST_CLONEABLE_EXCEPTION_0(not_cloneable);

	}
}

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_EXCEPTION_HPP

//EOF
