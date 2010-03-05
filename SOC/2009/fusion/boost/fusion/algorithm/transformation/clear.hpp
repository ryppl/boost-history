/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_CLEAR_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_CLEAR_HPP

#include <boost/fusion/sequence/convert.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/vector/vector.hpp>
#else
#   include <boost/fusion/container/vector/vector10.hpp>
#endif
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        //TODO empty sequence!!!
        template <typename Seq>
        struct clear
#ifdef BOOST_NO_VARIADIC_TEMPLATES
          : convert<typename traits::tag_of<Seq>::type, vector0<> >
#else
          : convert<typename traits::tag_of<Seq>::type, vector<> >
#endif
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
        };
    }

    template <typename Seq>
    inline typename result_of::clear<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    clear(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename
            result_of::clear<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type();
    }
}}

#endif
