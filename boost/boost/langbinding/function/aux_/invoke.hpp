#ifndef BOOST_PP_IS_ITERATING

#ifndef INVOKE_040917_HPP
#define INVOKE_040917_HPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>

#include <boost/spirit/fusion/sequence/get.hpp>

#include <boost/langbinding/function/aux_/invoke_tag.hpp>

namespace boost { namespace langbinding { namespace function { namespace aux {

#define BOOST_LANGBINDING_EXTRACT(z, n, tuple) fusion::get<n>(tuple)()

#define BOOST_PP_ITERATION_PARAMS_1                             \
    (3, (1, BOOST_LANGBINDING_FUNCTION_MAX_ARITY,               \
    <boost/langbinding/function/aux_/invoke.hpp>))
#include BOOST_PP_ITERATE()

}}}} // namespace boost::langbinding::function::aux

#endif // INVOKE_040917_HPP

#else // BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template<class F, class Converters>
void* invoke(invoke_tag_<true, false>, mpl::long_<N>, F const& fn, Converters const& cvs)
{
    fn(BOOST_PP_ENUM(N, BOOST_LANGBINDING_EXTRACT, cvs.args));
    return cvs.return_();
}

template<class F, class Converters>
void* invoke(invoke_tag_<false, false>, mpl::long_<N>, F const& fn, Converters const& cvs)
{
    return cvs.return_(
        fn(BOOST_PP_ENUM(N, BOOST_LANGBINDING_EXTRACT, cvs.args))
    );
}

#endif

