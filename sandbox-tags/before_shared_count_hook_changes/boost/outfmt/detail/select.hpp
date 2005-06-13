// (C) Copyright Jonathan Turkanis 2003-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Contains 
//      (1) the metafunction select, which mimics the effect of a chain
//          of nested mpl if_'s or apply_if's, and 
//      (2) the template eval, for use with select.
//
// -----------------------------------------------------------------------------
//
// Usage:
//      
// typedef typename select<
//                      case1,  type1,
//                      case2,  type2,
//                      ...
//                      true_,  typen
//                  >::type selection;
//
// Here case1, case2, ... are models of MPL::IntegralConstant with value type
// bool, and n <= 10 (the maxiumum number of cases used by this library is 8).
// 
// To get the effect of mpl::apply_if for a given type, wrap it with the 
// template eval. E.g.:
//
// typedef typename select<
//                      case1,  type1,
//                      case2,  type2,
//                      case3,  eval<type3>,
//                      ...
//                      true_,  typen
//                  >::type selection;
//                      
// Here type3 is a metafunction which will be applied only if it is selected.
//
// Note: select could be implemented in terms of mpl::swicth_; the present
// implementation is no more verbose, however.
//

#ifndef BOOST_IO_SELECT_HPP_INCLUDED
#define BOOST_IO_SELECT_HPP_INCLUDED
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif
 
#include <boost/type_traits.hpp>            // is_base_and_derived
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/void.hpp>

namespace boost { namespace io { 

namespace detail { 
    struct eval_base { }; 

    // Returns the given type, unless is is of the form eval<U>, in which case
    // returns the result of applying U.
    template<typename T>
    struct do_eval {
        template<typename U> struct apply_base_type {
            typedef typename U::base::type type;
        };
        typedef typename mpl::apply_if<
                             is_base_and_derived<detail::eval_base, T>,
                             apply_base_type<T>,
                             mpl::identity<T>
                         >::type type;
    };
}               // End namespace detail.

template<typename T>
struct eval : public detail::eval_base { typedef T base; };

namespace detail { 
    // Unspecified types default to void (VC6 requires this circumlocution.)
    typedef eval< mpl::identity<void> > select_default; 
}

template< typename Case1 = mpl::true_,
          typename Type1 = detail::select_default,
          typename Case2 = mpl::true_,
          typename Type2 = detail::select_default,
          typename Case3 = mpl::true_,
          typename Type3 = detail::select_default,
          typename Case4 = mpl::true_,
          typename Type4 = detail::select_default,
          typename Case5 = mpl::true_,
          typename Type5 = detail::select_default,
          typename Case6 = mpl::true_,
          typename Type6 = detail::select_default,
          typename Case7 = mpl::true_,
          typename Type7 = detail::select_default,
          typename Case8 = mpl::true_,
          typename Type8 = detail::select_default,
          typename Case9 = mpl::true_,
          typename Type9 = detail::select_default,
          typename Case10 = mpl::true_,
          typename Type10 = detail::select_default >
struct select {
    typedef typename
            mpl::if_<
                Case1, Type1, typename mpl::if_<
                Case2, Type2, typename mpl::if_<
                Case3, Type3, typename mpl::if_<
                Case3, Type3, typename mpl::if_<
                Case4, Type4, typename mpl::if_<
                Case5, Type5, typename mpl::if_<
                Case6, Type6, typename mpl::if_<
                Case7, Type7, typename mpl::if_<
                Case8, Type8, typename mpl::if_<
                Case9, Type9, typename mpl::if_<
                Case10, Type10, mpl::void_
                >::type
                >::type
                >::type
                >::type
                >::type
                >::type
                >::type
                >::type
                >::type
                >::type
            >::type                               temp;
    typedef typename detail::do_eval<temp>::type  type;
};

} }             // End namespaces io, boost.

#endif          // #ifndef BOOST_IO_SELECT_HPP_INCLUDED
