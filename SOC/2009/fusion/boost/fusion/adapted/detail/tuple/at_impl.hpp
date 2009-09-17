/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template <>
    struct at_impl<BOOST_FUSION_ADAPTED_TUPLE_TAG>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    Seq
                  , typename BOOST_FUSION_ADAPTED_TUPLE_NAMESPACE(element)<
                        N::value
                      , typename detail::identity<Seq>::type
                    >::type
                >::type
            type;

            static type
            call(Seq seq)
            {
                return
#ifdef BOOST_FUSION_ADAPTED_STD_TUPLE
#   ifdef BOOST_NO_0X_HDR_TUPLE
                    std::tr1::get<N::value>(
#   else
                    std::get<N::value>(
#   endif
#else
                    tuples::get<N::value>(
#endif
                        BOOST_FUSION_FORWARD(Seq,seq));
            }
        };
    };
}}}

