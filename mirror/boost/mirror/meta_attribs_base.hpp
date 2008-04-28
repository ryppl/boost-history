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
#include <boost/preprocessor/repetition/enum_params.hpp>
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

/** Implementation of the for_each function on meta_attributes
 */
template <class meta_class, class meta_attributes>
struct meta_class_attrib_for_each
{
protected:
	template <class meta_attrib_op, int I>
	static void do_apply_to(meta_attrib_op op, mpl::int_<I> pos)
	{
		typedef typename meta_attributes::type_list type_list;
		typedef typename mpl::at<type_list, mpl::int_<I> >::type attrib_type;
		meta_class mc;
		meta_attributes ma;
		op(mc, ma, pos, (attrib_type*)0);
	}

	template <class meta_attrib_op>
	static void apply_to(meta_attrib_op op, mpl::int_< -1 > pos){ }

	template <class meta_attrib_op>
	static void apply_to(meta_attrib_op op, mpl::int_<0> pos)
	{
		do_apply_to(op, pos);
	}

	template <class meta_attrib_op, int I>
	static void apply_to(meta_attrib_op op, mpl::int_<I> pos)
	{
		apply_to(op, mpl::int_<I - 1>()), 
		do_apply_to(op, pos);
	}
public:

	/** Performs op((meta_class*)0, mpl::int_<I>, 'type-of-I-th'*0
	 */
	template <class meta_attrib_op>
	static meta_attrib_op for_each(meta_attrib_op op)
	{
		typedef typename meta_attributes::type_list type_list;
		typedef mpl::int_<mpl::size<type_list>::value - 1> last;
		apply_to(op, last());
		return op;
	}
};

/** This template ties together several function implementations
 *  for the specializations of the meta_class_attributes<>
 *  template.
 */
template <class meta_class, class meta_attribs>
struct meta_class_attrib_utils
: meta_class_attrib_for_each<meta_class, meta_attribs>
{ };

} // namespace detail

/** Defaut (empty) list of base attributes of a meta class
 */
template <class the_class, class variant_tag>
struct meta_class_attributes
: public detail::meta_class_attrib_utils<
	meta_class<the_class, variant_tag>,
	meta_class_attributes<the_class, variant_tag>
>
{
	typedef mpl::vector<> type_list;
};

/** This macro starts the declaration of member attributes
 *  of the given class
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(THE_CLASS) \
	template <> struct meta_class_attributes< THE_CLASS , detail::default_meta_class_variant> \
	: public detail::meta_class_attrib_utils<\
		meta_class<THE_CLASS, detail::default_meta_class_variant>, \
		meta_class_attributes<THE_CLASS, detail::default_meta_class_variant> \
	>\
	{ \
		typedef THE_CLASS the_class; \
		typedef mpl::vector<> 

/** This macro starts the declaration of member attributes
 *  of the given template
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_BEGIN(THE_TEMPLATE, TEMPL_ARG_COUNT) \
	template < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, typename T) >  \
	struct meta_class_attributes< \
		THE_TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) >, \
		detail::default_meta_class_variant \
	> \
	: public detail::meta_class_attrib_utils<\
		meta_class< \
			THE_TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) >, \
			detail::default_meta_class_variant \
		>, \
		meta_class_attributes< \
			THE_TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) >, \
			detail::default_meta_class_variant \
		> \
	>\
	{ \
		typedef THE_TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) > \
			the_class; \
		typedef typename mpl::vector<> 


/** This is a helper for the BOOST_MIRROR_CLASS_ATTRIB*
 *  macros. 
 *  It declares all common things that need to be declared 
 *  before the specific stuff.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, NAME) \
	_partial_list_##NUMBER;\
	static const bchar* base_name(mpl::int_<NUMBER>){return BOOST_STR_LIT(#NAME);}\

/** This is a helper for the BOOST_MIRROR_CLASS_ATTRIB*
 *  macros. 
 *  It declares all common things that need to be declared 
 *  after the specific stuff.
 */
#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME, TYPENAME_KW) \
	typedef TYPENAME_KW mpl::push_back<_partial_list_##NUMBER, TYPE>::type  


#define BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME, BOOST_PP_EMPTY())

#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME, typename)


/** This is a helper for the BOOST_MIRROR_CLASS_ATTRIB*_TD
 *  macros. 
 *  It declares all common things that need to be declared 
 *  after the specific stuff.
 */
#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME, TYPENAME_KW) \
	typedef TYPENAME_KW mpl::push_back<\
		_partial_list_##NUMBER, \
		BOOST_MIRROR_TYPEDEFD_SELECTOR(TYPE_NS_ALIAS##_##TYPE, TYPE_NAMESPACE::TYPE)\
	>::type  

#define BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME, BOOST_PP_EMPTY())

#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME, typename)

/** Helper macro for implementing simple attrib value getting 
 *  querying scheme
 */
#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME, TYPENAME_KW) \
	static TYPENAME_KW call_traits<TYPE>::param_type get(const the_class& context, mpl::int_<NUMBER>)\
	{\
		return context.NAME;\
	}\
	template <typename dest_type>\
	static dest_type& query(const the_class& context, mpl::int_<NUMBER>, dest_type& dest)\
	{\
		dest = dest_type(context.NAME);\
		return dest;\
	} 

#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME)\
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME, BOOST_PP_EMPTY())
	
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME)\
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME, typename)
	

/** Helper macro for implementing no-op query meta-class function
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_NO_GETTER(NUMBER, TYPE) \
	static void get(const the_class& context, mpl::int_<NUMBER>){ }\
	template <typename dest_type>\
	static void query(const the_class& context, mpl::int_<NUMBER>, dest_type& dest){ }

#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_NO_GETTER(NUMBER, TYPE) \


/** Helper macros 
 */
#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME, TYPENAME_KW) \
	static void set(the_class& context, mpl::int_<NUMBER>, TYPENAME_KW call_traits<TYPE>::param_type val)\
	{\
		context.NAME = val;\
	} \
	static void set(const the_class& context, mpl::int_<NUMBER>, TYPENAME_KW call_traits<TYPE>::param_type val)\
	{\
	}

#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME, BOOST_PP_EMPTY())

#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME, typename)


/** Helper macro for implementing no-op set meta-class function
 */
#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_NO_SETTER(NUMBER, TYPE, TYPENAME_KW) \
	static void set(the_class& context, mpl::int_<NUMBER>, call_traits<TYPE>::param_type val){ }\
	static void set(const the_class& context, mpl::int_<NUMBER>, call_traits<TYPE>::param_type val){ }

#define BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_NO_SETTER(NUMBER, TYPE) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_NO_SETTER(NUMBER, TYPE, BOOST_PP_EMPTY())

#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_NO_SETTER(NUMBER, TYPE) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_ATTRIB_DECL_NO_SETTER(NUMBER, TYPE, typename)

/** This macro declares the meta-data for a single class' attribute
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME) 

/** This macro declares the meta-data for a single templates attribute
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME) 

/** This macro declares the meta-data for a single class' typedefd attribute
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) 

/** This macro declares the meta-data for a single templates typedefd attribute
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE_NAMESPACE::TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_EPILOGUE_TD(NUMBER, TYPE_NS_ALIAS, TYPE_NAMESPACE, TYPE, NAME) 

/** This macro finishes the declaration of attributes
 *  of the given class
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIBS_END \
		type_list; \
		struct size : public mpl::size<type_list>{ };\
	};

/** This macro finishes the declaration of attributes
 *  of the given template
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_END \
	BOOST_MIRROR_REG_CLASS_ATTRIBS_END


} // namespace mirror
} // namespace boost

#endif //include guard

