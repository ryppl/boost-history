
#ifndef BOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED
#define BOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/gcc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

// agurt, 25/apr/04: technically, the ADL workaround is only needed for GCC,
// but putting everything expect public, user-specializable metafunctions into
// a separate global namespace has a nice side effect of reducing the length 
// of template instantiation symbols, so we apply the workaround on all 
// platforms that can handle it

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) \
    || BOOST_WORKAROUND(__BORLANDC__, < 0x600) \
    || BOOST_WORKAROUND(__DMC__, BOOST_TESTED_AT(0x840)) \
    
#   define BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE boost::mpl
#   define BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN namespace boost { namespace mpl {
#   define BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE }}

#else

#   define BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE mpl_
#   define BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN namespace mpl_ { namespace aux {}
#   define BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE \
    } namespace boost { namespace mpl { using namespace mpl_; \
        namespace aux { using namespace mpl_::aux; } }} \
    /**/

#endif

#if BOOST_WORKAROUND(BOOST_MPL_CFG_GCC, <= 0x0295)
#   define BOOST_MPL_AUX_ADL_BARRIER_DECL(type) \
    namespace boost { namespace mpl { using ::mpl_::type; } } \
/**/
#else
#   define BOOST_MPL_AUX_ADL_BARRIER_DECL(type) /**/
#endif

#endif // BOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED
