/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_FILTER_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_FILTER_VIEW_ITERATOR_HPP

#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/algorithm/query/detail/find_if.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion
{
    struct filter_view_iterator_tag;

    template<
        typename Category
      , typename Begin
      , typename End
      , typename Pred
      , typename PredIsMetafunction
    >
    struct filter_view_iterator
      : iterator_base<
            filter_view_iterator<Category, Begin, End, Pred,PredIsMetafunction>
        >
    {
        typedef PredIsMetafunction pred_is_metafunction;
        typedef Pred pred_type;
        typedef
            detail::static_find_if<
                Begin
              , End
              , mpl::bind1<
                    typename mpl::eval_if_c<
                        pred_is_metafunction::value
                      , mpl::lambda<Pred>
                      , mpl::identity<Pred>
                    >::type
                  , mpl::bind1<mpl::quote1<result_of::value_of>,mpl::_1>
                >
            >
        filter;
        typedef typename filter::type begin_type;
        typedef End end_type;

        typedef filter_view_iterator_tag fusion_tag;
        typedef Category category;

        template<typename OtherIt>
        filter_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : first(BOOST_FUSION_FORWARD(OtherIt,it).first)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,filter_view_iterator_tag);
        }

        filter_view_iterator(Begin const& first,int)
          : first(filter::call(first))
        {}

        template<typename OtherIt>
        filter_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,filter_view_iterator_tag);

            first=BOOST_FUSION_FORWARD(OtherIt,it).first;
            return *this;
        }

        begin_type first;
    };
}}

#endif
