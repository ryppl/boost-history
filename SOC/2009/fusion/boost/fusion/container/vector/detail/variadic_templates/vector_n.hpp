// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define N BOOST_PP_ITERATION()

//cschmidt: template typedefs if possible...

#if !N
struct void_;

template <class T0=void_>
struct vector0;

template<>
struct vector0<void_>
#else
template <BOOST_PP_ENUM_PARAMS(N, typename T)>
#endif
struct BOOST_PP_CAT(vector, N)
  : vector<BOOST_PP_ENUM_PARAMS(N, T)>
{
private:
    typedef vector<BOOST_PP_ENUM_PARAMS(N, T)> base;

public:
    BOOST_PP_CAT(vector, N)()
    {}

#define VECTOR_CTOR(COMBINATION,_)\
    BOOST_PP_CAT(vector, N)(BOOST_PP_CAT(vector, N) COMBINATION vec)\
      : base(sequence_assign(\
            BOOST_FUSION_FORWARD(BOOST_PP_CAT(vector, N) COMBINATION,vec)))\
    {}

    BOOST_FUSION_ALL_CV_REF_COMBINATIONS(VECTOR_CTOR,_)

#undef VECTOR_CTOR

    template<typename Arg>
    BOOST_PP_CAT(vector, N)(BOOST_FUSION_R_ELSE_CLREF(Arg) arg)
      : base(BOOST_FUSION_FORWARD(Arg,arg))
    {}

#if N > 1
#   ifdef BOOST_NO_RVALUE_REFERENCES
    BOOST_PP_CAT(vector, N)(BOOST_PP_ENUM_BINARY_PARAMS(N, const T, & a))
      : base(BOOST_PP_ENUM_PARAMS(N, a))
    {}
#   else
#       define FORWARD_ARGUMENT(Z, INDEX, _) std::forward<\
            BOOST_PP_CAT(OtherT,INDEX)>(BOOST_PP_CAT(other,INDEX))

    template <BOOST_PP_ENUM_PARAMS(N, typename OtherT)>
    BOOST_PP_CAT(vector, N)(BOOST_PP_ENUM_BINARY_PARAMS(N, OtherT,&& other))
      : base(BOOST_PP_ENUM(N, FORWARD_ARGUMENT, _))
    {}

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

