
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

	detail::fiber_info_base::ptr_t	info;

	fiber( fiber &);
	fiber & operator=( fiber const&);

	void init();

	template< typename Fn >
	static inline detail::fiber_info_base::ptr_t make_info( Fn fn, attributes const& attribs)
	{ return detail::fiber_info_base::ptr_t( new detail::fiber_info< Fn >( fn, attribs) ); }

	struct dummy;

public:
	class id;

	static void convert_thread_to_fiber();

	fiber();

	template< typename Fn >
	explicit fiber( Fn fn, attributes const& attribs) :
		info( make_info( fn, attribs) )
	{ init(); }

#ifndef BOOST_FIBER_MAX_ARITY
#define BOOST_FIBER_MAX_ARITY 10
#endif

#define BOOST_FIBER_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
#define BOOST_ENUM_FIBER_ARGS(n) BOOST_PP_ENUM(n, BOOST_FIBER_ARG, ~)

#define BOOST_FIBER_CTOR(z, n, unused)	\
	template<							\
		typename Fn,					\
		BOOST_PP_ENUM_PARAMS(n, typename A)	\
	>										\
	fiber( Fn fn, attributes const& attribs, BOOST_ENUM_FIBER_ARGS(n)) :	\
		info(																\
			make_info(														\
				boost::bind( boost::type< void >(), fn, BOOST_PP_ENUM_PARAMS(n, a) ),	\
				attribs) )																\
	{ init(); }

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_FIBER_MAX_ARITY, BOOST_FIBER_CTOR, ~)

#undef BOOST_FIBER_CTOR
#undef BOOST_FIBER_MAX_ARITY

	~fiber();

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

	detail::fiber_info_base::ptr_t	info;

	explicit id( detail::fiber_info_base::ptr_t info_) :
		info( info_)
	{}

public:
	id() :
		info()
	{}

	bool operator==( id const& other) const
	{ return info == other.info; }

	bool operator!=( id const& other) const
	{ return info != other.info; }
	
	bool operator<( id const& other) const
	{ return info < other.info; }
	
	bool operator>( id const& other) const
	{ return other.info < info; }
	
	bool operator<=( id const& other) const
	{ return !( other.info < info); }
	
	bool operator>=( id const& other) const
	{ return ! ( info < other.info); }

	template< typename charT, class traitsT >
	friend std::basic_ostream< charT, traitsT > &
	operator<<( std::basic_ostream< charT, traitsT > & os, id const& other)
	{
		if ( other.info)
			return os << other.info;
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

