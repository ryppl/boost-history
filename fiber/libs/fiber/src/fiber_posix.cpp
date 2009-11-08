
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/fiber.hpp>

extern "C" {

#include <ucontext.h>

}

#include <cerrno>

#include <boost/system/system_error.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

void
fiber::init()
{
	typedef void fn_type( fiber *);
	typedef void ( * st_fn)();
	fn_type * fn_ptr( trampoline);

	::makecontext(
		& info->uctx,
		( st_fn)( fn_ptr),
		1,
		this);
}

void
fiber::switch_to( fiber & to)
{
	if ( ::swapcontext( & info->uctx, & to.info->uctx) != 0)
		throw system::system_error(
			system::error_code(
				errno,
				system::system_category) );
}

}}

#include <boost/config/abi_suffix.hpp>


