namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    , int tag1_ = Tag1::value
    , int tag2_ = Tag2::value
    >
struct equal_to_impl
    : if_c<
          ( tag1_ > tag2_ )
        , aux::cast2nd_impl< equal_to_impl<Tag1,Tag2>,Tag1,Tag2 >
        , aux::cast1st_impl< equal_to_impl<Tag1,Tag2>,Tag1,Tag2 >
        >
{
};

template< typename T > struct equal_to_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    >
struct equal_to
    : aux::msvc_eti_base< typename apply_wrap2<
          equal_to_impl<
              typename equal_to_tag<N1>::type
            , typename equal_to_tag<N2>::type
            >
        , N1
        , N2
        >::type >::type
{
};

}}
namespace boost { namespace mpl {
template<>
struct equal_to_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
    {
        enum {
            msvc71_wknd_ = ( BOOST_MPL_AUX_VALUE_WKND(N1)::value
  == BOOST_MPL_AUX_VALUE_WKND(N2)::value )
        };
        enum { value = msvc71_wknd_ };
        typedef bool_<value> type;
    };
};

}}
