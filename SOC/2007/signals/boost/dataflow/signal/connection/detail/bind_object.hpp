// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SIGNAL_NETWORK_BIND_OBJECT_HPP)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/dataflow/signal/connection/detail/slot_type.hpp>

#include <boost/function_types/function_arity.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace boost { namespace signals {
        
namespace detail {
        
    template<typename Signature, typename T, int Arity>
    struct bind_object_impl;
    
    template<typename Signature, typename T>
    struct bind_object : public bind_object_impl<Signature, T, boost::function_types::function_arity<Signature>::value>
    {};
        
#   define BOOST_PP_ITERATION_PARAMS_1 (3,(0,9,<boost/dataflow/signal/connection/detail/bind_object.hpp>))
#   include BOOST_PP_ITERATE()

    } // namespace detail

} } // namespace boost::signals

#define SIGNAL_NETWORK_BIND_OBJECT_HPP
#else // defined(BOOST_PP_IS_ITERATING)

template<typename Signature, typename T>
struct bind_object_impl<Signature, T, BOOST_PP_ITERATION()>
{
    boost::function<Signature> operator()(typename slot_type<Signature, T>::type mem_fn, T &object)
    {
        return boost::bind(mem_fn, boost::ref(object)
                        BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                        BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(BOOST_PP_ITERATION()),_));
    }
    boost::function<Signature> operator()(typename slot_type<Signature, T>::type mem_fn, const T &object)
    {
        return boost::bind(mem_fn, boost::ref(object)
                           BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                           BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(BOOST_PP_ITERATION()),_));
    }
};

#endif // defined(BOOST_PP_IS_ITERATING)
#endif // SIGNAL_NETWORK_BIND_OBJECT_HPP

    