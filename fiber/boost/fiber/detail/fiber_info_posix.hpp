
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_DETAIL_FIBER_INFO_BASE_POSIX_H
#define BOOST_FIBER_DETAIL_FIBER_INFO_BASE_POSIX_H

extern "C" {

#include <ucontext.h>

}

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {
namespace detail {

struct BOOST_FIBER_DECL fiber_info_base
{
	typedef shared_ptr< fiber_info_base >	sptr_t;

	attributes				attribs;
	::ucontext_t			uctx;
	shared_array< char >	uctx_stack;

	fiber_info_base();

	fiber_info_base( attributes const& attribs_);

	virtual ~fiber_info_base() {}

	virtual void run() = 0;
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBER_DETAIL_FIBER_INFO_BASE_POSIX_H
