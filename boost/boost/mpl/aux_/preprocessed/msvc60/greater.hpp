
// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// Preprocessed version of "boost/mpl/greater.hpp" header
// -- DO NOT modify by hand!

namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2

    , int tag1_ = Tag1::value
    , int tag2_ = Tag2::value
    >
struct greater_impl
    : if_c<
          ( tag1_ > tag2_ )

        , aux::cast2nd_impl< greater_impl< Tag1,Tag2 >,Tag1, Tag2 >
        , aux::cast1st_impl< greater_impl< Tag1,Tag2 >,Tag1, Tag2 >
        >
{
};

template< typename T > struct greater_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    >
struct greater
    : aux::msvc_eti_base< typename apply_wrap2<
          greater_impl<
              typename greater_tag<N1>::type
            , typename greater_tag<N2>::type
            >
        , N1
        , N2
        >::type >::type

{
};

}}

namespace boost { namespace mpl {

template<>
struct greater_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply

    {
        enum { value =
             ( BOOST_MPL_AUX_VALUE_WKND(N1)::value >
             BOOST_MPL_AUX_VALUE_WKND(N2)::value )
            };
        typedef bool_<value> type;
    };
};

}}
