namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    , int tag1_ = Tag1::value
    , int tag2_ = Tag2::value
    >
struct plus_impl
    : if_c<
          ( tag1_ > tag2_ )
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
    >
struct plus
    : aux::msvc_eti_base< typename apply_wrap2<
          plus_impl<
              typename plus_tag<N1>::type
            , typename plus_tag<N2>::type
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
struct msvc_plus_impl
{
  enum msvc_wknd { value = (n1 + n2) };
    typedef integral_c< T,value > type;
};

}
template<>
struct plus_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
        : aux::msvc_plus_impl<
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
