
// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// Preprocessed version of "boost/mpl/divides.hpp" header
// -- DO NOT modify by hand!

namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    >
struct divides_impl
    : if_c<
          ( Tag1::value > Tag2::value )

        , aux::cast2nd_impl< divides_impl< Tag1,Tag2 >,Tag1, Tag2 >
        , aux::cast1st_impl< divides_impl< Tag1,Tag2 >,Tag1, Tag2 >
        >
{
};

template< typename T > struct divides_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    >
struct divides
    : divides_impl<
          typename divides_tag<N1>::type
        , typename divides_tag<N2>::type
        >::template apply< N1,N2 >::type
{
};

}}

namespace boost { namespace mpl {
template<>
struct divides_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply

        : integral_c<
              typename aux::largest_int<
                  typename N1::value_type
                , typename N2::value_type
                >::type
            , BOOST_MPL_AUX_VALUE_WKND(N1)::value
                / BOOST_MPL_AUX_VALUE_WKND(N2)::value
            >
    {
    };
};

}}
