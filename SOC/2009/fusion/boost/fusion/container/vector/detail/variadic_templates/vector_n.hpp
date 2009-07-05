// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define N BOOST_PP_ITERATION()

//TODO cschmidt: template typedefs

#if N != 0
template <BOOST_PP_ENUM_PARAMS(N, typename T)>
#endif
struct BOOST_PP_CAT(vector, N)
    : vector<BOOST_PP_ENUM_PARAMS(N, T)>
{
private:
    typedef vector<BOOST_PP_ENUM_PARAMS(N, T)> base;

public:
    BOOST_PP_CAT(vector, N)()
    {
    }

    template<typename Arg>
    BOOST_PP_CAT(vector, N)(BOOST_FUSION_R_ELSE_CLREF(Arg) arg)
        : base(BOOST_FUSION_FORWARD(Arg,arg))
    {
    }

#if N > 1
#   ifdef BOOST_NO_RVALUE_REFERENCES
    BOOST_PP_CAT(vector, N)(BOOST_PP_ENUM_BINARY_PARAMS(
            N, typename detail::call_param<T, >::type _))
        : base(BOOST_PP_ENUM_PARAMS(N, _))
    {
    }
#   else
#       define FORWARD_ARGUMENT(Z, INDEX, _) std::forward<\
            BOOST_PP_CAT(OtherT,INDEX)>(BOOST_PP_CAT(other,INDEX))

    template <BOOST_PP_ENUM_PARAMS(N, typename OtherT)>
    BOOST_PP_CAT(vector, N)(BOOST_PP_ENUM_BINARY_PARAMS(N, OtherT,&& other))
        : base(BOOST_PP_ENUM(N, FORWARD_ARGUMENT, _))
    {
    }

#       undef FORWARD_ARGUMENT
#   endif
#endif

    template<typename Seq>
    BOOST_PP_CAT(vector, N)&
    operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        *static_cast<base*>(this)=BOOST_FUSION_FORWARD(Seq,seq);
        return *this;
    }
};

#undef N

