// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_SEQUENCE_ASSIGN_HPP
#define BOOST_FUSION_SUPPORT_SEQUENCE_ASSIGN_HPP

#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace boost{namespace fusion{

    namespace support
    {
        template<typename Seq>
        struct sequence_assign_type
        {
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
    support::sequence_assign_type<BOOST_FUSION_R_ELSE_CLREF(Seq)>
    sequence_assign(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return seq;
    }
}}

#endif
