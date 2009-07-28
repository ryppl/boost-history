// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PP_IS_ITERATING
#   include <boost/fusion/container/vector/vector.hpp>

#   include <boost/preprocessor/iteration/iterate.hpp>
#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/repetition/enum.hpp>
#   include <boost/preprocessor/repetition/enum_binary_params.hpp>

#   define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/variadic_templates/vector_n.hpp>
#   define BOOST_PP_ITERATION_LIMITS (BOOST_FUSION_FROM, BOOST_FUSION_TO)
#   include BOOST_PP_ITERATE()

#else
#   define N BOOST_PP_ITERATION()

    //cschmidt: template typedefs if possible...
namespace boost { namespace fusion
{

#   if !N
    struct void_;

    template <class T0=void_>
    struct vector0;

    template<>
    struct vector0<void_>
#   else
    template <BOOST_PP_ENUM_PARAMS(N, typename T)>
    struct BOOST_PP_CAT(vector, N)
#   endif
      : vector<BOOST_PP_ENUM_PARAMS(N, T)>
    {
    private:
        typedef vector<BOOST_PP_ENUM_PARAMS(N, T)> base;

    public:
        BOOST_PP_CAT(vector, N)()
        {}

#   define VECTOR_CTOR(COMBINATION,_)\
        BOOST_PP_CAT(vector, N)(BOOST_PP_CAT(vector, N) COMBINATION vec)\
          : base(sequence_assign(\
                BOOST_FUSION_FORWARD(BOOST_PP_CAT(vector, N) COMBINATION,vec)))\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(VECTOR_CTOR,_)

#   undef VECTOR_CTOR

        template<typename Arg>
        BOOST_PP_CAT(vector, N)(BOOST_FUSION_R_ELSE_CLREF(Arg) arg)
          : base(BOOST_FUSION_FORWARD(Arg,arg))
        {}

#   if N > 1
#       ifdef BOOST_NO_RVALUE_REFERENCES
        BOOST_PP_CAT(vector, N)(BOOST_PP_ENUM_BINARY_PARAMS(N, T, const& a))
          : base(BOOST_PP_ENUM_PARAMS(N, a))
        {}
#       else
#           define FORWARD_ARGUMENT(Z, N, __) std::forward<\
                BOOST_PP_CAT(A,N)>(BOOST_PP_CAT(_,N))

        template <BOOST_PP_ENUM_PARAMS(N, typename A)>
        BOOST_PP_CAT(vector, N)(BOOST_PP_ENUM_BINARY_PARAMS(N, A,&& _))
          : base(BOOST_PP_ENUM(N, FORWARD_ARGUMENT, _))
        {}

#           undef FORWARD_ARGUMENT
#       endif
#   endif

        template<typename Seq>
        BOOST_PP_CAT(vector, N)&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            *static_cast<base*>(this)=BOOST_FUSION_FORWARD(Seq,seq);
            return *this;
        }
    };
}}

    #undef N
#endif

