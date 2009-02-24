//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_DETAIL_MOVE_H
#define BOOST_NET_DETAIL_MOVE_H

namespace boost { namespace net {
namespace detail
{
template< typename T >
class move_type
{
private:
	T	&	t_;

	void operator=( move_type &);

public:
	explicit move_type( T & t)
	: t_( t)
	{}

	T & operator*() const
	{ return t_; }

	T * operator->() const
	{ return & t_; }
};
}}}

#endif // BOOST_NET_DETAIL_MOVE_H

