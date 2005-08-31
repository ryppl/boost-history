// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_TANGENT_HPP
#define BOOST_METAMATH_DOUBLE_TANGENT_HPP

#include <boost/metamath/double.hpp>
#include <boost/metamath/double/times.hpp>
#include <boost/metamath/double/divides.hpp>
#include <boost/metamath/double/plus.hpp>
#include <boost/metamath/double/minus.hpp>
#include <boost/metamath/double/constants.hpp>
#include <boost/metamath/double/bernoulli.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/metamath/double/equal_to.hpp>

namespace boost { namespace metamath {
    namespace detail {
        /*
         * Compile-time Taylor series.
         */
        template <
            typename Angle2,
            typename Above,
            typename Below,
            typename I,
            typename SeriesCount
        >
        struct tangent_series
        {
        private:
            typedef double_<mantissa<0x40000000,0>,2*I::value+1,true> two_to_twoI;
            typedef typename divides_double_double<
                typename times_double_double<
                    typename times_double_double<
                        typename times_double_double<
                            two_to_twoI,
                            typename minus_double_double<two_to_twoI,long_to_double<1> >::type
                        >::type,
                        meta::B<I::value>
                    >::type,
                    Above
                >::type,
                Below
            >::type term;
            typedef typename times_double_double<long_to_double<2>,integral_to_double<I> >::type I_two;
            typedef typename mpl::eval_if<
                              mpl::equal_to<mpl::plus<mpl::int_<1>,I>,SeriesCount>
                            , long_to_double<0>
                            , tangent_series<
                                Angle2,
                                typename times_double_double<Above,Angle2>::type,
                                typename times_double_double<
                                    typename times_double_double<
                                        typename plus_double_double<I_two,long_to_double<1> >::type,
                                        typename plus_double_double<I_two,long_to_double<2> >::type
                                    >::type,
                                    Below
                                >::type,
                                mpl::plus<mpl::int_<1>,I>,
                                SeriesCount
                            >
                            >::type
                next_term;
        public:
            typedef typename plus_double_double<term,next_term>::type type;
        };

        template<typename Angle>
        struct tangent_series_start {
            typedef typename times_double_double<Angle,Angle>::type square_angle;
            typedef typename tangent_series<square_angle,Angle,long_to_double<2>,mpl::int_<1>,mpl::int_<10> >::type type;
        };

    }  // namespace aux

    template<typename Tag>
    struct tangent_impl;

    template <>
    struct tangent_impl<double_c_tag>
    {
        template<typename Angle>
        struct apply : detail::round<typename detail::tangent_series_start<Angle>::type>::type
        {
        };
    };

    template <>
    struct tangent_impl<mpl::integral_c_tag>
    {
        template<typename Angle>
        struct apply : detail::round<typename detail::tangent_series_start<integral_to_double<Angle> >::type>::type
        {
        };
    };

}}  //namespace boost::metamath

#endif