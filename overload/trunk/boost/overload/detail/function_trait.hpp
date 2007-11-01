/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _FUNCTION_TRAIT_HPP_
#define _FUNCTION_TRAIT_HPP_


///////////////////////////////////////////////////////////////////////////////
// this file contains type trait metafunctions used for making up a 
// workaround implementation for MSVC < 8.0 (tested on MSVC 7.1 )

#include <boost/type_traits/function_traits.hpp>

#include "final_overload_base.hpp"
#include "if_metafunction.hpp"

///////////////////////////////////////////////////////////////////////////////
// helper macros 
 
#define OVL_ARG_TYPE(z, n, type) \
typedef type BOOST_PP_CAT(arg, BOOST_PP_CAT(n, _type));
// end macro OVL_ARG_TYPE

#define OVL_IS_SIG_ARITY_ZERO(z, n, sig) \
( func_trait<BOOST_PP_CAT(sig,n)>::arity == 0 ) ||
// end macro OVL_IS_SIG_ARITY_ZERO


namespace boost{ namespace overloads{ namespace detail{

struct no_type;

template<unsigned int N>
struct dummy_signature;

// every signature has to have its dummy type
template<unsigned int N>
struct dummy_type;

template< typename error_type>
struct error{};

// used as error message
struct no_function_found_matching_function_call;
struct ambiguous_function_call;

///////////////////////////////////////////////////////////////////////////////
// func_trait is used to create some function traits specialization that 
// provides dummy argument types and compile time errors pointing out the real 
// compile issues

template< typename Signature >
struct func_trait : public boost::function_traits<Signature>
{
};

template<>
struct func_trait< no_signature >
{
	static const unsigned int arity = UINT_MAX;
	typedef no_type		result_type;
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_OVERLOAD_MAX_NUM_ARGS, OVL_ARG_TYPE, no_type);
};

template<unsigned int N>
struct func_trait< dummy_signature<N> >
{
	static const unsigned int arity = UINT_MAX;
    typedef dummy_type<N>		result_type;
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_OVERLOAD_MAX_NUM_ARGS, OVL_ARG_TYPE, dummy_type<N>);
};

template<typename error_type>
struct func_trait< error<error_type> >
{
	static const unsigned int arity = UINT_MAX;
	typedef error_type		result_type;
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_OVERLOAD_MAX_NUM_ARGS, OVL_ARG_TYPE, error_type);
};


///////////////////////////////////////////////////////////////////////////////
// metafunction zero_arity_signature
// if one and only one of the signature has zero arity type is defined as such 
// a signature; if there is no signature with zero arity type is defined as 
// no_function_found_matching_function_call; if the is more than one signature 
// with zero arity type is defined as ambiguous_function_call

// only an helper metafunction
template<bool cond, typename S>
struct if_not_ambiguous
{
	typedef 
		typename if_c<cond, S, error<ambiguous_function_call> >::type
		type;
};

template< BOOST_PP_ENUM_PARAMS(BOOST_OVERLOAD_LIMIT, typename Sig) >
struct zero_arity_signature
{
  private:
	static const bool sig0_no_arg = ( func_trait<Sig0>::arity == 0 );
	static const bool sig0_no_other_no_arg 
		= !( BOOST_PP_REPEAT_FROM_TO(1, BOOST_OVERLOAD_LIMIT, 
                                     OVL_IS_SIG_ARITY_ZERO, Sig) false );
  public:
	typedef 
		typename if_c<sig0_no_arg, 
			typename if_not_ambiguous<sig0_no_other_no_arg, Sig0>::type,
			typename zero_arity_signature<
                BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_OVERLOAD_LIMIT, Sig), no_signature
            >::type
		>::type
		type;
};

template< BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_OVERLOAD_LIMIT, typename Sig) >
struct zero_arity_signature< 
    no_signature, 
    BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_OVERLOAD_LIMIT, Sig) 
>
{
	typedef error<no_function_found_matching_function_call> type;
};

///////////////////////////////////////////////////////////////////////////////
// metafunction real_or_dummy_signature
// if the signature arity is equal to arity type is defined as the passed 
// signature on the contrary type is defined as an ad-hoc dummy_signature<N>

template<unsigned int N, typename S, unsigned int arity>
struct real_or_dummy_signature
{
  private:
	static const bool same_arity = ( func_trait<S>::arity == arity );
  public:
	typedef 
		typename if_c< same_arity, S, dummy_signature<N> >::type
		type;
};


} } } // end namespaces

#undef OVL_IS_SIG_ARITY_ZERO
#undef OVL_ARG_TYPE

#endif // _FUNCTION_TRAIT_HPP_

