#ifndef BOOST_ASSIGN_V2_SUPPORT_MPL_VARIADIC_ARGS_TO_INDICES_HPP_ER_2010
#define BOOST_ASSIGN_V2_SUPPORT_MPL_VARIADIC_ARGS_TO_INDICES_HPP_ER_2010
#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

// Provided temporarily, until /variadic_template/package_range_c.hpp by
// Larry Evans is fully supported in the release.
// Source : http://groups.google.com/group/comp.lang.c++/browse_thread/thread/b21c20c4312c383f/3b842a03167f5bd9#3b842a03167f5bd9

namespace boost{
namespace assign{
namespace v2{

template<typename I, I...Values> struct indices{};
template<typename ...Args> struct pack_holder{};
template<typename I, typename PackHolder> struct pack_holder_size{};
template<typename I, typename ...Args>
struct pack_holder_size<
	I, pack_holder<Args...>
>
{
	BOOST_STATIC_CONSTANT(I, value = sizeof...(Args) );
};

namespace mpl_aux{

// Args to indices

template<typename Indices, typename Pack> struct args_to_indices{};

template<typename I, I...Tail, typename T, typename... Rest>
struct args_to_indices<
    indices<I, Tail...>,
    pack_holder<T, Rest...>
>
{
    typedef typename args_to_indices<
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

// Head indices

template<typename Indices, typename I, I K, I N>
struct collect_indices
{};

template<typename I, I K, I N, I...Head>
struct collect_indices<
    indices<I, Head...>,
    I, K, N
>
{
	typedef typename collect_indices<
    	indices<I, Head..., K>,
        I, K + 1, N
    >::type type;
};

template<typename I, I N, I...Head>
struct collect_indices<indices<I, Head...>, I, N, N>
{
	typedef indices<I, Head...> type;
};

}// aux

template<typename I, typename...Args>
struct args_to_indices
    : mpl_aux::args_to_indices<
        indices<I>,
        pack_holder<Args...>
    >
{};

template<typename I, I N>
struct head_indices
	: mpl_aux::collect_indices<
    	indices<I>, I, 0, N
    >
{};

template<typename I, I K, I N>
struct tail_indices
	: mpl_aux::collect_indices<
    	indices<I, K>, I, K + 1, N
    >
{};

template<typename I, I N>
struct tail_indices<I, N, N>
	: mpl_aux::collect_indices<
    	indices<I>, I, N, N
    >
{};

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_MPL_VARIADIC_ARGS_TO_INDICES_HPP_ER_2010
