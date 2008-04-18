/**
 * \file boost/mirror/meta_attribs_simple.hpp
 * Registering and reflection of simple class attributes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_ATTRIBS_SIMPLE_HPP
#define BOOST_MIRROR_META_ATTRIBS_SIMPLE_HPP

// mirror common definitions 
#include <boost/mirror/common_defs.hpp>
// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// 
// necessary type traits
#include <boost/call_traits.hpp>

namespace boost {
namespace mirror {

/** Forward declaration of the meta_class_attributes<> template
 */
template <class the_class, class variant_tag = detail::default_meta_class_variant>
struct meta_class_attributes;

namespace detail {

/** Forward declaration of the meta_class_attrib_utils<>
 *  template.
 */
template <class meta_attributes>
struct meta_class_attrib_utils;

/** Template used in implementation of the 
 *  meta_class_attrib_utils::for_each function
 */
template <class meta_attributes, bool last_one>
struct meta_class_attrib_for_each_impl
{
	template <class meta_attrib_op, class meta_class, class position>
	static void apply(meta_attrib_op, meta_class*, position)
	{
	}
};

template <class meta_attributes>
struct meta_class_attrib_for_each_impl<meta_attributes, false>
{
	template <class meta_attrib_op, class meta_class, int attrib_index>
	static void apply(meta_attrib_op& op, meta_class*, mpl::int_<attrib_index>)
	{
		typedef mpl::int_<attrib_index> position;
		typedef mpl::int_<position::value+1> next;
		// get the type of the attribute
		typedef mpl::at<meta_class::attributes::type_list, position>::type attrib_type;
		//
		// execute the operation
		op((meta_class*)0, position(), (attrib_type*)0);
		// and move to the next one
		meta_class_attrib_for_each_impl<
			meta_attributes,
			is_same<
				mpl::int_<mpl::size<meta_class::attributes::type_list>::value>, 
				next
			>::value 
		>::apply(op, (meta_class*)0, next()); 
	}
};


/** This template implements several functions
 *  for the specializations of the meta_class_attributes<>
 *  template.
 */
template <class the_class, class variant_tag>
struct meta_class_attrib_utils<meta_class_attributes<the_class, variant_tag > >
{
	typedef the_class base_class;
	typedef BOOST_MIRROR_REFLECT_CLASS_VT(the_class, variant_tag) meta_class;
	typedef typename meta_class_attributes<the_class, variant_tag > meta_attributes;
	/** 
	 */
	template <class meta_attrib_op>
	static meta_attrib_op for_each(meta_attrib_op op)
	{
		meta_class_attrib_for_each_impl<
			meta_attributes,
			is_same<
				mpl::int_<mpl::size<meta_class::attributes::type_list>::value>, 
				mpl::int_<0> 
			>::value 
		>::apply(op, (meta_class*)0, mpl::int_<0>());
		return op;
	};
};

} // namespace detail

/** Defaut (empty) list of base attributes of a meta class
 */
template <class the_class, class variant_tag>
struct meta_class_attributes
: public detail::meta_class_attrib_utils<
	meta_class_attributes<the_class, variant_tag>
>
{
	typedef mpl::vector<> type_list;
};

/** This macro starts the declaration of member attributes
 *  of the given class
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(THE_CLASS) \
	template <> struct meta_class_attributes<THE_CLASS, detail::default_meta_class_variant> \
	: public detail::meta_class_attrib_utils<\
		meta_class_attributes<THE_CLASS, detail::default_meta_class_variant> \
	>\
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

/** This is a helper for the BOOST_MIRROR_CLASS_ATTRIB*_TD
 *  macros. 
 *  It declares all common things that need to be declared 
 *  after the specific stuff.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) \
	typedef mpl::push_back<\
		_partial_list_##NUMBER, \
		BOOST_MIRROR_TYPEDEFD_SELECTOR(TYPE_NS_ALIAS##_##TYPE, TYPE_NAMESPACE::TYPE)\
	>::type  

/** Helper macro for implementing simple attrib value getting 
 *  querying scheme
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME) \
	static call_traits<TYPE>::param_type get(const the_class& context, mpl::int_<NUMBER>)\
	{\
		return context.NAME;\
	}\
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

/** This macro declares the meta-data for a single class' typedefd attribute
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) 

/** This macro finishes the declaration of attributes
 *  of the given class
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIBS_END \
	type_list; \
struct size : public mpl::size<type_list>{ };\
};
} // namespace mirror
} // namespace boost

#endif //include guard

