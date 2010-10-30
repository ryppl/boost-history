/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_SEQUENCE_ASSIGN_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_SEQUENCE_ASSIGN_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename SeqRef>
        struct sequence_assign_type
        {
            typedef SeqRef seq_type;
            typedef typename
                detail::remove_reference<SeqRef>::type*
            seq_ptr_type;

            sequence_assign_type(seq_ptr_type seq)
              : seq(seq)
            {}

            SeqRef get()const
            {
                return static_cast<SeqRef>(*seq);
            }

        private:
            seq_ptr_type seq;
        };
    }

    template<typename Seq>
    detail::sequence_assign_type<BOOST_FUSION_R_ELSE_CLREF(Seq)>
    sequence_assign(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));

        return &seq;
    }
}}

#endif
