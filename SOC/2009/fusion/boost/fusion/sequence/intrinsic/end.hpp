/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_END_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_END_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template<typename>
        struct end_impl;
    }

    namespace result_of
    {
        template<typename Seq>
        struct end
          : extension::end_impl<
                typename traits::tag_of<Seq>::type
            >::template apply<Seq>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))
        };
    }

    template<typename Seq>
    inline typename lazy_enable_if<
        traits::is_sequence<BOOST_FUSION_R_ELSE_CLREF(Seq)>
      , result_of::end<BOOST_FUSION_R_ELSE_CLREF(Seq)>
    >::type
    end(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return result_of::end<BOOST_FUSION_R_ELSE_CLREF(Seq)>::call(
            BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename Seq>
    inline typename lazy_enable_if<
        traits::is_sequence<Seq&>
      , result_of::end<Seq&>
    >::type
    end(Seq& seq)
    {
        return result_of::end<Seq&>::call(seq);
    }
#endif

}}

#endif
