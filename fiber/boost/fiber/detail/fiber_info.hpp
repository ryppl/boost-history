
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_FIBER_INFO_H
#define BOOST_FIBERS_DETAIL_FIBER_INFO_H

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_info_base.hpp>
#include <boost/fiber/detail/move.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {
namespace detail {

class BOOST_FIBER_DECL fiber_info_default : public fiber_info_base
{
private:
	fiber_info_default( fiber_info_default &);
	fiber_info_default & operator=( fiber_info_default const&);

public:
	fiber_info_default() :
		fiber_info_base()
	{}
	
	void run()
	{ BOOST_ASSERT( ! "run() of master-fiber should never be executed"); }
};

template< typename Fn >
class fiber_info : public fiber_info_base
{
private:
	Fn	fn_;

	fiber_info( fiber_info &);
	fiber_info & operator=( fiber_info const&);

public:
#ifdef BOOST_HAS_RVALUE_REFS
	thread_data( Fn && fn, attributes const& attribs) :
		fiber_info_base( attribs),
		fn_( static_cast< Fn && >( fn) )
	{}
#else
	fiber_info( Fn fn, attributes const& attribs) :
		fiber_info_base( attribs),
		fn_( fn)
	{}

	fiber_info( boost::detail::fiber_move_t< Fn > fn, attributes const& attribs) :
		fiber_info_base( attribs),
		fn_( fn)
	{}
#endif            

	void run()
	{ fn_(); }
};

template< typename Fn >
class fiber_info< reference_wrapper< Fn > > : public fiber_info_base
{
private:
	Fn	&	fn_;

	fiber_info( fiber_info &);
	fiber_info & operator=( fiber_info const&);

public:
	fiber_info( reference_wrapper< Fn > fn, attributes const& attribs) :
		fiber_info_base( attribs),
		fn_( fn)
	{}
	
	void run()
	{ fn_(); }
};

template< typename Fn >
class fiber_info< const reference_wrapper< Fn > > : public fiber_info_base
{
private:
	Fn	&	fn_;

	fiber_info( fiber_info &);
	fiber_info & operator=( fiber_info const&);

public:
	fiber_info( const reference_wrapper< Fn > fn, attributes const& attribs) :
		fiber_info_base( attribs),
		fn_( fn)
	{}
	
	void run()
	{ fn_(); }
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_FIBER_INFO_H
