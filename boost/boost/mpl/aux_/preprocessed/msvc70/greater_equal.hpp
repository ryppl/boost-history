namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    , int tag1_ = Tag1::value
    , int tag2_ = Tag2::value
    >
struct greater_equal_impl
    : if_c<
          ( tag1_ > tag2_ )
        , aux::cast2nd_impl< greater_equal_impl<Tag1,Tag2>,Tag1,Tag2 >
        , aux::cast1st_impl< greater_equal_impl<Tag1,Tag2>,Tag1,Tag2 >
        >
{
};

template< typename T > struct greater_equal_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    >
struct greater_equal
    : aux::msvc_eti_base< typename apply_wrap2<
          greater_equal_impl<
              typename greater_equal_tag<N1>::type
            , typename greater_equal_tag<N2>::type
            >
        , N1
        , N2
        >::type >::type
{
};

}}
namespace boost { namespace mpl {
template<>
struct greater_equal_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
    {
        enum { value =
             ( BOOST_MPL_AUX_VALUE_WKND(N1)::value >=
             BOOST_MPL_AUX_VALUE_WKND(N2)::value )
            };
        typedef bool_<value> type;
    };
};

}}
