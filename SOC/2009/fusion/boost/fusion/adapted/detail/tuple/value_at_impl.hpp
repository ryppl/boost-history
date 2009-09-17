/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct value_at_impl;

    template <>
    struct value_at_impl<BOOST_FUSION_ADAPTED_TUPLE_TAG>
    {
        template <typename Seq, typename N>
        struct apply
          : BOOST_FUSION_ADAPTED_TUPLE_NAMESPACE(element)<
                N::value
              , typename detail::identity<Seq>::type
            >
        {};
    };
}}}

