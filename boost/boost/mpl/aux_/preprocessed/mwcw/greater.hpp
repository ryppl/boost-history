namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    >
struct greater_impl
    : if_c<
          ( Tag1::value > Tag2::value )
        , aux::cast2nd_impl< greater_impl<Tag1,Tag2>,Tag1,Tag2 >
        , aux::cast1st_impl< greater_impl<Tag1,Tag2>,Tag1,Tag2 >
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
    : greater_impl<
          typename greater_tag<N1>::type
        , typename greater_tag<N2>::type
        >::template apply< N1,N2 >::type
{
};

}}
namespace boost { namespace mpl {
template<>
struct greater_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
        : bool_< ( BOOST_MPL_AUX_VALUE_WKND(N1)::value > BOOST_MPL_AUX_VALUE_WKND(N2)::value ) >
    {
    };
};

}}
