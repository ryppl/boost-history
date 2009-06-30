/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVERT_HPP
#define BOOST_FUSION_SEQUENCE_CONVERT_HPP

namespace boost { namespace fusion
{
    namespace extension
    {
        template <typename Tag>
        struct convert_impl;
    }

    namespace result_of
    {
        template <typename Tag, typename Sequence>
        struct convert
        {
            typedef typename extension::convert_impl<Tag> gen;

            typedef typename gen::
                template apply<typename detail::add_lref<Sequence>::type>::type
            type;
        };
    }

    template <typename Tag, typename Sequence>
    inline typename result_of::convert<
        Tag
      , BOOST_FUSION_R_ELSE_CLREF(Sequence)>::type
    convert(BOOST_FUSION_R_ELSE_CLREF(Sequence) seq)
    {
        typedef typename result_of::convert<
            Tag
          , BOOST_FUSION_R_ELSE_CLREF(Sequence)>::gen
        gen;
        return gen::call(seq);
    }

}}

#endif
