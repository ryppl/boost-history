// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_GENERATE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_GENERATE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/access.hpp>
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/interfaces/detail/fn_ptr.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost { namespace interfaces { namespace detail {

struct generate {
    template<typename G1, typename G2>
    struct apply {
        struct type { // The generator.
            template< typename Derived, 
                      typename Offset = mpl::int_<1>, 
                      typename Flags = flags::default_, 
                      typename Base = empty_base >
            struct apply {
                typedef typename 
                        G1::template
                        apply<
                            Derived, Offset, Flags, Base
                        >::type G1Inst;
                typedef typename 
                        G2::template
                        apply<
                            Derived, 
                            mpl::int_<
                                Offset::value + 
                                G1Inst::interface_initializer::size
                            >, 
                            Flags,
                            G1Inst
                        >::type G2Inst;
                struct type : G2Inst { // The generator instance.
                    typedef typename G2Inst::interface_advice interface_advice;
                    struct interface_initializer {
                        static const int size = 
                            G1Inst::interface_initializer::size + 
                            G2Inst::interface_initializer::size;
                        template<typename T>
                        static void initialize(fn_ptr* fns)
                        {
                            G1Inst::interface_initializer::template initialize<T>(fns);
                            G2Inst::interface_initializer::template initialize<T>(fns);
                        }
                    };
                #ifndef BOOST_IDL_NO_IMPLEMENTS
                    template<typename T> 
                    struct interface_implemented 
                        : mpl::and_<
                             typename G1Inst::template interface_implemented<T>,
                             typename G2Inst::template interface_implemented<T>
                          >
                        { };
                #endif
                };
            };
        };
    };
};

} } } // End namespace detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_GENERATE_HPP_INCLUDED
