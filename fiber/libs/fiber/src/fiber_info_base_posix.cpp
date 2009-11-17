
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/detail/fiber_info_base_posix.hpp>

#include <cerrno>

#include <boost/assert.hpp>
#include <boost/system/system_error.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {
namespace detail {

fiber_info_base::fiber_info_base() :
	use_count( 0),
	attrs(),
	uctx(),
	uctx_stack(),
	state( STATE_MASTER),
	interrupt( INTERRUPTION_DISABLED),
	at_exit()
{}

fiber_info_base::fiber_info_base( attributes const& attrs_) :
	use_count( 0),
	attrs( attrs_),
	uctx(),
	uctx_stack( new char[attrs.stack_size()]),
	state( STATE_NOT_STARTED),
	interrupt( INTERRUPTION_DISABLED)
{
	BOOST_ASSERT( uctx_stack);

	if ( ::getcontext( & uctx) == -1)
		throw system::system_error(
			system::error_code(
				errno,
				system::system_category) );
	uctx.uc_stack.ss_sp = uctx_stack.get();
	uctx.uc_stack.ss_size = attrs.stack_size();
	uctx.uc_link = 0;
}

}}}

#include <boost/config/abi_suffix.hpp>
