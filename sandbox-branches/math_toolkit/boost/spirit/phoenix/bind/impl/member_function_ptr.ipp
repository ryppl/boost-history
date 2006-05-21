/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_BIND_IMPL_MEMBER_FUNCTION_PTR_IPP)
#define PHOENIX_BIND_IMPL_MEMBER_FUNCTION_PTR_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>
#include <boost/utility/addressof.hpp>

    namespace impl
    {
        template <int N>
        struct member_function_ptr_impl
        {
            template <typename RT, typename FP>
            struct impl;
        };

        template <int N, typename RT, typename FP>
        struct member_function_ptr
            : member_function_ptr_impl<N>::template impl<RT, FP>
        {
            typedef typename member_function_ptr_impl<N>::
                template impl<RT, FP> base;
            member_function_ptr(FP fp)
                : base(fp) {}
        };

        template <>
        struct member_function_ptr_impl<0>
        {
            template <typename RT, typename FP>
            struct impl
            {
                template <typename Class>
                struct apply
                {
                    typedef RT type;
                };

                impl(FP fp)
                    : fp(fp) {}

                template <typename Class>
                RT operator()(Class& obj) const
                {
                    return (obj.*fp)();
                }

                template <typename Class>
                RT operator()(Class* obj) const
                {
                    return (obj->*fp)();
                }

                FP fp;
            };
        };
    }

#define PHOENIX_EVAL_ARG(z, n, data)                                            \
    BOOST_PP_CAT(_, n).eval(args, env)

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (1, PHOENIX_COMPOSITE_LIMIT,                                            \
    "boost/spirit/phoenix/bind/impl/member_function_ptr.ipp"))
#include BOOST_PP_ITERATE()

#undef PHOENIX_EVAL_ARG
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    namespace impl
    {
        template <>
        struct member_function_ptr_impl<N>
        {
            template <typename RT, typename FP>
            struct impl
            {
                template <typename Class, PHOENIX_TEMPLATE(N, T)>
                struct apply
                {
                    typedef RT type;
                };

                impl(FP fp)
                    : fp(fp) {}

                template <typename Class, PHOENIX_TEMPLATE(N, A)>
                RT operator()(Class& obj, PHOENIX_REF_ARGS(N, A)) const
                {
                    return (obj.*fp)(BOOST_PP_ENUM_PARAMS(N, _));
                }

                template <typename Class, PHOENIX_TEMPLATE(N, A)>
                RT operator()(Class* obj, PHOENIX_REF_ARGS(N, A)) const
                {
                    return (obj->*fp)(BOOST_PP_ENUM_PARAMS(N, _));
                }

                FP fp;
            };
        };
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


