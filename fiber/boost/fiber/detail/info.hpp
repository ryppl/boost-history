
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_INFO_H
#define BOOST_FIBERS_DETAIL_INFO_H

#include <cstddef>

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/info_base.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {
namespace detail {

class BOOST_FIBER_DECL info_default : public info_base
{
private:
	info_default( info_default &);
	info_default & operator=( info_default const&);

public:
	info_default() :
		info_base()
	{}
	
	void run()
	{ BOOST_ASSERT( ! "run() of master-fiber should never be executed"); }
};

template< typename Fn >
class info : public info_base
{
private:
	Fn	fn_;

	info( info &);
	info & operator=( info const&);

public:
#ifdef BOOST_HAS_RVALUE_REFS
	thread_data( Fn && fn, std::size_t stack_size) :
		info_base( stack_size),
		fn_( static_cast< Fn && >( fn) )
	{}
#else
	info( Fn fn, std::size_t stack_size) :
		info_base( stack_size),
		fn_( fn)
	{}

	info( boost::detail::thread_move_t< Fn > fn, std::size_t stack_size) :
		info_base( stack_size),
		fn_( fn)
	{}
#endif            

	void run()
	{ fn_(); }
};

template< typename Fn >
class info< reference_wrapper< Fn > > : public info_base
{
private:
	Fn	&	fn_;

	info( info &);
	info & operator=( info const&);

public:
	info( reference_wrapper< Fn > fn, std::size_t stack_size) :
		info_base( stack_size),
		fn_( fn)
	{}
	
	void run()
	{ fn_(); }
};

template< typename Fn >
class info< const reference_wrapper< Fn > > : public info_base
{
private:
	Fn	&	fn_;

	info( info &);
	info & operator=( info const&);

public:
	info( const reference_wrapper< Fn > fn, std::size_t stack_size) :
		info_base( stack_size),
		fn_( fn)
	{}
	
	void run()
	{ fn_(); }
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_INFO_H
