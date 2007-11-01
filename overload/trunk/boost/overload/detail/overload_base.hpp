/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_DETAIL_OVERLOAD_BASE_HPP_
#define _BOOST_DETAIL_OVERLOAD_BASE_HPP_

#include <boost/function.hpp>

#include "final_overload_base.hpp"

namespace boost{ namespace overloads{ namespace detail{

///////////////////////////////////////////////////////////////////////////////
// overload_base class

template< 
    unsigned int N,
    typename Sig0, 
    BOOST_PP_ENUM_SHIFTED(BOOST_OVERLOAD_LIMIT, OVL_TEMPL_PARAMS, Sig)
>
struct overload_base
    : public boost::function<Sig0>
    , public overload_base<N+1, BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_OVERLOAD_LIMIT, Sig)>
{
    static const unsigned int index = N;

    typedef Sig0                        signature_type;
    typedef boost::function<Sig0>       function_type;
    typedef 
        overload_base<N+1, BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_OVERLOAD_LIMIT, Sig)> 
        base_type;

// Workaround for MSVC < 8.0 tested on MSVC 7.1 
#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400) 
    using function_type::operator();
    using base_type::operator();
#endif
};

template< unsigned int N, typename Sig0 >
struct overload_base<N, Sig0>
    : public boost::function<Sig0>
    , public final_overload_base
{
    static const unsigned int index = N;

    typedef Sig0                        signature_type;
    typedef boost::function<Sig0>       function_type;
    typedef final_overload_base         base_type;

// Workaround for MSVC < 8.0 tested on MSVC 7.1 
#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)  
    using function_type::operator();
#endif
};

} } } // end namespaces


#endif // _BOOST_DETAIL_OVERLOAD_BASE_HPP_

