
// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// Preprocessed version of "boost/mpl/modulus.hpp" header
// -- DO NOT modify by hand!

namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2

    , int tag1_ = Tag1::value
    , int tag2_ = Tag2::value
    >
struct modulus_impl
    : if_c<
          ( tag1_ > tag2_ )

        , aux::cast2nd_impl< modulus_impl< Tag1,Tag2 >,Tag1, Tag2 >
        , aux::cast1st_impl< modulus_impl< Tag1,Tag2 >,Tag1, Tag2 >
        >
{
};

template< typename T > struct modulus_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    >
struct modulus
    : aux::msvc_eti_base< typename apply_wrap2<
          modulus_impl<
              typename modulus_tag<N1>::type
            , typename modulus_tag<N2>::type
            >
        , N1
        , N2
        >::type >::type

{
};

}}

namespace boost { namespace mpl {

namespace aux {
template< typename T, T n1, T n2 >
struct msvc_modulus_impl
{
  enum msvc_wknd { value = (n1 % n2) };
    typedef integral_c< T,value > type;
};

}

template<>
struct modulus_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
        : aux::msvc_modulus_impl<
              typename aux::largest_int<
                  typename N1::value_type
                , typename N2::value_type
                >::type
            , N1::value
            , N2::value
            >::type

    {
    };
};

}}
