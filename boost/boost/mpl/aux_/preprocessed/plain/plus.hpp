namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    >
struct plus_impl
    : if_c<
          ( Tag1::value > Tag2::value )
        , aux::cast2nd_impl< plus_impl<Tag1,Tag2>,Tag1,Tag2 >
        , aux::cast1st_impl< plus_impl<Tag1,Tag2>,Tag1,Tag2 >
        >
{
};

template< typename T > struct plus_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    , typename N3 = na, typename N4 = na, typename N5 = na
    >
struct plus
    : plus< plus<plus<plus<N1,N2>,N3>,N4>,N5 >
{
};

template<
      typename N1, typename N2, typename N3, typename N4
    >
struct plus< N1,N2,N3,N4,na >
    : plus< plus<plus<N1,N2>,N3>,N4 >
{
};

template<
      typename N1, typename N2, typename N3
    >
struct plus< N1,N2,N3,na,na >
    : plus< plus<N1,N2>,N3 >
{
};

template<
      typename N1, typename N2
    >
struct plus< N1,N2,na,na,na >
    : plus_impl<
          typename plus_tag<N1>::type
        , typename plus_tag<N2>::type
        >::template apply< N1,N2 >::type
{
};

}}
namespace boost { namespace mpl {
template<>
struct plus_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
        : integral_c<
              typename aux::largest_int<
                  typename N1::value_type
                , typename N2::value_type
                >::type
            , BOOST_MPL_AUX_VALUE_WKND(N1)::value
                + BOOST_MPL_AUX_VALUE_WKND(N2)::value
            >
    {
    };
};

}}
