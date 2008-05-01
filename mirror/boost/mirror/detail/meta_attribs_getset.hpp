/**
 * \file boost/mirror/detail/meta_attribs_getset.hpp
 * Mscros for declaring meta attributes accessed through getters/setters
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_DETAIL_META_ATTRIBS_GETSET_HPP
#define BOOST_MIRROR_DETAIL_META_ATTRIBS_GETSET_HPP

// macros for declaration of simple meta attributes
#include <boost/mirror/detail/meta_attribs_base.hpp>
//
// necessary preprocessor macros
#include <boost/preprocessor/tuple/to_list.hpp> 
#include <boost/preprocessor/list/enum.hpp> 
//
// various other things from boost to get this going
#include <boost/function_types/property_tags.hpp>
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/typeof/typeof.hpp>



namespace boost {
namespace mirror {

/** Helper macro for implementing getter based query meta-class function 
 *
 *  TODO: This is a rather unfortunate implementation ...
 *  It would be great is we could get rid of the do_* helpers.
 *  Anyway there should be two overloads of get and two for query 
 *  one having 'const the_class&' and the other 'the_class&' as the first param
 *  if the provided getter IS const qualified.
 *  Otherwise, if the getter IS NOT const qualified there should be only one
 *  version of get and one version of query, both having a 'the_class&'
 *  as the first argument. 
 *
 *  Now, both overloads get always compiled, but if the one with const the_class&
 *  gets never used it is (hopefully) culled out of the final binary.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_GETTER_ARGS(NUMBER, TYPE, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT) \
	static call_traits<TYPE>::param_type do_get(the_class& context, mpl::int_<NUMBER>, mpl::bool_<true>)\
	{\
		return context.GETTER_NAME(BOOST_PP_LIST_ENUM(BOOST_PP_TUPLE_TO_LIST(ARG_COUNT, GETTER_ARGS_TUPLE)));\
	} \
	static call_traits<TYPE>::param_type do_get(const the_class& context, mpl::int_<NUMBER>, mpl::bool_<function_types::is_member_function_pointer<BOOST_TYPEOF(&the_class::GETTER_NAME), function_types::const_qualified>::value>)\
	{\
		return const_cast<the_class&>(context).GETTER_NAME(BOOST_PP_LIST_ENUM(BOOST_PP_TUPLE_TO_LIST(ARG_COUNT, GETTER_ARGS_TUPLE)));\
	} \
	template <typename dest_type>\
	static dest_type& do_query(the_class& context, mpl::int_<NUMBER>, dest_type& dest, mpl::bool_<true>)\
	{\
		dest = dest_type(context.GETTER_NAME(BOOST_PP_LIST_ENUM(BOOST_PP_TUPLE_TO_LIST(ARG_COUNT, GETTER_ARGS_TUPLE))));\
		return dest;\
	} \
	template <typename dest_type>\
	static dest_type& do_query(const the_class& context, mpl::int_<NUMBER>, dest_type& dest, mpl::bool_<function_types::is_member_function_pointer<BOOST_TYPEOF(&the_class::GETTER_NAME), function_types::const_qualified>::value>)\
	{\
		dest = dest_type(const_cast<the_class&>(context).GETTER_NAME(BOOST_PP_LIST_ENUM(BOOST_PP_TUPLE_TO_LIST(ARG_COUNT, GETTER_ARGS_TUPLE))));\
		return dest;\
	} \
	template <class a_class> static call_traits<TYPE>::param_type get(a_class& context, mpl::int_<NUMBER>)\
	{\
		return do_get(context, mpl::int_<NUMBER>(), mpl::bool_<true>());\
	}\
	template <class a_class, typename dest_type> \
	static dest_type& query(a_class& context, mpl::int_<NUMBER>, dest_type& dest)\
	{\
		return do_query(context, mpl::int_<NUMBER>(), dest, mpl::bool_<true>());\
	}


#define BOOST_MIRROR_SETTER_SRC val

/** Helper macro for implementing setter based set meta-class function 
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SETTER_ARGS(NUMBER, TYPE, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT) \
	static void set(the_class& context, mpl::int_<NUMBER>, call_traits<TYPE>::param_type val)\
	{\
		context.SETTER_NAME(BOOST_PP_LIST_ENUM(BOOST_PP_TUPLE_TO_LIST(ARG_COUNT, SETTER_ARGS_TUPLE)));\
	} 


/** This macro declares a single class' query-only attribute with a getter 
 *  member function.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS_ONLY(NUMBER, TYPE, ATTRIB_NAME, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_GETTER_ARGS(NUMBER, TYPE, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_NO_SETTER(NUMBER, TYPE)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS
 *  for getter functions with no bound arguments
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_ONLY(NUMBER, TYPE, ATTRIB_NAME, GETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS_ONLY(NUMBER, TYPE, ATTRIB_NAME, GETTER_NAME, (), 0)


/** This macro declares a single class' query-only attribute with a getter 
 *  member function. Version for typedef'd types.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS_ONLY_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE_NAMESPACE::TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_GETTER_ARGS(NUMBER, TYPE_NAMESPACE::TYPE, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_NO_SETTER(NUMBER, TYPE_NAMESPACE::TYPE)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS
 *  for getter functions with no bound arguments
 *  Version for typedef'd types.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_ONLY_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, GETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS_ONLY_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, GETTER_NAME, (), 0)



/** This macro declares a single class' getter-query-simple-set attribute with a getter 
 *  member function and a simple value setting scheme.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS(NUMBER, TYPE, ATTRIB_NAME, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_GETTER_ARGS(NUMBER, TYPE, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, ATTRIB_NAME)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS
 *  for getter functions with no bound arguments
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER(NUMBER, TYPE, ATTRIB_NAME, GETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS(NUMBER, TYPE, ATTRIB_NAME, GETTER_NAME, (), 0)


/** This macro declares a single class' getter-query-simple-set attribute with a getter 
 *  member function and a simple value setting scheme. 
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE_NAMESPACE::TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_GETTER_ARGS(NUMBER, TYPE_NAMESPACE::TYPE, GETTER_NAME, GETTER_ARGS_TUPLE, ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE_NAMESPACE::TYPE, ATTRIB_NAME)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS
 *  for getter functions with no bound arguments.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_TD(NUMBER, TYPE, TYPE_NS_ALIAS, TYPE_NAMESPACE, ATTRIB_NAME, GETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_W_ARGS_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, GETTER_NAME, (), 0)

	
/** This macro declares a single class' set-only attribute with a setter 
 *  member function.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS_ONLY(NUMBER, TYPE, ATTRIB_NAME, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_NO_GETTER(NUMBER, TYPE)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SETTER_ARGS(NUMBER, TYPE, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS
 *  for getter functions with no bound arguments
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_ONLY(NUMBER, TYPE, ATTRIB_NAME, SETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS_ONLY(NUMBER, TYPE, ATTRIB_NAME, SETTER_NAME, (BOOST_MIRROR_SETTER_SRC), 1)

	
/** This macro declares a single class' set-only attribute with a setter 
 *  member function. Version for typedef'd types.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS_ONLY_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE_NAMESPACE::TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_NO_GETTER(NUMBER, TYPE_NAMESPACE::TYPE)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SETTER_ARGS(NUMBER, TYPE_NAMESPACE::TYPE, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS
 *  for getter functions with no bound arguments. 
 *  Version for typedef'd types.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_ONLY_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, SETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS_ONLY_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, SETTER_NAME, (BOOST_MIRROR_SETTER_SRC), 1)


/** This macro declares a single class' simple-get-setter-set attribute with a setter 
 *  member function and simple value getting scheme.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS(NUMBER, TYPE, ATTRIB_NAME, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, ATTRIB_NAME)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SETTER_ARGS(NUMBER, TYPE, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS
 *  for setter functions with no bound arguments besides the value to be set
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER(NUMBER, TYPE, ATTRIB_NAME, SETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS(NUMBER, TYPE, ATTRIB_NAME, SETTER_NAME, (BOOST_MIRROR_SETTER_SRC), 1)

/** This macro declares a single class' simple-get-setter-set attribute with a setter 
 *  member function and simple value getting scheme.
 *  Version for typedef'd types.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE_NAMESPACE::TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE_NAMESPACE::TYPE, ATTRIB_NAME)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SETTER_ARGS(NUMBER, TYPE_NAMESPACE::TYPE, SETTER_NAME, SETTER_ARGS_TUPLE, ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS
 *  for setter functions with no bound arguments besides the value to be set.
 *  Version for typedef'd types.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, SETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_W_ARGS_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, SETTER_NAME, (BOOST_MIRROR_SETTER_SRC), 1)


/** This macro declares a single class' simple-get-setter-set attribute with a setter 
 *  member function and simple value getting scheme.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER_W_ARGS(\
	NUMBER, \
	TYPE, \
	ATTRIB_NAME, \
	GETTER_NAME, \
	GETTER_ARGS_TUPLE, \
	GETTER_ARG_COUNT, \
	SETTER_NAME, \
	SETTER_ARGS_TUPLE, \
	SETTER_ARG_COUNT\
) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_GETTER_ARGS(NUMBER, TYPE, GETTER_NAME, GETTER_ARGS_TUPLE, GETTER_ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SETTER_ARGS(NUMBER, TYPE, SETTER_NAME, SETTER_ARGS_TUPLE, SETTER_ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER_W_ARGS
 *  for getter and setter functions with no bound arguments
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER(NUMBER, TYPE, ATTRIB_NAME, GETTER_NAME, SETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER_W_ARGS(\
		NUMBER, TYPE, ATTRIB_NAME, \
		GETTER_NAME, (), 0, \
		SETTER_NAME, (BOOST_MIRROR_SETTER_SRC), 1\
	)

/** This macro declares a single class' simple-get-setter-set attribute with a setter 
 *  member function and simple value getting scheme.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER_W_ARGS_TD(\
	NUMBER, \
	TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, \
	ATTRIB_NAME, \
	GETTER_NAME, \
	GETTER_ARGS_TUPLE, \
	GETTER_ARG_COUNT, \
	SETTER_NAME, \
	SETTER_ARGS_TUPLE, \
	SETTER_ARG_COUNT\
) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE_NAMESPACE::TYPE, ATTRIB_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_GETTER_ARGS(NUMBER, TYPE_NAMESPACE::TYPE, GETTER_NAME, GETTER_ARGS_TUPLE, GETTER_ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SETTER_ARGS(NUMBER, TYPE_NAMESPACE::TYPE, SETTER_NAME, SETTER_ARGS_TUPLE, SETTER_ARG_COUNT)\
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME) 

/** Simpler version of the BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER_W_ARGS_TD
 *  for getter and setter functions with no bound arguments
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, GETTER_NAME, SETTER_NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER_W_ARGS(\
		NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, ATTRIB_NAME, \
		GETTER_NAME, (), 0, \
		SETTER_NAME, (BOOST_MIRROR_SETTER_SRC), 1\
	)


} // namespace mirror
} // namespace boost

#endif //include guard

