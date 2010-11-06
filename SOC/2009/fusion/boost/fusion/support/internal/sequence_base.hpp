/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_SEQUENCE_BASE_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_SEQUENCE_BASE_HPP

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    namespace detail
    {
        struct from_sequence_convertible_type
        {};

        template<typename>
        struct sequence_base
        {
            operator from_sequence_convertible_type()const
            {
                return from_sequence_convertible_type();
            }
        };
    }
}}

#endif
