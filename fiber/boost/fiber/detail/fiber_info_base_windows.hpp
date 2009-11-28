
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_FIBER_INFO_WINDOWS_H
#define BOOST_FIBERS_DETAIL_FIBER_INFO_WINDOWS_H

extern "C" {

#include <windows.h>

}

#include <cstddef>
#include <stack>

#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_state.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {
namespace fibers {

class strategy;

namespace detail {

struct BOOST_FIBER_DECL fiber_info_base
{
	typedef intrusive_ptr< fiber_info_base >	ptr_t;	
	typedef function< void() >					callable_t;
	typedef std::stack< callable_t >			callable_stack_t;

	uint32_t			use_count;
	std::size_t			stack_size;
	int					priority;
	LPVOID				uctx;
	fiber_state_t		state;
	fiber_interrupt_t	interrupt;
	callable_stack_t	at_exit;
	strategy		*	st;

	fiber_info_base();

	fiber_info_base( std::size_t);

	virtual ~fiber_info_base();

	virtual void run() = 0;

    inline friend void intrusive_ptr_add_ref( fiber_info_base * p)
    { ++p->use_count; }

    inline friend void intrusive_ptr_release( fiber_info_base * p)
    { if ( --p->use_count == 0) delete p; }
};

}}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_FIBER_INFO_WINDOWS_H
