// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_DATAFLOW_UTILITY_BIND_MEM_FN_HPP)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/dataflow/utility/member_function_signature.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

namespace boost { namespace dataflow { namespace utility {

namespace detail {
        
    template<typename MemFn, typename T, int Arity>
    struct bind_mem_fn_impl;

#   define BOOST_PP_ITERATION_PARAMS_1 (3,(0,9,<boost/dataflow/utility/bind_mem_fn.hpp>))
#   include BOOST_PP_ITERATE()
    
    template<typename MemFn, typename T>
    struct bind_mem_fn : public bind_mem_fn_impl<MemFn, T, boost::function_types::function_arity<MemFn>::value-1>
    {};

} // namespace detail

/// Binds a class member function to a class object.
/** \returns boost::function type with the bound member function.
*/
template<typename MemFn, typename T>
boost::function<typename member_function_signature<MemFn>::type>
    bind_mem_fn(MemFn mem_fn, T &object)
{
	return detail::bind_mem_fn<MemFn, T>()(mem_fn, object);
}

} } } // namespace boost::dataflow::utility

#define BOOST_DATAFLOW_UTILITY_BIND_MEM_FN_HPP
#else // defined(BOOST_PP_IS_ITERATING)

template<typename MemFn, typename T>
struct bind_mem_fn_impl<MemFn, T, BOOST_PP_ITERATION()>
{
    boost::function<typename member_function_signature<MemFn>::type>
        operator()(MemFn mem_fn, T &object)
    {
        return boost::bind(mem_fn, boost::ref(object)
                        BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                        BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(BOOST_PP_ITERATION()),_));
    }
};

#endif // defined(BOOST_PP_IS_ITERATING)
#endif // BOOST_DATAFLOW_UTILITY_BIND_MEM_FN_HPP
