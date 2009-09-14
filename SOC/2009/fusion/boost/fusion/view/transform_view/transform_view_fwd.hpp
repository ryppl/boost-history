/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_TRANSFORM_VIEW_FWD_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_TRANSFORM_VIEW_FWD_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct transform_view_tag;
    struct transform_view2_tag;

    template<
        typename Seq1
      , typename Seq2
      , typename F=mpl::false_
      , typename IsAssociative=mpl::false_
    >
    struct transform_view;
}}

#endif
