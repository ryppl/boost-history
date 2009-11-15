
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/fiber.hpp>

extern "C" {

#include <windows.h>
#include <winnt.h>

}

#include <cerrno>

#include <boost/system/system_error.hpp>

#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/utility.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

VOID trampoline( LPVOID vp)
{
	detail::fiberinfo_base * self(
			static_cast< detail::fiber_info_base * >( vp) );
	BOOST_ASSERT( self);
	try
	{ self->run(); }
	catch ( fiber_interrupted const&)
	{}
	catch (...)
	{}
 	this_fiber::cancel();
}

void
fiber::init_()
{
	if ( ! info_) throw fiber_moved();

	info_->uctx = ::CreateFiber(
		info_->attrs.stacksize(),
		static_cast< LPFIBER_START_ROUTINE >( & trampoline),
		static_cast< LPVOID >( info_.get() ) );
}

void
fiber::switch_to_( fiber & to)
{
	if ( ! info_) throw fiber_moved();

	if ( ::SiwtchToFiber( & to.info_->uctx) != 0)
		throw system::system_error(
			system::error_code(
				::GetLastError(),
				system::system_category) );
}

void
fiber::convert_thread_to_fiber()
{
	if ( ! ::ConvertThreadToFiber( 0) )
		throw system::system_error(
			system::error_code(
				::GetLastError(),
				system::system_category) );
}

}}

#include <boost/config/abi_suffix.hpp>
