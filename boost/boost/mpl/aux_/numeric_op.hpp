
#if !defined(BOOST_PP_IS_ITERATING)

///// header body

// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION!

// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include <boost/mpl/numeric_cast.hpp>
#   include <boost/mpl/apply_wrap.hpp>
#   include <boost/mpl/if.hpp>
#   include <boost/mpl/aux_/na.hpp>
#   include <boost/mpl/aux_/msvc_eti_base.hpp>
#   include <boost/mpl/aux_/config/eti.hpp>
#endif

#include <boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER AUX778076_OP_NAME.hpp
#   include <boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <boost/mpl/limits/arity.hpp>
#   include <boost/mpl/aux_/preprocessor/partial_spec_params.hpp>
#   include <boost/mpl/aux_/preprocessor/def_params_tail.hpp>
#   include <boost/mpl/aux_/preprocessor/repeat.hpp>
#   include <boost/mpl/aux_/preprocessor/params.hpp>
#   include <boost/mpl/aux_/preprocessor/enum.hpp>

#   include <boost/preprocessor/dec.hpp>
#   include <boost/preprocessor/iterate.hpp>
#   include <boost/preprocessor/cat.hpp>


#if !defined(AUX778076_OP_ARITY)
#   if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#       define AUX778076_OP_ARITY BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#   else
#       define AUX778076_OP_ARITY 2
#   endif
#endif



namespace boost { namespace mpl {

template< 
      typename Tag1
    , typename Tag2
#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400))
    , int tag1_ = Tag1::value 
    , int tag2_ = Tag2::value 
    >
struct BOOST_PP_CAT(AUX778076_OP_NAME,_impl)
    : if_c<
          ( tag1_ < tag2_ )
#else
    >
struct plus_impl
    : if_c<
          ( Tag1::value <= Tag2::value )
#endif
        , aux::cast1st_impl< BOOST_PP_CAT(AUX778076_OP_NAME,_impl)<Tag1,Tag2>,Tag1,Tag2 >
        , aux::cast2nd_impl< BOOST_PP_CAT(AUX778076_OP_NAME,_impl)<Tag1,Tag2>,Tag1,Tag2 >
        >
{
};


template< typename T > struct BOOST_PP_CAT(AUX778076_OP_NAME,_tag)
{
    typedef typename T::tag type;
};


#if AUX778076_OP_ARITY == 2
template< 
      typename N1
    , typename N2
    >
struct AUX778076_OP_NAME
#else

#   define AUX778076_OP_N_CALLS(i, N) \
    BOOST_MPL_PP_PARAMS( BOOST_PP_DEC(i), AUX778076_OP_NAME<N ), \
    BOOST_PP_CAT(N,i) BOOST_MPL_PP_REPEAT(i, BOOST_MPL_PP_REPEAT_IDENTITY_FUNC, >) \
/**/

template<
      typename N1
    , typename N2
    , BOOST_MPL_PP_DEF_PARAMS_TAIL(2, typename N, na)
    >
struct AUX778076_OP_NAME
    : AUX778076_OP_N_CALLS(AUX778076_OP_ARITY, N)
{
};

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,( BOOST_PP_DEC(AUX778076_OP_ARITY), 2, <boost/mpl/aux_/numeric_op.hpp> ))
#include BOOST_PP_ITERATE()

#   undef AUX778076_OP_N_CALLS

#endif // AUX778076_OP_ARITY == 2

#if !defined(BOOST_MPL_CFG_MSVC_ETI_BUG)
    : BOOST_PP_CAT(AUX778076_OP_NAME,_impl)<
          typename BOOST_PP_CAT(AUX778076_OP_NAME,_tag)<N1>::type
        , typename BOOST_PP_CAT(AUX778076_OP_NAME,_tag)<N2>::type
        >::template apply<N1,N2>::type
#else
    : aux::msvc_eti_base< typename apply_wrap2<
          BOOST_PP_CAT(AUX778076_OP_NAME,_impl)<
              typename BOOST_PP_CAT(AUX778076_OP_NAME,_tag)<N1>::type
            , typename BOOST_PP_CAT(AUX778076_OP_NAME,_tag)<N2>::type
            >
        , N1
        , N2
        >::type >::type
#endif
{
};

}}

#endif // BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS

#undef AUX778076_OP_ARITY
#undef AUX778076_OP_NAME

///// iteration, depth == 1

#elif BOOST_PP_ITERATION_DEPTH() == 1

#   define i_ BOOST_PP_FRAME_ITERATION(1)

template<
      BOOST_MPL_PP_PARAMS(i_, typename N)
    >
struct AUX778076_OP_NAME<BOOST_MPL_PP_PARTIAL_SPEC_PARAMS(i_, N, na)>
#if i_ != 2
    : AUX778076_OP_N_CALLS(i_, N)
{
};
#endif

#   undef i_

#endif // BOOST_PP_IS_ITERATING
