/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_TRANSFORM_VIEW_FWD_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_TRANSFORM_VIEW_FWD_HPP

namespace boost { namespace fusion
{
    struct void_;
    struct transform_view_tag;
    struct transform_view2_tag;

    template <typename A, typename B, typename C = void_>
    struct transform_view;
}}

#endif
