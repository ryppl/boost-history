namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    >
struct less_impl
    : if_c<
          ( Tag1::value > Tag2::value )
        , aux::cast2nd_impl< less_impl<Tag1,Tag2>,Tag1,Tag2 >
        , aux::cast1st_impl< less_impl<Tag1,Tag2>,Tag1,Tag2 >
        >
{
};

template< typename T > struct less_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    >
struct less
    : less_impl<
          typename less_tag<N1>::type
        , typename less_tag<N2>::type
        >::template apply< N1,N2 >::type
{
};

}}
namespace boost { namespace mpl {
template<>
struct less_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
        : bool_< ( BOOST_MPL_AUX_VALUE_WKND(N2)::value > BOOST_MPL_AUX_VALUE_WKND(N1)::value ) >
    {
    };
};

}}
