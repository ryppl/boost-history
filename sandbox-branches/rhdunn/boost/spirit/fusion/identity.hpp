/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TUPLE_IDENTITY_IPP)
#define BOOST_FUSION_TUPLE_IDENTITY_IPP

#define BOOST_FUSION_TUPLE_IDENTITY_PARAM(z, n, data)  \
    typename id_type<BOOST_PP_CAT(T, n)>::type

    ///////////////////////////////////////////////////////////////////////////
    //
    //  tuple_identity
    //
    //      Given a list of types T0..TN, where each element can either be
    //      void_t or an arbitrary type, the objective is to create a unique
    //      type based on the number of non-void_t template parameters passed
    //      in.
    //
    //      The simplest scheme is to actually count the number of template
    //      parameters from T0 until we get a void_t. However, this not so
    //      compile-time efficient. The scheme presented simply creates a
    //      type where all non-void_t template parameters are converted to
    //      struct non_void_t. For example:
    //
    //          tuple_identity<int, char, double, void_t, void_t>::type
    //
    //      is:
    //
    //          tuple_identity_types<
    //              non_void_t, non_void_t, non_void_t, void_t, void_t>
    //
    ///////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_FUSION_TUPLE_DISABLE_ID)

    namespace tuple_detail
    {
        struct non_void_t {};

        template <typename T>
        struct id_type
        { typedef non_void_t type; };

        template <>
        struct id_type<void_t>
        { typedef void_t type; };

        template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)>
        struct tuple_identity_types {};

        template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)>
        struct tuple_identity
        {
            typedef tuple_identity_types<
                BOOST_PP_ENUM(
                    BOOST_FUSION_MAX_ARITY
                  , BOOST_FUSION_TUPLE_IDENTITY_PARAM, _)> type;
        };
    }

#define BOOST_FUSION_TUPLE_ID                                                   \
    BOOST_DEDUCED_TYPENAME                                                      \
        tuple_detail::tuple_identity<BOOST_FUSION_TEMPLATE_ARGS(T)>::type

#undef BOOST_FUSION_TUPLE_IDENTITY_PARAM

#else // defined(BOOST_FUSION_TUPLE_DISABLE_ID)
    namespace tuple_detail
    {
        struct id;
    }

#define BOOST_FUSION_TUPLE_ID tuple_detail::id
#endif

#endif // defined(BOOST_FUSION_TUPLE_IDENTITY_IPP)
