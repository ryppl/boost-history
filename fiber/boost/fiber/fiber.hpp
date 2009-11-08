
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_FIBER_H
#define BOOST_FIBER_FIBER_H

#include <iostream>

#include <boost/bind.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_info.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

class fiber;
void trampoline( fiber * fib);

class BOOST_FIBER_DECL fiber : private noncopyable
{
private:
	friend void trampoline( fiber *);

	detail::fiber_info_base::ptr_t	info_;

	fiber( fiber &);
	fiber & operator=( fiber const&);

	void init_();

	struct dummy;

public:
	class id;

	static void convert_thread_to_fiber();

	fiber();

	template< typename Fn >
	explicit fiber( Fn fn, attributes const& attrs) :
		info_( new detail::fiber_info< Fn >( fn, attrs) )
	{ init_(); }

	void swap( fiber & other);

	id get_id() const;

	bool operator==( fiber const& other) const;
	bool operator!=( fiber const& other) const;

	void switch_to( fiber &);
};

class fiber::id
{
private:
	friend class fiber;

	detail::fiber_info_base::ptr_t	info_;

	explicit id( detail::fiber_info_base::ptr_t info) :
		info_( info)
	{}

public:
	id() :
		info_()
	{}

	bool operator==( id const& other) const
	{ return info_ == other.info_; }

	bool operator!=( id const& other) const
	{ return info_ != other.info_; }
	
	bool operator<( id const& other) const
	{ return info_ < other.info_; }
	
	bool operator>( id const& other) const
	{ return other.info_ < info_; }
	
	bool operator<=( id const& other) const
	{ return !( other.info_ < info_); }
	
	bool operator>=( id const& other) const
	{ return ! ( info_ < other.info_); }

	template< typename charT, class traitsT >
	friend std::basic_ostream< charT, traitsT > &
	operator<<( std::basic_ostream< charT, traitsT > & os, id const& other)
	{
		if ( other.info_)
			return os << other.info_;
		else
			return os << "{not-a-fiber}";
	}
};

void trampoline( fiber *);

}

inline
void swap( fiber::fiber & lhs, fiber::fiber & rhs)
{ return lhs.swap( rhs); }

}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBER_FIBER_H

