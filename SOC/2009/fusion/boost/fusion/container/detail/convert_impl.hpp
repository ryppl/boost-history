/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || 1
#   include <boost/fusion/sequence/intrinsic/begin.hpp>
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#endif

#include <boost/preprocessor/cat.hpp>

//cschmidt: mpl's maximum supported arity is set to 5 by default. We need more
//for the variadic templates implementations though. Therefore we use the pp
//implementation for now.
#if defined(BOOST_NO_VARIADIC_TEMPLATES) || 1
#   include <boost/fusion/container/detail/pp/as_seq.hpp>
#else
#   include <boost/fusion/container/detail/variadic_templates/as_seq.hpp>
#endif

namespace boost { namespace fusion
{
    namespace extension
    {
        template<typename T>
        struct convert_impl;

        template<>
        struct convert_impl<BOOST_PP_CAT(BOOST_FUSION_SEQ_NAME,_tag)>
        {
            template<typename Seq>
            struct apply
            {
#if defined(BOOST_NO_VARIADIC_TEMPLATES) || 1
                typedef typename
                    detail::BOOST_PP_CAT(
                        BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME),_impl)<
                        result_of::size<Seq>::value
                    >::template apply<
                        typename result_of::begin<Seq>::type
                    >::type
                type;
#else
              typedef typename
                  detail::BOOST_PP_CAT(
                      BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME),_impl)<
                      Seq
                  >::type
              type;
#endif

                static type
                call(Seq seq)
                {
                    return type(fusion::sequence_assign(
                        BOOST_FUSION_FORWARD(Seq,seq)));
                }
            };
        };
    }

    namespace result_of
    {
        template<typename Seq>
        struct BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)
          : extension::convert_impl<
                BOOST_PP_CAT(BOOST_FUSION_SEQ_NAME,_tag)
            >::template apply<Seq>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))
        };
    }

    template<typename Seq>
    inline typename
        result_of::BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
        >::type
    BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return result_of::BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
        >::call(BOOST_FUSION_FORWARD(Seq,seq));
    }
}}
