
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_INFO_POSIX_H
#define BOOST_FIBERS_DETAIL_INFO_POSIX_H

extern "C" {

#include <ucontext.h>

}

#include <cstddef>
#include <stack>

#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_array.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/interrupt_flags.hpp>
#include <boost/fiber/detail/state_flags.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

class strategy;

namespace detail {

struct BOOST_FIBER_DECL info_base
{
	typedef intrusive_ptr< info_base >	ptr_t;
	typedef function< void() >					callable_t;
	typedef std::stack< callable_t >			callable_stack_t;

	uint32_t				use_count;
	std::size_t				stack_size;
	int						priority;
	::ucontext_t			uctx;
	shared_array< char >	uctx_stack;
	state_t					state;
	interrupt_t				interrupt;
	callable_stack_t		at_exit;
	strategy		*	st;

	info_base();

	info_base( std::size_t);

	virtual ~info_base() {}

	virtual void run() = 0;

    inline friend void intrusive_ptr_add_ref( info_base * p)
    { ++p->use_count; }

    inline friend void intrusive_ptr_release( info_base * p)
    { if ( --p->use_count == 0) delete p; }
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_INFO_POSIX_H
