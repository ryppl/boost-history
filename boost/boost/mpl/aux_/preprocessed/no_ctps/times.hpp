namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    >
struct times_impl
    : if_c<
          ( Tag1::value > Tag2::value )
        , aux::cast2nd_impl< times_impl<Tag1,Tag2>,Tag1,Tag2 >
        , aux::cast1st_impl< times_impl<Tag1,Tag2>,Tag1,Tag2 >
        >
{
};

template< typename T > struct times_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    >
struct times
    : times_impl<
          typename times_tag<N1>::type
        , typename times_tag<N2>::type
        >::template apply< N1,N2 >::type
{
};

}}
namespace boost { namespace mpl {
template<>
struct times_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
        : integral_c<
              typename aux::largest_int<
                  typename N1::value_type
                , typename N2::value_type
                >::type
            , BOOST_MPL_AUX_VALUE_WKND(N1)::value
                * BOOST_MPL_AUX_VALUE_WKND(N2)::value
            >
    {
    };
};

}}
