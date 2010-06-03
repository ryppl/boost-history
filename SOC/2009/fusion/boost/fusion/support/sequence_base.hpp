/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Tobias Schwinger

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_SEQUENCE_BASE_HPP
#define BOOST_FUSION_SUPPORT_SEQUENCE_BASE_HPP

#include <boost/mpl/begin_end_fwd.hpp>

namespace boost
{
    namespace fusion
    {
        struct sequence_root
        {};

        template<typename Seq>
        struct sequence_base
          : sequence_root
        {
            Seq const&
            derived() const
            {
                return static_cast<Seq const&>(*this);
            }

            Seq&
            derived()
            {
                return static_cast<Seq&>(*this);
            }
        };

        struct fusion_sequence_tag;
    }

    namespace mpl
    {
        // Deliberately break mpl::begin, so it doesn't lie that a Fusion
        // sequence is not an MPL sequence by returning mpl::void_.
        // In other words: Fusion Sequences are always MPL Sequences, but they
        // can be incompletely defined.
        template<>
        struct begin_impl<fusion::fusion_sequence_tag>;
    }
}

#endif
