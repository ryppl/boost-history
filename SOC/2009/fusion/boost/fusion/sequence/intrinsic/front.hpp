/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_FRONT_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_FRONT_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/deref.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    namespace result_of
    {
        template <typename Seq>
        struct front
          : result_of::deref<typename result_of::begin<Seq>::type>
        {};
    }

    template <typename Seq>
    inline typename result_of::front<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    front(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return *fusion::begin(seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq>
    inline typename result_of::front<Seq&>::type
    front(Seq& seq)
    {
        return *fusion::begin(seq);
    }
#endif
}}

#endif
