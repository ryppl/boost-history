// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_LESS_HPP
#define BOOST_METAMATH_DOUBLE_LESS_HPP

#include <boost/mpl/less.hpp>
#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/metamath/double/limits.hpp>
#include <boost/detail/workaround.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<typename N1,typename N2>
        struct less_double_double {
            BOOST_STATIC_CONSTANT(bool,is_zero1=(is_zero<N1>::value));
            BOOST_STATIC_CONSTANT(bool,is_zero2=(is_zero<N2>::value));
            BOOST_STATIC_CONSTANT(bool,both_negative=(N1::sign && N2::sign));
            BOOST_STATIC_CONSTANT(bool,compare_positive=
                (
                (N1::exponent!=N2::exponent)
                    ?(N1::exponent<N2::exponent)
                    :(N1::mantissa::part1!=N2::mantissa::part1)
                        ?(N1::mantissa::part1<N2::mantissa::part1)
                        :(N1::mantissa::part2!=N2::mantissa::part2)
                            ?(N1::mantissa::part2<N2::mantissa::part2)
                            :false
                )
            );
            BOOST_STATIC_CONSTANT(bool,value=
                (
                (is_zero1&&is_zero2)?false:
                (N1::sign && !N2::sign)
                    ?true
                    :(!N1::sign && N2::sign)
                        ?false
                        :compare_positive^both_negative
                )
            );
            typedef mpl::bool_<value> type;
        };
    }//namespace detail
}} //namespace boost::metamath

namespace boost { namespace mpl {
   template<>
   struct less_impl< 
         metamath::double_c_tag
        ,metamath::double_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
   >
   {
       template< typename N1, typename N2 > struct apply 
           : metamath::detail::less_double_double<N1,N2>::type
       {};
   };

   template<>
   struct less_impl< 
         metamath::double_c_tag
        ,integral_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
   >
   {
       template< typename N1, typename N2 > struct apply 
           : metamath::detail::less_double_double<N1,metamath::integral_to_double<N2> >::type
       {};
   };

   template<>
   struct less_impl< 
        integral_c_tag
        ,metamath::double_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
    >
   {
       template< typename N1, typename N2 > struct apply
           : metamath::detail::less_double_double<metamath::integral_to_double<N1>,N2>::type
       {};
   };
}}

#endif //BOOST_METAMATH_DOUBLE_LESS_HPP
