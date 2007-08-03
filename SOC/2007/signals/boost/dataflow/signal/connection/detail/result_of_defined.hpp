// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_RESULT_OF_DEFINED_HPP
#define SIGNAL_NETWORK_RESULT_OF_DEFINED_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
//#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/integral_constant.hpp>

namespace boost { namespace signals {
        
namespace detail {

//    BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)
    template<typename T, typename Enable=void>
    struct has_result_type : public boost::false_type {};
        
    template<typename T>
    struct has_result_type<T, typename boost::dataflow::detail::enable_if_defined<typename T::result_type>::type>
        : public boost::true_type {};

    template<typename F, typename FArgs, typename Enable=void>
    struct result_defined : public boost::false_type {};

    template<typename F, typename FArgs>
        struct result_defined<F, FArgs, typename boost::dataflow::detail::enable_if_defined<typename F::template result<FArgs>::type >::type>
        : public boost::true_type {};

    template<typename T, typename Enable=void>
    struct result_of_defined : public boost::false_type {};

    template<typename T>
    struct result_of_defined<T, typename boost::enable_if<
        typename boost::mpl::or_<
            has_result_type<typename boost::function_types::result_type<T>::type>,
            result_defined<typename boost::function_types::result_type<T>::type, T>
        >::type
    >::type>
        : public boost::true_type {};        

} // namespace detail

} } // namespace boost::signals

#endif // 