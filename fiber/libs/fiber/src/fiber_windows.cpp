
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

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

void
fiber::init_()
{
	typedef void fn_type( fiber *);
	typedef void ( * st_fn)();
	fn_type * fn_ptr( trampoline);

	info_->uctx = ::CreateFiber(
		info_->stack_size_,
		static_cast< LPFIBER_START_ROUTINE >( & trampoline< fiber >),
		static_cast< LPVOID >( this) );
}

void
fiber::switch_to( fiber & to)
{
	if ( ::SiwtchToFiber( & info_->uctx) != 0)
		throw system::system_error(
			system::error_code(
				::GetLastError(),
				system::system_category) );
}

}}

#include <boost/config/abi_suffix.hpp>
