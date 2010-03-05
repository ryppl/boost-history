/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    template<typename F, typename TransformSeq>
    struct unfused_typed_impl<BOOST_FUSION_N, F, TransformSeq>
    {
        typedef typename
            result_of::as_vector<BOOST_FUSION_R_ELSE_CLREF(TransformSeq)>::type
        transformed_args;
        typedef BOOST_FUSION_R_ELSE_LREF(transformed_args) transformed_args_arg;

        template<typename Sig>
        struct result;

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
                >::type(transformed_args_arg)
            >
        {};

#ifdef BOOST_NO_RVALUE_REFERENCES
#   define BOOST_FUSION_CALL_PARAMETER(Z,N,__)\
    typename call_traits<\
        typename result_of::value_at_c<transformed_args,N>::type\
    >::param_type BOOST_PP_CAT(_,N)

#   define BOOST_FUSION_CALL_OPERATOR(F_,MODIFIER)\
        typename boost::result_of<\
            typename detail::get_func_base<F_>::type(transformed_args_arg)\
        >::type\
        operator()(BOOST_PP_ENUM(BOOST_FUSION_N,BOOST_FUSION_CALL_PARAMETER,_))\
            MODIFIER\
        {\
            return static_cast<unfused_typed<F,TransformSeq> MODIFIER&>(*this).\
                f(fusion::as_vector(TransformSeq(\
                    EXPAND_PARAMETERS_A(BOOST_FUSION_N))));\
        }
#else
#   define BOOST_FUSION_CALL_OPERATOR(F_,MODIFIER)\
        BOOST_FUSION_VARIADIC_TEMPLATES_A(BOOST_FUSION_N)\
        typename boost::result_of<\
            typename detail::get_func_base<F_>::type(transformed_args_arg)\
        >::type\
        operator()(EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_LREF(BOOST_FUSION_N))\
            MODIFIER\
        {\
            return static_cast<unfused_typed<F,TransformSeq> MODIFIER&>(*this).\
                f(fusion::as_vector(TransformSeq(\
                    EXPAND_PARAMETERS_A(BOOST_FUSION_N))));\
        }
#endif

        BOOST_FUSION_CALL_OPERATOR(F,BOOST_PP_EMPTY());
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,const);

#ifdef BOOST_NO_RVALUE_REFERENCES
#   undef BOOST_FUSION_CALL_PARAMETER
#endif
#undef BOOST_FUSION_CALL_OPERATOR
    };

#undef BOOST_FUSION_N

