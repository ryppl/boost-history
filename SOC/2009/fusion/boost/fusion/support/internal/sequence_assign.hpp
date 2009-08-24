/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_SEQUENCE_ASSIGN_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_SEQUENCE_ASSIGN_HPP

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename Seq>
        struct sequence_assign_type
        {
            typedef Seq seq_type;

            sequence_assign_type(Seq seq)
              : seq(seq)
            {}

            Seq get()const
            {
                return seq;
            }

        private:
            Seq seq;
        };
    }

    template<typename Seq>
    detail::sequence_assign_type<BOOST_FUSION_R_ELSE_CLREF(Seq)>
    sequence_assign(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));

        return seq;
    }
}}

#endif
