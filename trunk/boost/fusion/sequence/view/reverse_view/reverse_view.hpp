/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_REVERSE_VIEW_07202005_0836)
#define FUSION_REVERSE_VIEW_07202005_0836

#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/sequence/view/reverse_view/reverse_view_iterator.hpp>
#include <boost/fusion/sequence/view/reverse_view/detail/begin_impl.hpp>
#include <boost/fusion/sequence/view/reverse_view/detail/end_impl.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct reverse_view_tag;
    struct fusion_sequence_tag;

    template <typename Sequence>
    struct reverse_view : sequence_base<reverse_view<Sequence> >
    {
        typedef reverse_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;

        typedef typename traits::category_of<Sequence>::type category;
        typedef typename result_of::begin<Sequence>::type first_type;
        typedef typename result_of::end<Sequence>::type last_type;
        typedef typename result_of::size<Sequence>::type size;

        BOOST_STATIC_ASSERT((
            is_base_of<
                bidirectional_traversal_tag
              , typename traits::category_of<first_type>::type>::value));

        reverse_view(Sequence& seq)
            : seq(seq)
        {}

        first_type first() const { return fusion::begin(seq); }
        last_type last() const { return fusion::end(seq); }
        typename mpl::if_<traits::is_view<Sequence>, Sequence, Sequence&>::type seq;
    };
}}

#endif


