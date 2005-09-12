// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_SINE_HPP
#define BOOST_METAMATH_DOUBLE_SINE_HPP

#include <boost/metamath/double.hpp>
#include <boost/metamath/double/times.hpp>
#include <boost/metamath/double/divides.hpp>
#include <boost/metamath/double/plus.hpp>
#include <boost/metamath/double/minus.hpp>
#include <boost/metamath/double/constants.hpp>

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
            typename Angle
          , typename I
          , typename SeriesCount
        >
        struct sine_series
        {
         private:
             typedef typename mpl::eval_if<
                                 mpl::equal_to<mpl::plus<mpl::int_<1>,I>,SeriesCount>
                               , meta::one
                               , sine_series<Angle,mpl::plus<mpl::int_<1>,I>,SeriesCount>
                             >::type
                    next_term;

         public:
            typedef minus_double_double<
                        meta::one,
                        times_double_double<
                            divides_double_double<
                                divides_double_double<
                                    times_double_double<Angle,Angle>
                                    , plus_double_double<times_double_double<meta::two,integral_to_double<I> >,meta::two >
                                >
                                , plus_double_double<times_double_double<meta::two,integral_to_double<I> >,meta::three >
                            >
                            , next_term
                        >
                    >
                type;
        };

    }  // namespace aux

    template<typename Angle>
    struct sine_double :
      detail::times_double_double<Angle,typename detail::sine_series<Angle,mpl::int_<0>,mpl::int_<13> >::type >
    {
    };

    template<typename Tag>
    struct sine_impl;

    template <>
    struct sine_impl<double_c_tag>
    {
        template<typename Angle>
        struct apply : detail::round<sine_double<Angle> >::type
        {
        };
    };

    template <>
    struct sine_impl<mpl::integral_c_tag>
    {
        template<typename Angle>
        struct apply : detail::round<sine_double<integral_to_double<Angle> >::type
        {
        };
    };

}}  //namespace boost::metamath

#endif