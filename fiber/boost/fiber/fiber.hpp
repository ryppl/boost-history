
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_FIBER_H
#define BOOST_FIBERS_FIBER_H

#include <iostream>

#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_info.hpp>
#include <boost/fiber/detail/move.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

class rrp;

class BOOST_FIBER_DECL fiber
{
private:
	friend class rrp;

	struct dummy;

	detail::fiber_info_base::ptr_t	info_;

	explicit fiber( detail::fiber_info_base::ptr_t);

	fiber( fiber &);
	fiber & operator=( fiber &);

	void init_();

#ifdef BOOST_HAS_RVALUE_REFS
	template< typename Fn >
	static detail::fiber_info_base::ptr_t make_info_( Fn && fn)
	{
		return detail::fiber_info_base::ptr_t(
			new detail::fiber_info< typename remove_reference< Fn >::type >(
				static_cast< Fn && >( fn), attributes() ) );
	}

	static detail::fiber_info_base::ptr_t make_info_( void ( * fn)() );
#else
	template< typename Fn >
	static detail::fiber_info_base::ptr_t make_info_( Fn fn)
	{
		return detail::fiber_info_base::ptr_t(
			new detail::fiber_info< Fn >( fn, attributes() ) );
	}

	template< typename Fn >
	static detail::fiber_info_base::ptr_t make_info_(
		boost::detail::fiber_move_t< Fn > fn)
	{
		return detail::fiber_info_base::ptr_t(
			new detail::fiber_info< Fn >( fn, attributes() ) );
	}
#endif

public:
	class id;

	static void convert_thread_to_fiber();

	fiber();

#ifdef BOOST_HAS_RVALUE_REFS
	template< typename Fn >
	fiber( Fn && fn) :
		info_( make_info_( static_cast< Fn && >( fn) ) )
	{ init_(); }

	fiber( fiber &&);
	
	fiber & operator=( fiber &&);

	fiber && move();
#else
#ifdef BOOST_NO_SFINAE
	template< typename Fn >
	explicit fiber( Fn fn) :
		info_( make_info_( fn) )
	{ init_(); }
#else
	template< typename Fn >
	explicit fiber(
			Fn fn,
			typename disable_if< boost::is_convertible< Fn &, boost::detail::fiber_move_t< Fn > >, dummy * >::type = 0) :
		info_( make_info_( fn) )
	{ init_(); }
#endif
	template< typename Fn >
	explicit fiber( boost::detail::fiber_move_t< Fn > fn) :
		info_( make_info_( fn) )
	{ init_(); }

	fiber( boost::detail::fiber_move_t< fiber >);
	
	fiber & operator=( boost::detail::fiber_move_t< fiber >);
	
	operator boost::detail::fiber_move_t< fiber >();
	
	boost::detail::fiber_move_t< fiber > move();
#endif

#define BOOST_FIBER_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
#define BOOST_ENUM_FIBER_ARGS(n) BOOST_PP_ENUM(n, BOOST_FIBER_ARG, ~)

#define BOOST_FIBER_FIBER_CTOR(z, n, unused) \
	template< typename Fn, BOOST_PP_ENUM_PARAMS(n, typename A) > \
	fiber( Fn fn, BOOST_ENUM_FIBER_ARGS(n)) : \
		info_( \
			make_info_( \
				boost::bind( boost::type< void >(), fn, BOOST_PP_ENUM_PARAMS(n, a)) ) ) \
	{ init_(); } \

#ifndef BOOST_FIBER_MAX_ARITY
#define BOOST_FIBER_MAX_ARITY 10
#endif

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_FIBER_MAX_ARITY, BOOST_FIBER_FIBER_CTOR, ~)

#undef BOOST_FIBER_FIBER_CTOR

	typedef detail::fiber_info_base::ptr_t::unspecified_bool_type	unspecified_bool_type;

	operator unspecified_bool_type() const;

	bool operator!() const;

	void swap( fiber & other);

	id get_id() const;

	attributes const& get_attributes() const;
	void set_attributes( attributes const&);

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

template< typename Fn >
fiber make_fiber( Fn fn)
{ return fiber( fn); }

#define BOOST_FIBER_make_info_FUNCTION(z, n, unused) \
template< typename Fn, BOOST_PP_ENUM_PARAMS(n, typename A) > \
fiber make_fiber( Fn fn, BOOST_ENUM_FIBER_ARGS(n)) \
{ return fiber( fn, BOOST_PP_ENUM_PARAMS(n, a) ); } \

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_FIBER_MAX_ARITY, BOOST_FIBER_make_info_FUNCTION, ~)

#undef BOOST_FIBER_make_info_FUNCTION
#undef BOOST_ENUM_FIBER_ARGS
#undef BOOST_FIBER_ARG
#undef BOOST_FIBER_MAX_ARITY

}

using fibers::fiber;

inline
void swap( fiber & lhs, fiber & rhs)
{ return lhs.swap( rhs); }

#ifdef BOOST_HAS_RVALUE_REFS
inline
fiber && move( fiber && f)
{ return f; }
#else
inline
fiber move( boost::detail::fiber_move_t< fiber > f)
{ return fiber( f); }
#endif

}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_FIBER_H
