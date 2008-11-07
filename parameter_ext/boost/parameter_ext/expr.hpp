/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/ for library home page.
 */

#ifndef BOOST_DSL_EXPR_HPP_
#define BOOST_DSL_EXPR_HPP_

#if !defined(BOOST_DSL_EXPR_ARITY)
#	define BOOST_DSL_EXPR_ARITY 5
#endif

#if !defined(BOOST_MPL_LIMIT_METAFUNCTION_ARITY)
#   define BOOST_MPL_LIMIT_METAFUNCTION_ARITY BOOST_DSL_EXPR_ARITY 
#elif BOOST_MPL_LIMIT_METAFUNCTION_ARITY < BOOST_DSL_EXPR_ARITY
#	define BOOST_MPL_LIMIT_METAFUNCTION_ARITY BOOST_DSL_EXPR_ARITY
#else
#endif

#if !defined(BOOST_PARAMETER_MAX_ARITY)
#   define BOOST_PARAMETER_MAX_ARITY BOOST_DSL_EXPR_ARITY
#elif BOOST_PARAMETER_MAX_ARITY < BOOST_DSL_EXPR_ARITY
#	define BOOST_PARAMETER_MAX_ARITY BOOST_DSL_EXPR_ARITY
#else
#endif

#include <boost/parameter/parameters.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/size.hpp>
#include <boost/dsl/or_seq.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/type_traits/detail/type_trait_def.hpp>
#include <boost/parameter/binding.hpp>

namespace boost { 
namespace dsl {

// optional parameter
template <typename Tag, typename Pred, typename Default> 
struct optional {
	typedef Tag tag;
	typedef Pred pred;
	typedef Default default_t;
};

// mandatory/required parameter
template <typename Tag, typename Pred, typename Default> 
struct required {
	typedef Tag tag;
	typedef Pred pred;
	typedef Default default_t;
};

namespace detail{

template <typename Parameter, typename Tag>
struct has_tag : is_same<typename Parameter::tag, Tag> {
};

template <typename Parameter>
struct get_pred_impl {
		typedef typename Parameter::pred type;
};

BOOST_TT_AUX_TYPE_TRAIT_DEF1(get_pred,T,typename get_pred_impl<T>::type)

// get the default_t member of a dsl::vector of parameters 

template <typename Tag, typename Parameters> 
struct default_t {
	typedef typename mpl::find_if<Parameters, has_tag<mpl::_1,Tag> >::type iter;
	typedef typename mpl::deref<iter>::type::default_t type;
};

// maps from dsl::optional/requered parameters to a parameters::optional/requered parameters 

template <typename T> struct signature_parameter;

template <typename Tag, typename Pred, typename Default> 
struct signature_parameter<optional<Tag, Pred, Default> > {
	typedef parameter::optional<parameter::deduced<Tag>, Pred>  type;
};

template <typename Tag, typename Pred, typename Default> 
struct signature_parameter<required<Tag, Pred, Default> >{
	typedef parameter::required<parameter::deduced<Tag>, Pred>  type;
};

// maps a vector of dsl::optional/requered parameters to a parameter::parameters of parameters::optional/requered  
template <typename T> struct signature;

// bind to the signature the Args provided

template <typename Signature, typename Args> 
struct args;

/* Used for the detection of unmatched template args in a
 * dsl instantiations.
 */

struct unmatched_arg;

// defines the unmatched signature type
template <typename Parameters> 
struct unmatched_signature {
	typedef typename mpl::transform<Parameters, get_pred<mpl::_1>, 
	mpl::back_inserter< mpl::vector0<> > >::type preds;
	
    typedef parameter::parameters<
    	parameter::optional<
    		parameter::deduced<detail::unmatched_arg>, 
    		mpl::not_< or_seq<preds> >    		
    	>
    >  type;
};

// bind the unmatched signature to the given args
template <typename UnmatchedSignature, typename Args> 
struct unmatched_args;

}
}
}

// include variadic templates using the preprocesor
#include <boost/dsl/expr_pp.hpp>

namespace boost { 
namespace dsl {

template <typename Tag, typename Parameters, typename Args> 
class expr {
public:	
	typedef Tag 			tag;
    typedef expr 			base_expr;
private:	
	typedef Parameters		parameters;
    typedef mpl::size<Parameters> 	arity;
    typedef typename detail::signature<Parameters>::type signature;
    typedef typename detail::args<signature, Args>::type args;

    typedef typename detail::unmatched_signature<Parameters>::type unmatched_signature;
    typedef typename detail::unmatched_args<unmatched_signature, Args>::type unmatched_args;
    typedef typename parameter::binding<
      unmatched_args,detail::unmatched_arg,  detail::unmatched_arg
    >::type                                      unmatched_arg_detected;
    /* You have passed a type in the specification of a expr type that
     * could not be interpreted as a valid argument.
     */
    BOOST_STATIC_ASSERT((
      is_same<unmatched_arg_detected,detail::unmatched_arg>::value));
    
public:
	template <typename TagP>
    struct get{
  	  typedef typename parameter::binding<
  	  	args, TagP, typename detail::default_t<TagP, parameters>::type
  	  >::type                                      type;
    };
};

#if 1

template <BOOST_PP_ENUM_PARAMS(n,typename Param)>
struct parameters<BOOST_PP_ENUM_PARAMS(n,Param), void_> {
	template <BOOST_PP_ENUM_PARAMS(n,typename Arg)>
	class bind {
private:	
//	typedef Parameters		parameters;
//    typedef mpl::size<Parameters> 	arity;
	
//    typedef typename detail::signature<Parameters>::type signature;
    typedef parameter::parameters<
    BOOST_PP_ENUM(n,BOOST_DSL_EXPR_signature_parameter,~)
    >                                      signature;	
//    typedef typename detail::args<signature, Args>::type args;
	typedef typename signature::template bind<BOOST_PP_ENUM_PARAMS(n,Arg)>::type args;
    

//    typedef typename detail::unmatched_signature<Parameters>::type unmatched_signature;
	typedef typename mpl::transform<mpl::vector<BOOST_PP_ENUM_PARAMS(n,Param), get_pred<mpl::_1>, 
	mpl::back_inserter< mpl::vector0<> > >::type preds;
	
    typedef parameter::parameters<
    	parameter::optional<
    		parameter::deduced<detail::unmatched_arg>, 
    		mpl::not_< or_seq<preds> >    		
    	>
    >  unmatched_signature;
    
    
//    typedef typename detail::unmatched_args<unmatched_signature, Args>::type unmatched_args;
	typedef typename unmatched_signature::template bind<BOOST_PP_ENUM_PARAMS(n,Arg)>::type unmatched_args;

	typedef typename parameter::binding<
      unmatched_args,detail::unmatched_arg,  detail::unmatched_arg
    >::type                                      unmatched_arg_detected;
    /* You have passed a type in the specification of a expr type that
     * could not be interpreted as a valid argument.
     */
    BOOST_STATIC_ASSERT((
      is_same<unmatched_arg_detected,detail::unmatched_arg>::value));
    
public:
	template <typename TagP>
    struct get{
  	  typedef typename parameter::binding<
  	  	args, TagP, typename detail::default_t<TagP, parameters>::type
  	  >::type                                      type;
    };
};

#endif

} // dsl
} // boost
#endif // BOOST_DSL_EXPR_HPP_



