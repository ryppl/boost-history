
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_DETAIL_FIBER_INFO_H
#define BOOST_FIBER_DETAIL_FIBER_INFO_H

extern "C" {

#include <ucontext.h>

}

#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>

# if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#include <boost/fiber/detail/fiber_info_windows.hpp>

# elif defined(_POSIX_VERSION)
#include <boost/fiber/detail/fiber_info_posix.hpp>

# endif

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {
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
	
	void run() {}
};

template< typename Fn >
class fiber_info : public fiber_info_base
{
private:
	Fn	fn_;

	fiber_info( fiber_info &);
	fiber_info & operator=( fiber_info const&);

public:
	fiber_info( Fn fn, attributes const& attribs) :
		fiber_info_base( attribs),
		fn_( fn)
	{}

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

#endif // BOOST_FIBER_DETAIL_FIBER_INFO_H
