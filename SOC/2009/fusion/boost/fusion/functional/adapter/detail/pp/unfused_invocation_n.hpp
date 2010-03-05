/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

        template<
            typename Self
#if BOOST_FUSION_N > 0
          , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, typename T)
#endif
        >
        struct result<Self(EXPAND_TEMPLATE_ARGUMENTS(BOOST_FUSION_N))>
          : boost::result_of<
                typename detail::get_func_base<
                    typename detail::forward_as<Self,F>::type
                >::type(BOOST_FUSION_R_ELSE_LREF(
                    BOOST_PP_IF(BOOST_FUSION_N, typename, BOOST_PP_EMPTY())
                    result_of::vector_tie<
                        EXPAND_TEMPLATE_ARGUMENTS(BOOST_FUSION_N)
                    >::type))
            >
        {};

#define BOOST_FUSION_CALL_OPERATOR(F,MODIFIER)\
        VARIADIC_TEMPLATE_A(BOOST_FUSION_N)\
        typename boost::result_of<\
            typename detail::get_func_base<F>::type(\
                BOOST_FUSION_R_ELSE_LREF(\
                    BOOST_PP_IF(BOOST_FUSION_N, typename, BOOST_PP_EMPTY())\
                    result_of::vector_tie<\
                    EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_LREF(BOOST_FUSION_N)\
                >::type))\
        >::type\
        operator()(EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_LREF(\
            BOOST_FUSION_N)) MODIFIER\
        {\
            return f(fusion::vector_tie(EXPAND_PARAMETERS_A(BOOST_FUSION_N)));\
        }

    BOOST_FUSION_CALL_OPERATOR(F,BOOST_PP_EMPTY());
    BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,const);

#undef BOOST_FUSION_CALL_OPERATOR

#undef BOOST_FUSION_N

