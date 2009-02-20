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
#include <boost/mpl/vector.hpp>
// 
// necessary type traits
#include <boost/call_traits.hpp>

namespace boost {
namespace mirror {

namespace detail {

/** Allows to detect the type of the member attribute.
 *  Thanks to Jean-Louis Leroy for suggesting this
 */
template<typename Class, typename Type>
Type detect_class_member_attrib_type(Type (Class::*));

template <typename Type>
Type detect_class_member_attrib_type(Type*);


} // namespace detail

template <class Class, class VariantTag = detail::default_meta_class_variant>
struct meta_class_attributes_base;


/** Defaut (empty) list of base attributes of a meta class
 */
template <class Class, class VariantTag>
struct meta_class_attributes_base
{
	typedef mpl::vector0<> type_list;
	typedef ::boost::mirror::meta_class<Class, VariantTag>
		scope;
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
	template <> struct meta_class_attributes_base< \
		THE_CLASS , \
		detail::default_meta_class_variant \
	> \
	{ \
		typedef THE_CLASS Class; \
		typedef detail::default_meta_class_variant variant_tag; \
		typedef ::boost::mirror::meta_class<Class, variant_tag> \
			scope; \
		typedef mpl::vector<> 

/** This macro starts the declaration of member attributes
 *  of the given template
 */
#define BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_BEGIN( \
	THE_TEMPLATE, \
	TEMPL_ARG_COUNT \
) \
	template < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, typename T) >  \
	struct meta_class_attributes_base< \
		THE_TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) >, \
		detail::default_meta_class_variant \
	> \
	{ \
		typedef THE_TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) > \
			Class; \
		typedef detail::default_meta_class_variant variant_tag; \
		typedef ::boost::mirror::meta_class<Class, variant_tag> \
			scope; \
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
	NAME, \
	TYPENAME_KW \
) \
	partial_list_##NAME; \
	typedef mpl::int_< mpl::size< partial_list_##NAME >::value > \
		position_of_##NAME; \
	typedef TYPENAME_KW ::boost::mirror::typedef_::extract_type< \
		TYPE_SELECTOR \
	>::type type_of_##NAME; \
	inline static const ::std::string& get_name( \
		position_of_##NAME, \
		::boost::mpl::false_, \
		::std::char_traits<char> \
	){static ::std::string name(#NAME); return name;}\
	inline static const ::std::wstring& get_name( \
		position_of_##NAME, \
		::boost::mpl::false_, \
		::std::char_traits<wchar_t> \
	){static ::std::wstring name(L ## #NAME); return name;}\
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
	type_of_##NAME \
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
		type_of_##NAME \
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
	) { QUERY_BODY return dest;}

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
			type_of_##NAME \
		>::param_type value \
	) SETTER_BODY \

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
		NAME, \
		TYPENAME_KW \
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
	static inline type_of_##NAME* address( \
		Class& instance, \
		position_of_##NAME position \
	){return NULL;} \
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
) 	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_PROLOGUE( \
		SPECIFIERS, \
		TYPE_SELECTOR, \
		NAME, \
		{return instance.NAME;}, \
		{dest = DestType(instance.NAME);}, \
		{instance.NAME = value;}, \
		TYPENAME_KW \
	) \
	static inline type_of_##NAME* address( \
		Class& instance, \
		position_of_##NAME position \
	){return &instance.NAME;} \
	BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_EPILOGUE( \
		TYPE_SELECTOR, \
		NAME, \
		TYPENAME_KW \
	) 
/*
*/


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


/** Macro used for registering meta-data about class' attribute
 *  that can be accessed from the outside directly without
 *  getter / setter functions. This version automatically
 *  detects the type of the member attribute .
 *  Thanks to Jean-Louis Leroy for suggesting the auto-detection
 *  mechanism.
 */
#define BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(\
	SPECIFIERS, \
	NAME \
) BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB( \
	SPECIFIERS, \
	BOOST_TYPEOF( ::boost::mirror::detail::detect_class_member_attrib_type( \
	&Class :: NAME)), \
	NAME \
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
		{dest = DestType(instance.GETTER_CALL);}, \
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
		{dest = DestType(instance.NAME);}, \
		{instance.NAME = value;}, \
		BOOST_PP_EMPTY() \
	)

/** Declaration of the meta_class_attributes<> template
 */
template <class Class, class VariantTag = detail::default_meta_class_variant>
struct meta_class_attributes
 : public meta_class_attributes_base<Class, VariantTag>
{
private:
	typedef meta_class_attributes_base<Class, VariantTag> base_class;
	static inline ptrdiff_t calculate_offset(
		const unsigned char* base_ptr, 
		const unsigned char* attr_ptr
	)
	{
		// we are unable to calculate the offset
		if(attr_ptr == 0) return -1;
		else return attr_ptr - base_ptr;
	}
public:
	/** Gets the byte-offset of the I-th member 
	 */
	template <class Owner, int I>
	static inline ptrdiff_t	offset(Owner& instance, mpl::int_<I> pos)
	{
		return calculate_offset(
			(const unsigned char*)&instance,
			(const unsigned char*)base_class::address(instance, pos)
		);
	}
};

		

} // namespace mirror
} // namespace boost

#endif //include guard

