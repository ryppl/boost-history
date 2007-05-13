// signal_defines.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef AME_SIGNAL_DEFINES_HPP
#define AME_SIGNAL_DEFINES_HPP

#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>

#define SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE namespace boost { namespace signal_network { namespace signet {
#define SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE } } }

#define SIGNAL_NETWORK_OPEN_SIGNAL_NETWORK_NAMESPACE namespace boost { namespace signal_network {
#define SIGNAL_NETWORK_CLOSE_SIGNAL_NETWORK_NAMESPACE } }


// generates a list of arguments with names and types
// e.g. SIGNAL_NETWORK_ARG_TYPENAMES(2) evaluates to:
// typename boost::function_traits<Signature>::arg1_type arg1, typename boost::function_traits<Signature>::arg2_type arg2
#define SIGNAL_NETWORK_ARG_TYPENAME(z,n,text) typename boost::function_traits<Signature>::arg##n##_type arg##n
#define SIGNAL_NETWORK_ARG_TYPENAME_COMMA(z,n,text) SIGNAL_NETWORK_ARG_TYPENAME(z,n,text) BOOST_PP_COMMA()
#define SIGNAL_NETWORK_ARG_TYPENAMES(n) BOOST_PP_REPEAT_FROM_TO(1,n,SIGNAL_NETWORK_ARG_TYPENAME_COMMA,_) BOOST_PP_IF(n,SIGNAL_NETWORK_ARG_TYPENAME(n,n,_),BOOST_PP_EMPTY())
#define SIGNAL_NETWORK_ARG_TYPENAMES_INC(n) BOOST_PP_EXPAND(BOOST_PP_ENUM_SHIFTED(n,SIGNAL_NETWORK_ARG_TYPENAME,_))

// generates a list of arguments with names only
// e.g. SIGNAL_NETWORK_ARG_NAMES(2) evaluates to:
// arg1, arg2
#define SIGNAL_NETWORK_ARG_NAMES(n) BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(n), arg)

// generates a list of arguments with types only
// e.g. SIGNAL_NETWORK_ARG_TYPES(2) evaluates to:
// typename boost::function_traits<Signature>::arg1_type, typename boost::function_traits<Signature>::arg2_type
#define SIGNAL_NETWORK_ARG_TYPE(n) typename boost::function_traits<Signature>::arg##n##_type
#define SIGNAL_NETWORK_ARG_TYPE_COMMA(z,n,text) SIGNAL_NETWORK_ARG_TYPE(n) BOOST_PP_COMMA()
#define SIGNAL_NETWORK_ARG_TYPES(n) BOOST_PP_REPEAT_FROM_TO(1,n,SIGNAL_NETWORK_ARG_TYPE_COMMA,_) BOOST_PP_IF(n,SIGNAL_NETWORK_ARG_TYPE(n),BOOST_PP_EMPTY())

#define SIGNAL_NETWORK_FUNCTION(_name,_signature,_arity) typename boost::function_traits<_signature>::result_type _name (SIGNAL_NETWORK_ARG_TYPENAMES(_arity))
#define SIGNAL_NETWORK_DEFAULT_SLOT operator()

// generates a list of bound aruments
// e.g. SIGNAL_NETWORK_BIND_ARGS(2) evaluates to ,_1, _2
#define SIGNAL_NETWORK_BIND_ARGS(n) BOOST_PP_IF(n,BOOST_PP_COMMA,BOOST_PP_EMPTY)() BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(n),_)

#define SIGNAL_NETWORK_RETURN(_return) BOOST_PP_IF(_return,return,BOOST_PP_EMPTY())

#endif