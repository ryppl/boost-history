/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_DETAIL_NVIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_NVIEW_DETAIL_NVIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>

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

        explicit
        nview_iterator(SeqRef seq)
          : seq(seq)
        {}

        SeqRef seq;
    };

}}

#endif


