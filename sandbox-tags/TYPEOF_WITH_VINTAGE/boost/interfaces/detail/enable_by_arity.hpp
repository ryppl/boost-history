// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_ENABLE_BY_ARITY_HPP_INCLUDED
#define BOOST_IDL_DETAIL_ENABLE_BY_ARITY_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/aux_/config/ttp.hpp>
#include <boost/utility/enable_if.hpp>
#ifdef BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING
# include <boost/mpl/aux_/template_arity.hpp>
# include <boost/mpl/equal_to.hpp>
# include <boost/mpl/int.hpp>
# include <boost/utility/enable_if.hpp>
#endif

namespace boost { namespace interfaces { namespace detail {

#ifndef BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING
    template<typename T, int N>
    struct enable_by_arity { typedef void type; };
#else 
    template<typename T, int N>
    struct enable_by_arity
        : enable_if< 
             mpl::equal_to<
                 typename mpl::aux::template_arity<T>::type,
                 mpl::int_<N>
             >
          >
        { };
#endif

} } } // End namespaces detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_ENABLE_BY_ARITY_HPP_INCLUDED
