
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_FIBER_INFO_POSIX_H
#define BOOST_FIBERS_DETAIL_FIBER_INFO_POSIX_H

extern "C" {

#include <ucontext.h>

}

#include <boost/cstdint.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_array.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_state.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {
namespace detail {

struct BOOST_FIBER_DECL fiber_info_base
{
	typedef intrusive_ptr< fiber_info_base >	ptr_t;	

	uint32_t				use_count;
	attributes				attrs;
	::ucontext_t			uctx;
	shared_array< char >	uctx_stack;
	fiber_state_t			state;

	fiber_info_base();

	fiber_info_base( attributes const&);

	virtual ~fiber_info_base() {}

	virtual void run() = 0;

    inline friend void intrusive_ptr_add_ref( fiber_info_base * p)
    { ++p->use_count; }

    inline friend void intrusive_ptr_release( fiber_info_base * p)
    { if ( --p->use_count == 0) delete p; }
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_FIBER_INFO_POSIX_H
