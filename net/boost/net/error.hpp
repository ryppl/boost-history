//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_ERROR_H
#define BOOST_NET_ERROR_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#ifdef BOOST_NET_HAVE_NETDB_H
#include <netdb.h>
#endif
}

#include <cstring>
#include <string>

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/system/error_code.hpp>

namespace boost { namespace net
{
static system::error_category const& get_eof_category();
static system::error_category const& eof_error_cat = get_eof_category();

class eof_error_category
: public system::error_category
{
public:
	const char * name() const
	{ return "eof"; }

	system::error_condition default_error_condition( int ev) const
	{ return system::error_condition( ev, eof_error_cat); }

	std::string message( int ev) const
	{
		BOOST_ASSERT( ev == EOF);
		return "end of file or stream";
	}
};

system::error_category const& get_eof_category()
{
	static const eof_error_category eof_cat;
	return eof_cat;
}

static system::error_category const& get_gai_category();
static system::error_category const& gai_error_cat = get_gai_category();

class gai_error_category
: public system::error_category
{
public:
	const char * name() const
	{ return "gai"; }

	system::error_condition default_error_condition( int ev) const
	{ return system::error_condition( ev, gai_error_cat); }

	std::string message( int ev) const
	{ return ::gai_strerror( ev); }
};

system::error_category const& get_gai_category()
{
	static const gai_error_category gai_cat;
	return gai_cat;
}

template< typename Result >
inline Result wrap_error( Result result, system::error_code & ec)
{
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
  ec = system::error_code(
	::WSAGetLastError(),
	system::get_system_category() );
#else
  ec = system::error_code(
	errno,
	system::get_system_category() );
#endif
  return result;
}
} }

#endif // BOOST_NET_ERROR_H
