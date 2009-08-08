// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_TO_RANDOM_ACCESS_MPL_SEQ_HPP
#define BOOST_FUSION_SUPPORT_TO_RANDOM_ACCESS_MPL_SEQ_HPP

#include <boost/fusion/mpl.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<
        typename Seq
      , bool random_access=traits::is_random_access<Seq>::value
    >
    struct to_random_access_mpl_seq;

    template<typename Seq>
    struct to_random_access_mpl_seq<Seq,true>
    {
        typedef Seq type;
    };

    template<typename Seq>
    struct to_random_access_mpl_seq<Seq,false>
    {
        typedef typename
            mpl::fold<
                Seq
              , mpl::vector0<>
              , mpl::push_back<mpl::_1, mpl::_2>
            >::type
         type;
    };
}}}

#endif
