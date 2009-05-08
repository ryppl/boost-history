/**
 * \file boost/mirror/meta_constructors.hpp
 * Meta data concerning class' constructors
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_CONSTRUCTORS_HPP
#define BOOST_MIRROR_META_CONSTRUCTORS_HPP

#include <boost/mirror/detail/function_meta_data.hpp>

namespace boost {
namespace mirror {

/** Forward declaration of the meta-constructors base template
 */
template <class Class /*, class VariantTag*/ >
struct meta_constructors_base;

/** Begins the registering of template class' constructors
 */
#define BOOST_MIRROR_REG_TEMPLATE_CONSTRUCTORS_BEGIN(TEMPLATE, TEMPL_ARG_COUNT)\
BOOST_MIRROR_REG_TEMPLATE_META_FUNCTIONS_BEGIN( \
	TEMPLATE, \
	TEMPL_ARG_COUNT, \
	meta_constructors_base \
)

/** Begins the registering of class' constructors
 */
#define BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN(CLASS) \
BOOST_MIRROR_REG_META_FUNCTIONS_BEGIN(CLASS, meta_constructors_base)

/** Implements the get_function_name helper member function
 *  for the meta-constructor-base class
 */
#define BOOST_MIRROR_REG_CONSTR_IMPL_GET_FUNCTION_NAME(R, DATA, I, CHAR_T) \
        template <bool FullOrBase, class FunctionIndex> \
        inline static const ::std::basic_string<CHAR_T>& \
        get_function_name( \
                mpl::bool_<FullOrBase> full_or_base, \
                const ::std::char_traits<CHAR_T>& traits, \
		FunctionIndex \
        ) \
        { \
		typedef typename get_scope<FunctionIndex>::type scope; \
		return scope::get_name(full_or_base, traits); \
        }


/** Ends the registering of (template) class' constructors
 */
#define BOOST_MIRROR_REG_CONSTRUCTORS_END \
        param_type_lists; \
	BOOST_CTS_FOR_EACH_CHAR_T( \
		BOOST_MIRROR_REG_CONSTR_IMPL_GET_FUNCTION_NAME, \
		_ \
	) \
};

/** Registers a single default constructor
 */
#define BOOST_MIRROR_REG_DEFAULT_CONSTRUCTOR(CONSTR_INDEX) \
        param_type_lists_ ## CONSTR_INDEX ; \
	BOOST_MIRROR_REG_META_FUNCTION_DEFINE_EMPTY_PARAM_TYPELIST(CONSTR_INDEX)\
	BOOST_MIRROR_REG_META_FUNCTION_PUSH_BACK_PARAM_TYPES(\
		CONSTR_INDEX, \
		BOOST_PP_EMPTY()\
	) 

/** registers a single non-default constructor 
 */
#define BOOST_MIRROR_DO_REG_CLASS_OR_TEMPL_CONSTRUCTOR(\
	CONSTR_INDEX, \
	PARAM_SEQ, \
	TYPENAME_KW\
) \
        param_type_lists_ ## CONSTR_INDEX ; \
	BOOST_MIRROR_REG_META_FUNCTION_DEFINE_PARAM_TYPELIST( \
		CONSTR_INDEX, \
		PARAM_SEQ \
	) \
	BOOST_MIRROR_REG_META_FUNCTION_PARAM_NAMES( \
		CONSTR_INDEX, \
		PARAM_SEQ \
	) \
	BOOST_MIRROR_REG_META_FUNCTION_PUSH_BACK_PARAM_TYPES( \
		CONSTR_INDEX, \
		TYPENAME_KW \
	) 


/** expands into the default constructor registering macro
 */
#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR_1( \
        CONSTR_INDEX, \
        PARAM_SEQ, \
        TYPENAME_KW \
) BOOST_MIRROR_REG_DEFAULT_CONSTRUCTOR(CONSTR_INDEX)

/** expands into the non-default constructor registering macro
 */
#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR_0( \
        CONSTR_INDEX, \
        PARAM_SEQ, \
        TYPENAME_KW \
) BOOST_MIRROR_DO_REG_CLASS_OR_TEMPL_CONSTRUCTOR( \
	CONSTR_INDEX, \
	PARAM_SEQ, \
	TYPENAME_KW \
)

/** Registers a single constructor, by disparching between 
 *  the BOOST_MIRROR_DO_REG_CLASS_OR_TEMPL_CONSTRUCTOR and
 *  the BOOST_MIRROR_REG_DEFAULT_CONSTRUCTOR macros based on
 *  whether the constructor is default or non-default.
 *  Default constructors are identified as those
 *  having only a single parameter of void type. All other constructors
 *  are considered as non-default.
 */
#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR( \
	CONSTR_INDEX, \
	PARAM_SEQ, \
	TYPENAME_KW \
) BOOST_PP_CAT( \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR_, \
	BOOST_MIRROR_IS_VOID_FN_ARG_LIST(PARAM_SEQ) \
) (CONSTR_INDEX, PARAM_SEQ, TYPENAME_KW)
	


/** Registers a constructor of a non-template class
 */
#define BOOST_MIRROR_REG_CONSTRUCTOR(CONSTR_INDEX, PARAM_SEQ) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR( \
		CONSTR_INDEX, \
		PARAM_SEQ, \
		BOOST_PP_EMPTY() \
	)
 
/** Registers a constructor of a template class
 */
#define BOOST_MIRROR_REG_TEMPLATE_CONSTRUCTOR(CONSTR_INDEX, PARAM_SEQ) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR( \
		CONSTR_INDEX, \
		PARAM_SEQ, \
		typename \
	)
 
/** Helper macro used to call the BOOST_MIRROR_REG_CONSTRUCTOR 
 *  for each constructor in the quick registering macro
 */
#define BOOST_MIRROR_CALL_REG_CONSTRUCTOR_QREG(R, D, PARAM_SEQ)\
	BOOST_MIRROR_REG_CONSTRUCTOR(BOOST_PP_SUB(R,2), PARAM_SEQ)

/** Quick and simple constructor registering macro
 */
#define BOOST_MIRROR_QREG_CONSTRUCTORS( \
	CLASS, \
	CONSTR_PARAM_SEQ_SEQ \
) \
BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN( CLASS ) \
	BOOST_PP_SEQ_FOR_EACH( \
		BOOST_MIRROR_CALL_REG_CONSTRUCTOR_QREG, \
		_, \
		CONSTR_PARAM_SEQ_SEQ \
	) \
BOOST_MIRROR_REG_CONSTRUCTORS_END


/** Helper macro which registers the constructors of
 *  native C++ types and some other commonly used types
 *  like strings.
 */
#define BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(TYPE)\
BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN( TYPE ) \
        BOOST_MIRROR_REG_DEFAULT_CONSTRUCTOR(0) \
        BOOST_MIRROR_REG_CONSTRUCTOR(1, ((TYPE)(init))) \
BOOST_MIRROR_REG_CONSTRUCTORS_END 

BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(bool)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(char)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(unsigned char)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(signed char)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(wchar_t)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(short int)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(unsigned short int)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(int)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(unsigned int)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(long int)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(unsigned long int)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(float)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(double)

BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(::std::string)
BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(::std::wstring)

#undef BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS

/** Template providing meta-data about the constructors
 *  of the Class.
 */
template <class Class /*, class VariantTag*/ >
struct meta_constructors : public meta_constructors_base<Class>
{
	template <class ConstructorIndex>
	struct constructor
	 : detail::meta_function<meta_constructors, ConstructorIndex>
	{
		typedef BOOST_MIRRORED_CLASS(Class) result_type;
	};

	template <class FunctionIndex>
	struct function : constructor<FunctionIndex>
	{
		typedef mpl::true_ is_constructor;
 	};
};

} // namespace mirror
} // namespace boost

#endif //include guard

