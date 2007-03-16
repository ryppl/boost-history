// Copyright (C) 2005 Chris Weed
// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_EXPONENTIAL_HPP
#define BOOST_METAMATH_DOUBLE_EXPONENTIAL_HPP

#include <boost/metamath/double.hpp>
#include <boost/metamath/double/times.hpp>
#include <boost/metamath/double/divides.hpp>
#include <boost/metamath/double/plus.hpp>
#include <boost/metamath/double/minus.hpp>
#include <boost/metamath/double/constants.hpp>
#include <boost/metamath/double/power.hpp>

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
            typename Arg
          , typename I
          , typename SeriesCount
        >
        struct exponential_series
        {
         private:
             typedef typename mpl::eval_if<
                                 mpl::equal_to<mpl::plus<mpl::int_<1>,I>,SeriesCount>
                               , meta::one
                               , exponential_series<Arg,mpl::plus<mpl::int_<1>,I>,SeriesCount>
                             >::type
                    next_term;

         public:
            typedef plus_double_double<
                    meta::one,
                    times_double_double<
                        divides_double_double<Arg,integral_to_double<I> >,
                        next_term> > type;
        };

    }  // namespace detail

    template<typename Arg>
    struct exponential_double :
      detail::exponential_series<Arg,mpl::int_<1>,mpl::int_<14> >
    {
    };

    template<typename Tag>
    struct exponential_impl;

    template <>
    struct exponential_impl<double_c_tag>
    {
        template<typename Arg>
        struct apply : detail::round<typename detail::exponential_series<Arg,mpl::int_<1>,mpl::int_<18> >::type >::type
        {
        };
    };

    template <>
    struct exponential_impl<mpl::integral_c_tag>
    {
        template<typename Arg>
        struct apply : detail::round<detail::power_double_int<meta::e,Arg> >::type
        {
        };
    };

}}  //namespace boost::metamath

#endif // BOOST_METAMATH_DOUBLE_EXPONENTIAL_HPP
