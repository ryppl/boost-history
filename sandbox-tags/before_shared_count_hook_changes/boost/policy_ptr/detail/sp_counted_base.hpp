#ifndef BOOST_POLICY_PTR_DETAIL_SP_COUNTED_BASE_HPP_INCLUDED
#define BOOST_POLICY_PTR_DETAIL_SP_COUNTED_BASE_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  Copyright 2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ChangeLog:
//   2005-05-24 Larry Evans
//     WHAT:
//       1) Copied from boost/detail and then modified by adding
//          #define BOOST_SP_DISABLE_THREADS
//       2) Moved to policy_ptr/detail namespace.
//       3) Renamed all #includes to be in policy_ptr/detail.
//     WHY:
//       1) So far, only non-threaded version has been implemented
//       2) Avoid name conflict.
//       3) To anticipate future implementations of other versions.
//

#include <boost/config.hpp>

#define BOOST_SP_DISABLE_THREADS
#if defined( BOOST_SP_DISABLE_THREADS )

# include <boost/policy_ptr/detail/sp_counted_base_nt.hpp>

#elif defined( BOOST_SP_USE_PTHREADS )

# include <boost/policy_ptr/detail/sp_counted_base_pt.hpp>

#elif defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )

# include "boost/policy_ptr/detail/sp_counted_base_gcc_x86.hpp"

//~ #elif defined( __MWERKS__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )

//~ # include <boost/policy_ptr/detail/sp_counted_base_cw_x86.hpp>

#elif defined( __GNUC__ ) && defined( __ia64__ )

# include <boost/policy_ptr/detail/sp_counted_base_gcc_ia64.hpp>

#elif defined( __MWERKS__ ) && defined( __POWERPC__ )

# include <boost/policy_ptr/detail/sp_counted_base_cw_ppc.hpp>

#elif defined( __GNUC__ ) && ( defined( __powerpc__ ) || defined( __ppc__ ) )

# include <boost/policy_ptr/detail/sp_counted_base_gcc_ppc.hpp>

#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ )

# include <boost/policy_ptr/detail/sp_counted_base_w32.hpp>

#elif !defined( BOOST_HAS_THREADS )

# include <boost/policy_ptr/detail/sp_counted_base_nt.hpp>

#elif defined( BOOST_HAS_PTHREADS )

# include <boost/policy_ptr/detail/sp_counted_base_pt.hpp>

#else

// Use #define BOOST_DISABLE_THREADS to avoid the error
# error Unrecognized threading platform

#endif

#endif  // #ifndef BOOST_DETAIL_SP_COUNTED_BASE_HPP_INCLUDED
