/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(BOOST_FUSION_TUPLE_ELEMENT_AT_IPP)
#define BOOST_FUSION_TUPLE_ELEMENT_AT_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

    namespace tuple_detail
    {
        template <unsigned N, typename TupleT>
        class get_result
        {
            typedef typename tuple_element<N, TupleT>::type t;

        public:

            typedef typename add_reference<t>::type type;
        };

        template <unsigned N, typename TupleT>
        class const_get_result
        {
            typedef typename tuple_element<N, TupleT>::type t;
            typedef typename add_const<t>::type ct;

        public:

            typedef typename add_reference<ct>::type type;
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The following Boost::PP code generate expansions for:
    //
    //      struct element_at_N
    //      {
    //          template <typename T0... typename TM>
    //          static typename element<N, tuple<T0... TM> >::const_reference
    //          get(tuple<T0... TM> const& t) { return t.data.mN; }
    //
    //          template <typename T0... typename TM>
    //          static typename element<N, tuple<T0... TM> >::reference
    //          get(tuple<T0... TM>& t) { return t.data.mN; }
    //      };
    //
    //  where N runs from 0 to BOOST_FUSION_MAX_ARITY
    //  and M = BOOST_FUSION_MAX_ARITY
    //
    ///////////////////////////////////////////////////////////////////////////
    namespace tuple_detail
    {
        #define BOOST_PP_ITERATION_PARAMS_1                                     \
            (3, (0, BOOST_FUSION_MAX_ARITY,                                     \
            "boost/spirit/fusion/detail/element_at.ipp"))
        #include BOOST_PP_ITERATE()
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The following Boost::PP code generates:
    //
    //      typename mpl::at_c<
    //          mpl::vectorMAX<
    //              tuple_detail::element_at_0,
    //              tuple_detail::element_at_1,
    //              ...
    //              tuple_detail::element_at_MAX>,
    //          N
    //      >::type
    //
    //  where MAX = BOOST_FUSION_MAX_ARITY+1
    //
    ///////////////////////////////////////////////////////////////////////////
    #define BOOST_FUSION_TUPLE_ELEMENT_AT_N_PARAM(z, n, data)                   \
        BOOST_PP_CAT(tuple_detail::element_at_, n)
    #define BOOST_FUSION_TUPLE_ELEMENT_AT_N                                     \
        typename mpl::at_c<                                                     \
            BOOST_PP_CAT(                                                       \
                mpl::vector,                                                    \
                BOOST_PP_INC(BOOST_FUSION_MAX_ARITY))                           \
                <BOOST_PP_ENUM(                                                 \
                    BOOST_PP_INC(BOOST_FUSION_MAX_ARITY),                       \
                    BOOST_FUSION_TUPLE_ELEMENT_AT_N_PARAM, _)>,                 \
            N                                                                   \
        >::type                                                                 \

    namespace tuple_detail
    {
        template <unsigned N>
        struct element_at_
        {
            typedef BOOST_FUSION_TUPLE_ELEMENT_AT_N type;
        };
    }

#undef BOOST_FUSION_TUPLE_ELEMENT_AT_N_PARAM
#undef BOOST_FUSION_TUPLE_ELEMENT_AT_N
#endif // defined(BOOST_FUSION_TUPLE_ELEMENT_AT_IPP)

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    struct BOOST_PP_CAT(element_at_, N)
    {
        template <BOOST_FUSION_TEMPLATE_PARAMS(BOOST_FUSION_MAX_ARITY, T)>
        static
        typename tuple_detail::
            const_get_result<N, tuple<BOOST_FUSION_TEMPLATE_ARGS(T)> >::type
        get(tuple<BOOST_FUSION_TEMPLATE_ARGS(T)> const& t)
        {
            return t.data.BOOST_PP_CAT(m, N);
        }

        template <BOOST_FUSION_TEMPLATE_PARAMS(BOOST_FUSION_MAX_ARITY, T)>
        static
        typename tuple_detail::
            get_result<N, tuple<BOOST_FUSION_TEMPLATE_ARGS(T)> >::type
        get(tuple<BOOST_FUSION_TEMPLATE_ARGS(T)>& t)
        {
            return t.data.BOOST_PP_CAT(m, N);
        }
    };

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)
