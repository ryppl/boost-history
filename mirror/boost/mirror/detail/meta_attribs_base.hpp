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
#include <boost/mirror/detail/meta_attrib_traits.hpp>
#include <boost/mirror/detail/meta_attribs_outline.hpp>
//
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/vector.hpp>
// 
// necessary type traits
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/alignment_of.hpp>
// enable if
#include <boost/utility/enable_if.hpp>
//
// std::allocator
//#include <memory>

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


#define BOOST_MIRROR_REG_TEMPL_OR_CLASS_ATTRIB_IMPL_GET_NAME(R,NAME,I, CHAR_T)\
        inline static const ::std::basic_string< CHAR_T >& get_name( \
                position_of_##NAME, \
                ::boost::mpl::false_, \
                ::std::char_traits< CHAR_T > \
        ) \
	{ \
		static ::std::basic_string< CHAR_T > name( \
			BOOST_CTS_STRINGIZE_CHAR_T(CHAR_T, NAME) \
		); \
		return name; \
	}
	
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
	BOOST_CTS_FOR_EACH_CHAR_T( \
		BOOST_MIRROR_REG_TEMPL_OR_CLASS_ATTRIB_IMPL_GET_NAME, \
		NAME \
	) \
	static meta_class_attribute_traits< \
		::boost::mirror::attrib_storage_specifiers:: SPECIFIERS##_, \
		TYPE_SELECTOR \
	> get_traits(position_of_##NAME); 

/**
 */
#define BOOST_MIRROR_REG_META_CLASS_GENERATOR_PLUGIN(NAME) \
	template < \
		class MetaClassAttributes, \
		template <class, class, class, class> \
		class MetaFunction \
	> \
	struct NAME##_generator_plugin_mem_typedef \
	{ \
		typedef typename MetaFunction< \
			Class, \
			variant_tag, \
			MetaClassAttributes, \
			position_of_##NAME \
		>::type NAME; \
	}; \
	template < \
		class MetaClassAttributes, \
		template <class, class, class, class> \
		class MetaFunction \
	> \
	static NAME##_generator_plugin_mem_typedef< \
		MetaClassAttributes, \
		MetaFunction \
	> get_generator_plugin(position_of_##NAME, mpl::true_); \
	template < \
		class MetaClassAttributes, \
		template <class, class, class, class> \
		class MetaFunction \
	> \
	struct NAME##_generator_plugin_mem_attrib \
	{ \
		typedef typename MetaFunction< \
			Class, \
			variant_tag, \
			MetaClassAttributes, \
			position_of_##NAME \
		>::type type; \
		type NAME; \
		inline NAME##_generator_plugin_mem_attrib(void){ } \
		template <class Param> \
		inline NAME##_generator_plugin_mem_attrib(Param& param) \
		 : NAME(param) \
		{ } \
		template <class Param> \
		inline NAME##_generator_plugin_mem_attrib(const Param& param) \
		 : NAME(param) \
		{ } \
	}; \
	template < \
		class MetaClassAttributes, \
		template <class, class, class, class> \
		class MetaFunction \
	> \
	static NAME##_generator_plugin_mem_attrib< \
		MetaClassAttributes, \
		MetaFunction \
	> get_generator_plugin(position_of_##NAME, mpl::false_);
	

/** Helper macro expanding into an epilogue of a meta-attribute
 *  declaration
 */
#define BOOST_MIRROR_REG_TEMPLATE_OR_CLASS_ATTRIB_EPILOGUE( \
	TYPE_SELECTOR, \
	NAME, \
	TYPENAME_KW \
) \
	BOOST_MIRROR_REG_META_CLASS_GENERATOR_PLUGIN(NAME) \
	typedef TYPENAME_KW mpl::push_back< \
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
	) 


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

namespace detail {

template <class Class, class MetaAttribsBase>
struct meta_class_attributes_offset_calculator : public MetaAttribsBase
{
protected:
	typedef unsigned char byte;	
	typedef const byte* byte_ptr;	

	struct dummy_instance
	{
		inline Class* ptr(void) const
		{
			return reinterpret_cast<Class*>(
				::boost::alignment_of<Class>::value
			);
		}

		inline Class& ref(void) const
		{
			Class* pointer(ptr());
			return *pointer;
		}
	};

	static inline ptrdiff_t invalid_offset(void)
	{
		return -1;
	}

	typedef MetaAttribsBase base_class;
	static inline ptrdiff_t calculate_offset(
		byte_ptr base_ptr, 
		byte_ptr attr_ptr
	)
	{
		// we are unable to calculate the offset
		if(attr_ptr == 0) return invalid_offset();
		else return attr_ptr - base_ptr;
	}

	template <int I>
	static inline ptrdiff_t& get_offset_storage(mpl::int_<I> pos)
	{
		static ptrdiff_t offs = invalid_offset();
		return offs;
	}

	template <int I>
	static inline bool attrib_is_static(mpl::int_<I> position)
	{
		typedef BOOST_TYPEOF(base_class::get_traits(position))
			traits;
		return traits::is_static::value;
	}


        template <class T, int I>
        static inline ptrdiff_t get_offset_of(
                mpl::int_<I> pos,
		const T* _dummy_ptr,
		typename enable_if<is_pod<T> >::type* _dummy = 0
        )
        {
		// TODO: check whether dereferencing of null_ptr is safe
		// this can be a problem with pod-types
		// meta-types of which have custom implementation
		// of address(...) expecting a valid instance 
		dummy_instance instance;
		//
		// otherwise something like the following will 
		// be necessary:
		// 
		//::std::allocator<T> alloc; // this needs #include <memory>
		//T* pointer((T*)alloc.allocate(1));
		// ...
		// alloc.deallocate(pointer, 1);
                return offset(instance.ref(), pos);
        }

	template <class T, int I>
        static inline ptrdiff_t get_offset_of(
		mpl::int_<I> pos,
		const T* _dummy_ptr,
		typename disable_if<is_pod<T> >::type* _dummy = 0,
		T instance = T()
	)
        { 
		return offset(instance, pos);
	}

	template <class T, int I>
        static inline ptrdiff_t get_offset_of(mpl::int_<I> pos, ...)
	{
		return get_offset_storage(pos);
	}

public:
	/** Gets the byte-offset of the I-th member 
	 */
	template <int I>
	static inline ptrdiff_t	offset(Class& instance, mpl::int_<I> pos)
	{
                if(attrib_is_static(pos)) return invalid_offset();
                ptrdiff_t& result = get_offset_storage(pos);
                if(result == invalid_offset())
                {
                        result = calculate_offset(
                                (byte_ptr)&instance,
                                (byte_ptr)base_class::address(instance, pos)
                        );
                }
                return result;
	}
};


} // namespace detail

/** Declaration of the meta_class_attributes<> template
 */
template <class Class, class VariantTag>
struct meta_class_attributes
 : public detail::meta_class_attributes_offset_calculator<
	Class,
	meta_class_attributes_base<Class, VariantTag>
>
{
private:
	typedef detail::meta_class_attributes_offset_calculator<
	        Class,
	        meta_class_attributes_base<Class, VariantTag>
	> offs_calc;
	typedef typename offs_calc::byte byte;
	typedef typename offs_calc::byte_ptr byte_ptr;

public:
	template <int I>
        static inline ptrdiff_t offset_of(mpl::int_<I> pos)
        { 
		return get_offset_of(pos, (Class*)0);
        } 
};

		

} // namespace mirror
} // namespace boost

#endif //include guard
