// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_GENERATE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_GENERATE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <utility> // pair.
#include <boost/interfaces/access.hpp>
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/interfaces/detail/fn_ptr.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/joint_view.hpp>
//#include <boost/mpl/insert_range.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost { namespace interfaces { namespace detail {

//template<typename First, typename Second>
//struct concatenate
//    : mpl::insert_range<
//          First,
//          typename mpl::end<First>::type,
//          Second
//      >
//    { };

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
                        >::type                              G1Inst;
                typedef typename G1Inst::interface_metadata  G1Metadata;
                typedef typename 
                        G2::template
                        apply<
                            Derived, 
                            mpl::int_<
                                Offset::value + 
                                G1Metadata::size
                            >, 
                            Flags,
                            G1Inst
                        >::type                              G2Inst;
                typedef typename G2Inst::interface_metadata  G2Metadata;
                struct type : G2Inst { // The generator instance.
                    struct interface_metadata {
                        typedef Derived                               derived_type;
                        typedef Offset                                offset_type;
                        typedef Flags                                 flags_type;
                        typedef Base                                  base_type;
                        static const int size = G1Metadata::size + G2Metadata::size;
                        typedef mpl::joint_view<
                                    typename G1Metadata::signatures,
                                    typename G2Metadata::signatures
                                >                                     signatures;
                        #ifndef BOOST_IDL_NO_IMPLEMENTS
                            template<typename T> 
                            struct implements
                                : mpl::and_<
                                    typename G1Metadata::template implements<T>,
                                    typename G2Metadata::template implements<T>
                                  >
                                { };
                        #endif
                        template<typename X>
                        static void initialize(fn_ptr* table)
                        {
                            G1Metadata::template initialize<X>(table);
                            G2Metadata::template initialize<X>(table);
                        }
                        ::std::pair<const char*, fn_ptr>
                        lookup(int n, fn_ptr* table)
                        {
                            typedef ::boost::mpl::size<
                                         typename G1Metadata::signatures
                                    >::type size;
                            return (n < size::value) ?
                                G1Metadata::lookup(n, table) :
                                G2Metadata::lookup(n - size::value, table);
                        }
                    }; 
                };
            };
        };
    };
};

} } } // End namespace detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_GENERATE_HPP_INCLUDED
