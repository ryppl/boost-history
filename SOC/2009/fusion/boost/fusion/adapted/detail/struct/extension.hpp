/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_EXTENSION_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_EXTENSION_HPP

namespace boost { namespace fusion
{
    struct struct_tag;

    namespace extension
    {
        struct no_such_member;

        template<typename Seq, int N>
        struct struct_member;

        template<typename Seq>
        struct struct_size;

        template<typename Seq, typename Key>
        struct struct_assoc_member
        {
            typedef no_such_member type;
        };
    }
}}

#endif
