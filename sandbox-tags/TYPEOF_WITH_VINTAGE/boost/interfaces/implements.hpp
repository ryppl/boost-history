// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_IMPLEMENTS_HPP_INCLUDED
#define BOOST_IDL_IMPLEMENTS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/always.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/iter_fold_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/type_traits/is_class.hpp>

namespace boost { namespace interfaces {

#ifndef BOOST_IDL_NO_IMPLEMENTS //--------------------------------------------//
namespace detail {

template<typename T, typename Interface>
struct implements_impl {
    template<typename U, typename V>
    struct implements_base {
        typedef typename V::template interface_implemented<U>::type type;
    };
    template<typename U, typename Superinterfaces>
    struct implements_derived {
        struct not_done { 
            template<typename V>
            struct apply {
                typedef typename
                        mpl::not_<
                            implements_impl<U, typename mpl::deref<V>::type >
                        >::type type;
            };
        };
        typedef typename
                mpl::iter_fold_if<
                    Superinterfaces,
                    void,              // Ignore
                    mpl::always<void>, // Ignore
                    not_done
                >::type                                           result;
        typedef typename 
                mpl::not_<
                    is_same<
                        typename mpl::second<result>::type,
                        typename mpl::end<Superinterfaces>::type
                    >
                >::type                                           type;
    };
    typedef typename Interface::interface_extends                 superinterfaces;
    typedef typename 
            mpl::eval_if<
                mpl::empty<superinterfaces>,
                implements_base<T, Interface>,
                implements_derived<T, superinterfaces>
            >::type                                               type;
    static const bool value = type::value;
};

} // End namespace detail.

template<typename T, typename Interface>
struct implements 
    : mpl::and_<
        is_class<T>,
        detail::implements_impl<T, Interface>
      >
    { };
#else // #ifndef BOOST_IDL_NO_IMPLEMENTS //-----------------------------------//
    template<typename T, typename Interface>
    struct implements : mpl::false_ { };
#endif // #ifndef BOOST_IDL_NO_IMPLEMENTS //----------------------------------//

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_IMPLEMENTS_HPP_INCLUDED
