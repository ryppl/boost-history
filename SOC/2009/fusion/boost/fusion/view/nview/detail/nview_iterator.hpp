/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_DETAIL_NVIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_NVIEW_DETAIL_NVIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    struct nview_iterator_tag;

    template<typename SeqRef, typename Pos>
    struct nview_iterator
      : iterator_base<nview_iterator<SeqRef, Pos> >
    {
        typedef SeqRef seq_type;
        typedef Pos pos_type;

        typedef nview_iterator_tag fusion_tag;
        typedef random_access_traversal_tag category;

        template<typename OtherIt>
        nview_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : seq(BOOST_FUSION_FORWARD(OtherIt,it).seq)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,nview_iterator_tag);
            BOOST_FUSION_MPL_ASSERT((
                is_same<
                    Pos
                  , detail::remove_reference<OtherIt>::type::pos_type
                >));
        }

        nview_iterator(SeqRef seq,int)
          : seq(seq)
        {}

        template<typename OtherIt>
        nview_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,nview_iterator_tag);
            BOOST_FUSION_MPL_ASSERT((
                is_same<
                    Pos
                  , detail::remove_reference<OtherIt>::type::pos_type
                >));

            seq=BOOST_FUSION_FORWARD(OtherIt,it).seq;
            return *this;
        }

        SeqRef seq;
    };

}}

#endif


