// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/sequence/intrinsic/begin.hpp>
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#endif
#include <boost/fusion/support/ref.hpp>

#include <boost/preprocessor/cat.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/detail/pp/as_seq.hpp>
#else

#   include <boost/fusion/container/detail/variadic_templates/as_seq.hpp>
#endif

namespace boost { namespace fusion
{
    namespace extension
    {
        template <typename T>
        struct convert_impl;

        template <>
        struct convert_impl<BOOST_PP_CAT(BOOST_FUSION_SEQ_NAME,_tag)>
        {
            template <typename SeqRef>
            struct apply
            {
#ifdef BOOST_NO_VARIADIC_TEMPLATES
                typedef typename
                    detail::BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)<
                        result_of::size<SeqRef>::value
                    >
                gen;
                typedef typename
                    gen::template apply<
                        typename result_of::begin<SeqRef>::type
                    >::type
                type;
#else
                typedef typename
                    detail::BOOST_PP_CAT(
                        BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME),_impl)<SeqRef>
                gen;
                typedef typename gen::apply::type type;
#endif

                static type call(SeqRef seq)
                {
                    return type(fusion::sequence_assign(
                            BOOST_FUSION_FORWARD(SeqRef,seq)));
                }
            };
        };
    }

    namespace result_of
    {
        template <typename Seq>
        struct BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)
        {
            typedef typename
                extension::convert_impl<
                    BOOST_PP_CAT(BOOST_FUSION_SEQ_NAME,_tag)
                >::template apply<typename detail::add_lref<Seq>::type>
            gen;

            typedef typename gen::type type;
        };
    }

    template <typename Seq>
    inline typename
        result_of::BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
        >::type
    BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return
            result_of::BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME)<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
            >::gen::call(seq);
    }

}}
