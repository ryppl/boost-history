/**
 * \file boost/mirror/detail/meta_attribs_base.hpp
 * Registering and reflection of simple class attributes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_DETAIL_META_ATTRIBS_BASE_HPP
#define BOOST_MIRROR_DETAIL_META_ATTRIBS_BASE_HPP

// mirror common definitions 
#include <boost/mirror/common_defs.hpp>
// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// class generation related
#include <boost/mirror/detail/meta_attribs_outline.hpp>
//
#include <boost/preprocessor/repetition/enum_params.hpp>
// 
// necessary type traits
#include <boost/call_traits.hpp>

namespace boost {
namespace mirror {

/** Forward declaration of the meta_class_attributes<> template
 */
template <class Class, class VariantTag = detail::default_meta_class_variant>
struct meta_class_attributes;

/** Defaut (empty) list of base attributes of a meta class
 */
template <class Class, class VariantTag>
struct meta_class_attributes
{
	typedef mpl::vector<> type_list;
};

/** Possible class attribute storage specifiers 
 */
namespace attrib_storage_specifiers {
	// static class attribute 
	struct static_ { };
	// mutable class attribute
	struct mutable_ { };
	// regular class attribure
	struct __ { };
}

/** Additional attribute traits containing information
 *  about storage class specifiers and some information 
 *  about the type of the attribute
 */
template <class Specifiers, class TypeOrTypedefSelector>
struct meta_class_attribute_traits;

/** Specialization for non-static, non-mutable members
 */
template <class TypeOrTypedefSelector>
struct meta_class_attribute_traits<
	attrib_storage_specifiers::__,
	TypeOrTypedefSelector
>
{
	typedef mpl::false_ is_static;
	typedef mpl::false_ is_mutable;
	typedef TypeOrTypedefSelector meta_type_selector;
};

/** Specialization for static member attribs
 */
template <class TypeOrTypedefSelector>
struct meta_class_attribute_traits<
	attrib_storage_specifiers::static_,
	TypeOrTypedefSelector
>
{
	typedef mpl::true_ is_static;
	typedef mpl::false_ is_mutable;
	typedef TypeOrTypedefSelector meta_type_selector;
};

/** Specialization for mutable member attribs
 */
template <class TypeOrTypedefSelector>
struct meta_class_attribute_traits<
	attrib_storage_specifiers::mutable_,
	TypeOrTypedefSelector
>
{
	typedef mpl::false_ is_static;
	typedef mpl::true_ is_mutable;
	typedef TypeOrTypedefSelector meta_type_selector;
};


/** This macro starts the declaration of member attributes
 *  of the given class
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(THE_CLASS) \
	template <> struct meta_class_attributes< \
		THE_CLASS , \
		detail::default_meta_class_variant \
	> \
	{ \
		typedef THE_CLASS Class; \
		typedef detail::default_meta_class_variant variant_tag; \
		typedef mpl::vector<> 

/** This macro starts the declaration of member attributes
 *  of the given template
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_BEGIN( \
	THE_TEMPLATE, \
	TEMPL_ARG_COUNT \
) \
	template < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, typename T) >  \
	struct meta_class_attributes< \
		THE_TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) >, \
		detail::default_meta_class_variant \
	> \
	{ \
		typedef THE_TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) > \
			Class; \
		typedef detail::default_meta_class_variant variant_tag; \
		typedef typename mpl::vector<> 


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


/** Helper macro expanding into a prologue of a meta-attribute
 *  declaration
 */
#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_BASE_PROLOGUE( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME \
) \
	partial_list_##NAME; \
	typedef mpl::int_< mpl::size< partial_list_##NAME >::value > \
		position_of_##NAME; \
	static const bchar* base_name( \
		position_of_##NAME \
	){return BOOST_STR_LIT(#NAME);} \
	static meta_class_attribute_traits< \
		::boost::mirror::attrib_storage_specifiers:: SPECIFIERS##_, \
		TYPE_SELECTOR \
	> get_traits(position_of_##NAME); 

/** Helper macro expanding into an epilogue of a meta-attribute
 *  declaration
 */
#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_EPILOGUE( \
	TYPE_SELECTOR, \
	NAME, \
	TYPENAME_KW \
) typedef TYPENAME_KW mpl::push_back< \
	partial_list_##NAME, \
	TYPENAME_KW ::boost::mirror::typedef_::extract_type< \
		TYPE_SELECTOR \
	>::type \
>::type 

/** Helper macro expanding into the declaraion of getter
 *  function of the meta-attribute
 */
#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_GETTER( \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_BODY, \
	TYPENAME_KW \
)	inline static TYPENAME_KW call_traits< \
		TYPENAME_KW ::boost::mirror::typedef_::extract_type< \
			TYPE_SELECTOR \
		>::type \
	>::param_type get( \
		const Class& instance, \
		position_of_##NAME position \
	) GETTER_BODY

/** Helper macro expanding into the declaration of query
 *  function of the meta-attribute
 */
#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_QUERY( \
	TYPE_SELECTOR, \
	NAME, \
	QUERY_BODY, \
	TYPENAME_KW \
)	template <typename DestType> \
	inline static DestType& query( \
		const Class& instance, \
		position_of_##NAME, \
		DestType& dest \
	) QUERY_BODY 

 /** Helper macro expanding into the declaraion of setter
 *  function of the meta-attribute
 */
#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_SETTER( \
	TYPE_SELECTOR, \
	NAME, \
	SETTER_BODY, \
	TYPENAME_KW \
)	inline static void set( \
		Class& instance, \
		position_of_##NAME, \
		TYPENAME_KW call_traits< \
			TYPENAME_KW ::boost::mirror::typedef_::extract_type< \
				TYPE_SELECTOR \
			>::type \
		>::param_type value \
	) SETTER_BODY \
	inline static void set( \
		const Class& instance, \
		position_of_##NAME, \
		TYPENAME_KW call_traits< \
			TYPENAME_KW ::boost::mirror::typedef_::extract_type< \
				TYPE_SELECTOR \
			>::type \
		>::param_type value \
	) { } 

#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_PROLOGUE( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_BODY, \
	QUERY_BODY, \
	SETTER_BODY, \
	TYPENAME_KW \
) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_BASE_PROLOGUE( \
		SPECIFIERS, \
		TYPE_SELECTOR, \
		NAME \
	) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_GETTER( \
		TYPE_SELECTOR, \
		NAME, \
		GETTER_BODY, \
		TYPENAME_KW \
	) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_QUERY( \
		TYPE_SELECTOR, \
		NAME, \
		QUERY_BODY, \
		TYPENAME_KW \
	) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_SETTER( \
		TYPE_SELECTOR, \
		NAME, \
		SETTER_BODY, \
		TYPENAME_KW \
	) \


/** General macro for registering meta data about class' or template's 
 *  member attribute
 */

#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_BODY, \
	QUERY_BODY, \
	SETTER_BODY, \
	TYPENAME_KW \
) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_PROLOGUE( \
		SPECIFIERS, \
		TYPE_SELECTOR, \
		NAME, \
		GETTER_BODY, \
		QUERY_BODY, \
		SETTER_BODY, \
		TYPENAME_KW \
	) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_EPILOGUE( \
		TYPE_SELECTOR, \
		NAME, \
		TYPENAME_KW \
	) \


/** General macro used for registering meta-data about 
 *  a class' member attribute
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_BODY, \
	QUERY_BODY, \
	SETTER_BODY \
) BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB( \
		SPECIFIERS, \
		TYPE_SELECTOR, \
		NAME, \
		GETTER_BODY, \
		QUERY_BODY, \
		SETTER_BODY, \
		BOOST_PP_EMPTY() \
	)

/** General macro used for registering meta-data about 
 *  a template's member attribute
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_BODY, \
	QUERY_BODY, \
	SETTER_BODY \
) BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB( \
		SPECIFIERS, \
		TYPE_SELECTOR, \
		NAME, \
		GETTER_BODY, \
		QUERY_BODY, \
		SETTER_BODY, \
		typename \
	)

/** Macro used for registering meta-data about class' or template's 
 *  attribute that can be accessed from the outside directly without
 *  getter / setter functions
 */
#define BOOST_MIRROR_REG_SIMPLE_TEMPLATE_OR_CLASS_ATTRIB( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	TYPENAME_KW \
) BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB( \
		SPECIFIERS, TYPE_SELECTOR, NAME, \
		{return instance.NAME;}, \
		{dest = DestType(instance.NAME); return dest;}, \
		{instance.NAME = value;}, \
		TYPENAME_KW \
	)


/** Macro used for registering meta-data about class' attribute
 *  that can be accessed from the outside directly without
 *  getter / setter functions
 */
#define BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME \
) BOOST_MIRROR_REG_SIMPLE_TEMPLATE_OR_CLASS_ATTRIB( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	BOOST_PP_EMPTY() \
)

/** Macro used for registering meta-data about template's attribute
 *  that can be accessed from the outside directly without
 *  getter / setter functions
 */
#define BOOST_MIRROR_REG_SIMPLE_TEMPLATE_ATTRIB( \
	SPECIFIERS, \
	TYPE, \
	NAME \
) BOOST_MIRROR_REG_SIMPLE_TEMPLATE_OR_CLASS_ATTRIB( \
	SPECIFIERS, \
	TYPE, \
	NAME, \
	typename \
)

/** Macro used for registering meta-data about class' or template's 
 *  attribute that can be accessed from the outside by calling
 *  the getter function as specified by GETTER_CALL and setter
 *  function as specified by SETTER_CALL.
 */
#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_GET_SET( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_CALL, \
	SETTER_CALL, \
	TYPENAME_KW \
) BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB( \
		SPECIFIERS, TYPE_SELECTOR, NAME, \
		{return instance.GETTER_CALL;}, \
		{dest = DestType(instance.GETTER_CALL); return dest;}, \
		{instance.SETTER_CALL;}, \
		TYPENAME_KW \
	)


/** Macro used for registering meta-data about class' attribute
 *  that can be accessed from the outside by calling
 *  the getter function as specified by GETTER_CALL and setter
 *  function as specified by SETTER_CALL.
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_GET_SET( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_CALL, \
	SETTER_CALL \
) BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_GET_SET( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_CALL, \
	SETTER_CALL, \
	BOOST_PP_EMPTY() \
)

/** Macro used for registering meta-data about template's attribute
 *  that can be accessed from the outside by calling
 *  the getter function as specified by GETTER_CALL and setter
 *  function as specified by SETTER_CALL.
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_GET_SET( \
	SPECIFIERS, \
	TYPE, \
	NAME, \
	GETTER_CALL, \
	SETTER_CALL \
) BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_GET_SET( \
	SPECIFIERS, \
	TYPE, \
	NAME, \
	GETTER_CALL, \
	SETTER_CALL, \
	typename \
)

#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_WITH_OUTLINE( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME, \
	GETTER_BODY, \
	QUERY_BODY, \
	SETTER_BODY, \
	TYPENAME_KW \
) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_PROLOGUE( \
		SPECIFIERS, \
		TYPE_SELECTOR, \
		NAME, \
		GETTER_BODY, \
		QUERY_BODY, \
		SETTER_BODY, \
		TYPENAME_KW \
	) \
	BOOST_MIRROR_DECLARE_ATTRIB_OUTLINE(TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_EPILOGUE( \
		TYPE_SELECTOR, \
		NAME, \
		TYPENAME_KW \
	) \


#define BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB_OUTLINE( \
	SPECIFIERS, \
	TYPE_SELECTOR, \
	NAME \
) \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_WITH_OUTLINE( \
		SPECIFIERS, \
		TYPE_SELECTOR, \
		NAME, \
		{return instance.NAME;}, \
		{dest = DestType(instance.NAME); return dest;}, \
		{instance.NAME = value;}, \
		BOOST_PP_EMPTY() \
	)


#ifdef NEVER_COMPILE_THIS




/** This macro declares the meta-data for a single class' attribute outline
 */
#define BOOST_MIRROR_REG_CLASS_ATTRIB_OUTLINE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME, false) 

/** This macro declares the meta-data for a single templates attribute
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIB_OUTLINE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_CLASS_ATTRIB_PROLOGUE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_SIMPLE_GET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_DECL_SIMPLE_SET(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_DECLARE_ATTRIB_OUTLINE(NUMBER, TYPE, NAME) \
	BOOST_MIRROR_REG_TEMPLATE_ATTRIB_EPILOGUE(NUMBER, TYPE, NAME, false) 


#endif // NEVER_COMPILE_THIS

} // namespace mirror
} // namespace boost

#endif //include guard

