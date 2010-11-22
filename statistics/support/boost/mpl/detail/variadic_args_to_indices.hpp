#ifndef BOOST_MPL_DETAIL_VARIADIC_ARGS_TO_INDICES_HPP_ER_2010
#define BOOST_MPL_DETAIL_VARIADIC_ARGS_TO_INDICES_HPP_ER_2010

// Provided temporarily, until /variadic_template/package_range_c.hpp by
// Larry Evans is fully supported in the release.
// Source : http://groups.google.com/group/comp.lang.c++/browse_thread/thread/b21c20c4312c383f/3b842a03167f5bd9#3b842a03167f5bd9

namespace boost{
namespace mpl{
namespace er2010{

template<typename I, I...Values> struct indices{};

namespace impl{

template<typename ...Args> struct pack_holder{};

template<typename Indices,typename Pack> struct args_to_indices{};

template<typename I, I...Tail, typename T, typename... Rest>
struct args_to_indices<
    indices<I, Tail...>,
    impl::pack_holder<T, Rest...>
>
{
    typedef typename impl::args_to_indices<
        indices<I, Tail..., sizeof...(Tail)>,
        pack_holder<Rest...>
    >::type type;
};

template<typename I, I...Tail>
struct args_to_indices<
    indices<I, Tail...>,
    pack_holder<>
>{
    typedef indices<I, Tail...> type;
};

}// impl

template<typename I,typename...Args>
struct args_to_indices : impl::args_to_indices<
    indices<I>,
    impl::pack_holder<Args...>
>{};

}// er2010
}// mpl
}// boost

#endif // BOOST_MPL_DETAIL_VARIADIC_ARGS_TO_INDICES_HPP_ER_2010
