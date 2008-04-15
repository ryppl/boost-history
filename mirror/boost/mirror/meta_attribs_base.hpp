/**
 * \file boost/mirror/meta_attribs_simple.hpp
 * Registering and reflection of simple class attributes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_ATTRIBS_SIMPLE
#define BOOST_MIRROR_META_ATTRIBS_SIMPLE

// mirror common definitions 
#include <boost/mirror/common_defs.hpp>
// 
// necessary type traits
#include <boost/call_traits.hpp>

namespace boost {
namespace mirror {

/** Defaut (empty) list of base attributes of a meta class
 */
template <class the_class>
struct meta_class_attributes
{
	typedef mpl::vector<> type_list;
};

/** This macro should start the declaration of attributes
 *  of the given class
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(THE_CLASS) \
	template <> struct meta_class_attributes<THE_CLASS> \
	{ \
		typedef THE_CLASS the_class; \
		typedef mpl::vector<> 

/** This is a helper for the BOOST_MIRROR_CLASS_ATTRIB*
 *  macros. 
 *  It declares all common things that need to be declared 
 *  before the specific stuff.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, NAME) \
	_partial_list_##NUMBER;\
	static const bchar* base_name(mpl::int_<NUMBER>){return BOOST_STR_LIT(#NAME);} 

/** This is a helper for the BOOST_MIRROR_CLASS_ATTRIB*
 *  macros. 
 *  It declares all common things that need to be declared 
 *  after the specific stuff.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME) \
	typedef mpl::push_back<_partial_list_##NUMBER, TYPE>::type  

/** Helper macro for implementing simple attrib value getting 
 *  querying scheme
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME) \
	static call_traits<TYPE>::param_type get(const the_class& context, mpl::int_<NUMBER>)\
	{\
		return context.NAME;\
	} \
	static TYPE& query(const the_class& context, mpl::int_<NUMBER>, TYPE& dest)\
	{\
		dest = context.NAME;\
		return dest;\
	} 

/** Helper macro for implementing no-op query meta-class function
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_NO_GETTER(NUMBER, TYPE) \
	static void get(const the_class& context, mpl::int_<NUMBER>){ }\
	static void query(const the_class& context, mpl::int_<NUMBER>, TYPE& dest){ }

/** Helper macros 
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME) \
	static call_traits<TYPE>::param_type set(the_class& context, mpl::int_<NUMBER>, call_traits<TYPE>::param_type val)\
	{\
		context.NAME = val;\
		return val;\
	} \
	static void set(const the_class& context, mpl::int_<NUMBER>, call_traits<TYPE>::param_type val)\
	{\
	}


/** Helper macro for implementing no-op set meta-class function
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_NO_SETTER(NUMBER, TYPE) \
	static void set(the_class& context, mpl::int_<NUMBER>, call_traits<TYPE>::param_type val){ }\
	static void set(const the_class& context, mpl::int_<NUMBER>, call_traits<TYPE>::param_type val){ }


/** This macro declares the meta-data for a single class' attribute
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME) 

/** This macro finishes the declaration of attributes
 *  of the given class
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIBS_END \
	type_list; \
};

} // namespace mirror
} // namespace boost

#endif //include guard

