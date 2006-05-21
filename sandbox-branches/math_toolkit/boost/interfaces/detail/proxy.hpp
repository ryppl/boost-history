// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_PROXY_HPP_INCLUDED
#define BOOST_IDL_DETAIL_PROXY_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/at.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/size.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#ifndef BOOST_IDL_MAX_BASE_LIST_SIZE
# define BOOST_IDL_MAX_BASE_LIST_SIZE 5
#endif

namespace boost { namespace interfaces { namespace detail {

template<typename Sequence, int Size>
struct proxy_impl;

//------------------Definition of proxy---------------------------------------//

template<typename Sequence>
struct proxy 
    : proxy_impl< Sequence, mpl::size<Sequence>::value >
    { };

//------------------Primary definition of proxy_impl--------------------------//

struct inherit_by_proxy {
    template<typename T1, typename T2>
    struct apply {
        struct type 
            : T1::interface_metadata::proxy, 
              T2::interface_metadata::proxy
            { };
    };
};

template<typename Sequence, int Size>
struct proxy_impl {
    typedef typename 
            mpl::inherit_linearly<
                Sequence,
                inherit_by_proxy
            >::type type;
};
                    
//------------------Partial specializations of proxy_impl---------------------//

template<typename Sequence>
struct proxy_impl<Sequence, 0> { struct type { }; };

#define BOOST_IDL_DECL(z, n, seq) \
    BOOST_PP_COMMA_IF(n) mpl::at_c<seq, n>::type::interface_metadata::proxy \
    /**/
#define BOOST_PP_LOCAL_MACRO(n) \
    template<typename Sequence> \
    struct proxy_impl<Sequence, n> { \
        struct type : BOOST_PP_REPEAT(n, BOOST_IDL_DECL, Sequence) { }; \
    }; \
    /**/
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_IDL_MAX_BASE_LIST_SIZE)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_IDL_DECL
#undef BOOST_PP_LOCAL_MACRO

} } } // End namespaces detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_PROXY_HPP_INCLUDED
