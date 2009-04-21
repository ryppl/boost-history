
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_ID_H
#define BOOST_TASK_ID_H

#include <iostream>
#include <string>

namespace boost { namespace task
{
template< typename R >
class task;

class id
{
private:
	template< typename R >
	friend class task;

	std::string	id_;

	id( std::string const& id__)
	: id_( id__)
	{}

public:
	id()
	: id_("{not-any-task}")
	{}

	bool operator==( id const& other) const
	{ return id_ == other.id_; }

	bool operator!=( id const& other) const
	{ return id_ != other.id_; }

	bool operator<( id const& other) const
	{ return id_ < other.id_; }

	bool operator>( id const& other) const
	{ return id_ > other.id_; }

	bool operator<=( id const& other) const
	{ return id_ <= other.id_; }

	bool operator>=( id const& other) const
	{ return id_ >= other.id_; }

	template< typename charT, typename traitsT >
	friend std::basic_ostream< charT, traitsT > & 
	operator<<( std::basic_ostream< charT, traitsT > & os, id const& r)
	{ return os << r.id_; }

	void swap( id & other)
	{ id_.swap( other.id_); }
};
} }

#endif // BOOST_TASK_ID_H

