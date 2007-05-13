// storage.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_GENERATOR_HPP
#define SIGNAL_NETWORK_GENERATOR_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

namespace detail {

///	Converts a type into a storable type by removing const qualifiers and references.
template<typename T>
struct storable : public boost::remove_const<typename boost::remove_reference<T>::type > {};

}
SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#define SIGNAL_NETWORK_ARGVAR_TYPEDEF(z,n,text) typedef typename boost::signal_network::signet::detail::storable<typename boost::function_traits<Signature>::arg##n##_type>::type arg##n##_value_type;
#define SIGNAL_NETWORK_ARGVAR_TYPEDEFS(n) BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_ADD(n,1),SIGNAL_NETWORK_ARGVAR_TYPEDEF,_)

#define SIGNAL_NETWORK_ARGVAR(z,n,text) arg##n##_value_type arg##n##_value;
#define SIGNAL_NETWORK_ARGVARS(n) BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_ADD(n,1),SIGNAL_NETWORK_ARGVAR,_)

#define SIGNAL_NETWORK_ARGVAR_TYPENAME(n,text) typename boost::call_traits<text arg##n##_value_type>::param_type arg##n##_value
#define SIGNAL_NETWORK_ARGVAR_TYPENAME_COMMA(z,n,text) SIGNAL_NETWORK_ARGVAR_TYPENAME(n,text) BOOST_PP_COMMA()
#define SIGNAL_NETWORK_ARGVAR_TYPENAMES(n,text) BOOST_PP_REPEAT_FROM_TO(1,n,SIGNAL_NETWORK_ARGVAR_TYPENAME_COMMA,text) BOOST_PP_IF(n,SIGNAL_NETWORK_ARGVAR_TYPENAME(n,text),BOOST_PP_EMPTY())

#define SIGNAL_NETWORK_ARGVAR_NAME(n) arg##n##_value
#define SIGNAL_NETWORK_ARGVAR_NAME_COMMA(z,n,text) SIGNAL_NETWORK_ARGVAR_NAME(n) BOOST_PP_COMMA()
#define SIGNAL_NETWORK_ARGVAR_NAMES(n) BOOST_PP_REPEAT_FROM_TO(1,n,SIGNAL_NETWORK_ARGVAR_NAME_COMMA,_) BOOST_PP_IF(n,SIGNAL_NETWORK_ARGVAR_NAME(n),BOOST_PP_EMPTY())

#define SIGNAL_NETWORK_SET_ARGVAR(z,n,text) this->arg##n##_value = arg##n##_value;
//#define SIGNAL_NETWORK_SET_ARGVAR_COMMA(z,n,text) SIGNAL_NETWORK_SET_ARGVAR(n) BOOST_PP_COMMA()
//#define SIGNAL_NETWORK_SET_ARGVARS(n) BOOST_PP_REPEAT_FROM_TO(1,n,SIGNAL_NETWORK_SET_ARGVAR_COMMA,_) BOOST_PP_IF(n,SIGNAL_NETWORK_SET_ARGVAR(n),BOOST_PP_EMPTY())
#define SIGNAL_NETWORK_SET_ARGVARS(n) BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_INC(n),SIGNAL_NETWORK_SET_ARGVAR,_)

#define SIGNAL_NETWORK_RETURN_STORED(z,n,text) \
	typename boost::call_traits<arg##n##_value_type>::param_type value##n() {return arg##n##_value;} \
	slot_selector_t<storage_impl<Signature, SIGNAL_NETWORK_TEMPLATE_ARITY>, typename boost::call_traits<arg##n##_value_type>::param_type ()> slot##n () \
	{return slot_selector<typename boost::call_traits<arg##n##_value_type>::param_type ()>(*this, &storage_impl<Signature, SIGNAL_NETWORK_TEMPLATE_ARITY>::value##n);} \
	arg##n##_value_type value_exact##n() {return arg##n##_value;} \
	slot_selector_t<storage_impl<Signature, SIGNAL_NETWORK_TEMPLATE_ARITY>, arg##n##_value_type ()> slot_exact##n () \
	{return slot_selector<arg##n##_value_type ()>(*this, &storage_impl<Signature, SIGNAL_NETWORK_TEMPLATE_ARITY>::value_exact##n);}


#define SIGNAL_NETWORK_STORAGE_SIGNAL() \
void operator()(SIGNAL_NETWORK_ARGVAR_TYPENAMES(SIGNAL_NETWORK_TEMPLATE_ARITY,BOOST_PP_EMPTY())) \
{ \
	SIGNAL_NETWORK_SET_ARGVARS(SIGNAL_NETWORK_TEMPLATE_ARITY) \
}

#define SIGNAL_NETWORK_TEMPLATE_CLASS storage
#define SIGNAL_NETWORK_TEMPLATE_BASE filter<Signature>
#define SIGNAL_NETWORK_TEMPLATE_ITERATE_MAIN_CLASS
#include <boost/signal_network/detail/loader.hpp>

#undef SIGNAL_NETWORK_STORAGE_SIGNAL
#endif // SIGNAL_NETWORK_GENERATOR_HPP