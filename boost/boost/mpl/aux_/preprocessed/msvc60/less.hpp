namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    , int tag1_ = Tag1::value
    , int tag2_ = Tag2::value
    >
struct less_impl
    : if_c<
          ( tag1_ > tag2_ )
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
    : aux::msvc_eti_base< typename apply_wrap2<
          less_impl<
              typename less_tag<N1>::type
            , typename less_tag<N2>::type
            >
        , N1
        , N2
        >::type >::type
{
};

}}
namespace boost { namespace mpl {
template<>
struct less_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
    {
        enum { value =
             ( BOOST_MPL_AUX_VALUE_WKND(N2)::value >
             BOOST_MPL_AUX_VALUE_WKND(N1)::value )
            };
        typedef bool_<value> type;
    };
};

}}
