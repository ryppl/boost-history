/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DETAIL_IS_IN_SEQ_HPP
#define BOOST_FUSION_SUPPORT_DETAIL_IS_IN_SEQ_HPP

#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/mpl/bool.hpp>
#   include <boost/mpl/identity.hpp>
#   include <boost/mpl/eval_if.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename It, typename Begin,typename End>
    struct next_is_in_range;

    template<typename It, typename Begin,typename End>
    struct is_in_range
      : mpl::eval_if<
            result_of::equal_to<It,Begin>
          , mpl::identity<mpl::true_>
          , mpl::eval_if<
                result_of::equal_to<Begin,End>
              , mpl::identity<mpl::false_>
              , next_is_in_range<It,Begin,End>
            >
        >::type
    {};

    template<typename It, typename Begin,typename End>
    struct next_is_in_range
      : is_in_range<It,typename result_of::next<Begin>::type,End>
    {};

    template<typename It, typename Seq>
    struct is_in_seq
      : is_in_range<
            It
          , typename result_of::begin<Seq>::type
          , typename result_of::end<Seq>::type
        >
    {};
}}}

#endif
#endif
