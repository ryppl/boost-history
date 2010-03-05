/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PP_IS_ITERATING
#   include <boost/fusion/container/vector/vector.hpp>

#   include <boost/preprocessor/iteration/iterate.hpp>
#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/repetition/enum.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>
#   include <boost/preprocessor/repetition/enum_binary_params.hpp>
#   include <boost/call_traits.hpp>

#   define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/variadic_templates/vector_n.hpp>
#   define BOOST_PP_ITERATION_LIMITS (BOOST_FUSION_FROM, BOOST_FUSION_TO)
#   include BOOST_PP_ITERATE()

#else
#   define BOOST_FUSION_N BOOST_PP_ITERATION()

    //cschmidt: template typedefs if possible...
namespace boost { namespace fusion
{
#   if !BOOST_FUSION_N
    struct void_;

    template <typename T0=void_>
    struct vector0;

    template<>
    struct vector0<void_>
#   else
    template <BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, typename T)>
    struct BOOST_PP_CAT(vector, BOOST_FUSION_N)
#   endif
      : vector<BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, T)>
    {
        typedef vector<BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, T)> base_type;

        BOOST_PP_CAT(vector, BOOST_FUSION_N)()
        {}

#   define BOOST_FUSION_VECTOR_CTOR(MODIFIER,_)\
        BOOST_PP_CAT(vector, BOOST_FUSION_N)(\
            BOOST_PP_CAT(vector, BOOST_FUSION_N) MODIFIER vec)\
          : base_type(sequence_assign(BOOST_FUSION_FORWARD(\
                  BOOST_PP_CAT(vector, BOOST_FUSION_N) MODIFIER,vec)))\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_VECTOR_CTOR,_)

#   undef BOOST_FUSION_VECTOR_CTOR

        template<typename Arg>
        BOOST_PP_CAT(vector, BOOST_FUSION_N)(BOOST_FUSION_R_ELSE_CLREF(Arg) arg)
          : base_type(BOOST_FUSION_FORWARD(Arg,arg))
        {}

#   if BOOST_FUSION_N > 1
#       ifdef BOOST_NO_RVALUE_REFERENCES
        BOOST_PP_CAT(vector, BOOST_FUSION_N)(
            BOOST_PP_ENUM_BINARY_PARAMS(N, call_traits<T,>::param_type a))
          : base_type(BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, a))
        {}
#       else
#           define BOOST_FUSION_FORWARD_ARGUMENT(Z, N, __)\
                std::forward<BOOST_PP_CAT(A,N)>(BOOST_PP_CAT(_,N))

        template <BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, typename A)>
        BOOST_PP_CAT(vector, BOOST_FUSION_N)(
            BOOST_PP_ENUM_BINARY_PARAMS(BOOST_FUSION_N, A,&& _))
          : base_type(
              BOOST_PP_ENUM(BOOST_FUSION_N, BOOST_FUSION_FORWARD_ARGUMENT, _))
        {}

#           undef BOOST_FUSION_FORWARD_ARGUMENT
#       endif
#   endif

        template<typename Seq>
        BOOST_PP_CAT(vector, BOOST_FUSION_N)&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            *static_cast<base_type*>(this)=BOOST_FUSION_FORWARD(Seq,seq);
            return *this;
        }
    };
}}

    #undef BOOST_FUSION_N
#endif

