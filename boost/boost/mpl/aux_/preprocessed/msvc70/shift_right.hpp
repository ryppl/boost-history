
// Copyright Aleksey Gurtovoy 2000-2004
// Copyright Jaap Suter 2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// Preprocessed version of "boost/mpl/shift_right.hpp" header
// -- DO NOT modify by hand!

namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2

    , int tag1_ = Tag1::value
    , int tag2_ = Tag2::value
    >
struct shift_right_impl
    : if_c<
          ( tag1_ > tag2_ )

        , aux::cast2nd_impl< shift_right_impl< Tag1,Tag2 >,Tag1, Tag2 >
        , aux::cast1st_impl< shift_right_impl< Tag1,Tag2 >,Tag1, Tag2 >
        >
{
};

/// for Digital Mars C++/compilers with no CTPS support
template<> struct shift_right_impl< na,na >
{
    template< typename U1, typename U2 > struct apply
    {
        typedef apply type;
        enum { value = 0 };
    };
};

template<> struct shift_right_impl< na,integral_c_tag >
{
    template< typename U1, typename U2 > struct apply
    {
        typedef apply type;
        enum { value = 0 };
    };
};

template<> struct shift_right_impl< integral_c_tag,na >
{
    template< typename U1, typename U2 > struct apply
    {
        typedef apply type;
        enum { value = 0 };
    };
};

template< typename T > struct shift_right_tag
    : tag< T,na >
{
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(N1)
    , typename BOOST_MPL_AUX_NA_PARAM(N2)
    >
struct shift_right
    : aux::msvc_eti_base< typename apply_wrap2<
          shift_right_impl<
              typename shift_right_tag<N1>::type
            , typename shift_right_tag<N2>::type
            >
        , N1
        , N2
        >::type >::type

{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2, shift_right, (N1, N2))

};

BOOST_MPL_AUX_NA_SPEC2(2, 2, shift_right)

}}

namespace boost { namespace mpl {

namespace aux {
template< typename T, typename Shift, T n, Shift s >
struct msvc_shift_right_impl
{
  enum msvc_wknd { value = (n >> s) };
    typedef integral_c< T,value > type;
};

}

template<>
struct shift_right_impl< integral_c_tag,integral_c_tag >
{
    template< typename N, typename Shift > struct apply

        : aux::msvc_shift_right_impl<
              typename N::value_type
            , typename Shift::value_type
            , N::value
            , Shift::value
            >::type

    {
    };
};

}}
