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

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/push_back.hpp>

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/wstringize.hpp>
#include <boost/preprocessor/logical/and.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/array/elem.hpp>

#include <boost/char_type_switch/string.hpp>
#include <boost/mirror/meta_data_fwd.hpp>

namespace boost {
namespace mirror {

/** Forward declaration of the meta-constructors base template
 */
template <class Class /*, class VariantTag*/ >
struct meta_constructors_base;

/** Begins the registering of template class' constructors
 */
#define BOOST_MIRROR_REG_TEMPLATE_CONSTRUCTORS_BEGIN(TEMPLATE, TEMPL_ARG_COUNT)\
template < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, typename T) >  \
struct meta_constructors_base< \
	TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) > \
> \
{ \
        typedef mpl::vector0<>


/** Begins the registering of class' constructors
 */
#define BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN(CLASS) \
template <> \
struct meta_constructors_base< CLASS > \
{ \
        typedef mpl::vector0<>

/** Registers the parameter name of the j-th parameter
 *  of the i-th constructor
 */
#define BOOST_MIRROR_IMPLEMENT_CONSTRUCTOR_GET_PARAM_NAME( \
	CONSTRUCTOR, \
	PARAM, \
	NAME, \
	CHAR_T \
) \
inline static const ::std::basic_string< CHAR_T >& get_param_name( \
        mpl::false_, \
        const ::std::char_traits< CHAR_T >&, \
        mpl::int_< CONSTRUCTOR >, \
        mpl::int_< PARAM > \
) \
{ \
        static ::std::basic_string< CHAR_T > result( \
		BOOST_CTS_STRINGIZE_CHAR_T( CHAR_T, NAME ) \
	); \
        return result; \
} 

#define BOOST_MIRROR_IMPLEMENT_CONSTRUCTOR_GET_PARAM_NAME_HELPER(\
	R, PARAMS, I, CHAR_T \
) BOOST_MIRROR_IMPLEMENT_CONSTRUCTOR_GET_PARAM_NAME( \
	BOOST_PP_ARRAY_ELEM(0, PARAMS), \
	BOOST_PP_ARRAY_ELEM(1, PARAMS), \
	BOOST_PP_ARRAY_ELEM(2, PARAMS), \
	CHAR_T \
)

#define BOOST_MIRROR_REG_CONSTRUCTOR_PARAM_NAME(R,CONSTRUCTOR, PARAM, NAME) \
	BOOST_CTS_FOR_EACH_CHAR_T_2( \
		BOOST_MIRROR_IMPLEMENT_CONSTRUCTOR_GET_PARAM_NAME_HELPER, \
		(3, (CONSTRUCTOR, PARAM, NAME)) \
	)

/** Ends the registering of (template) class' constructors
 */
#define BOOST_MIRROR_REG_CONSTRUCTORS_END \
        param_type_lists; \
};


/** Helper macro which expands into the type of the j-th parameter
 *  preceeded by a comma if it is not the first parameter
 */
#define BOOST_MIRROR_REG_CONSTR_EXTRACT_PARAM_TYPE( \
	R, X, \
	PARAM_INDEX, \
	TYPE_AND_NAME \
) BOOST_PP_COMMA_IF(PARAM_INDEX) BOOST_PP_SEQ_HEAD(TYPE_AND_NAME)



/** Calls the BOOST_MIRROR_REG_CONSTRUCTOR_PARAM_NAME macro
 *  in repetitions.
 */
#define BOOST_MIRROR_REG_CONSTR_REG_CALL_PARAM_NAME(\
	R, \
	CONSTR_INDEX, \
	PARAM_INDEX, \
	TYPE_AND_NAME \
) BOOST_MIRROR_REG_CONSTRUCTOR_PARAM_NAME( \
		R, \
                CONSTR_INDEX, \
                PARAM_INDEX, \
                BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(TYPE_AND_NAME)) \
        )


/** Adds the parameter typelist to the list storing 
 *  storing the parameter types for the individual constructors
 */
#define BOOST_MIRROR_REG_CONSTRUCTOR_PUSH_BACK_PARAM_TYPES( \
	CONSTR_INDEX, \
	TYPENAME_KW\
) 	typedef TYPENAME_KW mpl::push_back< \
                BOOST_PP_CAT(param_type_lists_, CONSTR_INDEX), \
                BOOST_PP_CAT(BOOST_PP_CAT(constr_, CONSTR_INDEX), _params) \
	>::type


/** Registers a single default constructor
 */
#define BOOST_MIRROR_REG_DEFAULT_CONSTRUCTOR(CONSTR_INDEX) \
        param_type_lists_ ## CONSTR_INDEX ; \
        typedef mpl::vector0< \
        > BOOST_PP_CAT(BOOST_PP_CAT(constr_, CONSTR_INDEX), _params) ;\
	BOOST_MIRROR_REG_CONSTRUCTOR_PUSH_BACK_PARAM_TYPES(\
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
        typedef BOOST_PP_CAT(mpl::vector, BOOST_PP_SEQ_SIZE(PARAM_SEQ)) < \
                BOOST_PP_SEQ_FOR_EACH_I( \
			BOOST_MIRROR_REG_CONSTR_EXTRACT_PARAM_TYPE, \
			0, \
			PARAM_SEQ \
		) \
        > BOOST_PP_CAT(BOOST_PP_CAT(constr_, CONSTR_INDEX), _params) ;\
        BOOST_PP_SEQ_FOR_EACH_I( \
		BOOST_MIRROR_REG_CONSTR_REG_CALL_PARAM_NAME, \
		CONSTR_INDEX, \
		PARAM_SEQ \
	) \
	BOOST_MIRROR_REG_CONSTRUCTOR_PUSH_BACK_PARAM_TYPES( \
		CONSTR_INDEX, \
		TYPENAME_KW \
	) 



/** Decides whether the PARAM_SEQ is a argument sequence for 
 *  a default constructor (having no parameters)
 */
#define BOOST_MIRROR_IS_VOID_FN_ARG_LIST(PARAM_SEQ) \
	BOOST_PP_AND( \
		BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(PARAM_SEQ),1), \
		BOOST_PP_EQUAL( \
			BOOST_PP_SEQ_SIZE( \
				BOOST_PP_SEQ_HEAD(PARAM_SEQ) \
			),1 \
		) \
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
	typedef meta_constructors_base<Class> base_class;
	template <class ConstructorIndex>
	struct constructor
	{
		struct params
		{
		private:
			typedef typename mpl::at<
				typename base_class::param_type_lists,
				ConstructorIndex
			>::type type_list;
		public:
			template <class ParamIndex>
			struct param
			{
			private:
				typedef typename mpl::at<
					type_list, 
					ParamIndex
				>::type param_type;
			public:
				typedef BOOST_MIRRORED_TYPE(param_type)
					type;

				template <class CharT>
				inline static const ::std::basic_string<CharT>& 
				get_name(
					mpl::false_ full_or_base,
					const ::std::char_traits<CharT>& traits
				)
				{
					return meta_constructors::get_param_name(
						full_or_base, 
						traits,
						ConstructorIndex(),
						ParamIndex()
					);
				}

				inline static const cts::bstring& base_name(void)
				{
	                                return meta_constructors::get_param_name(
	                                        mpl::false_(),
	                                        cts::bchar_traits(),
	                                        ConstructorIndex(),
	                                        ParamIndex()
	                                );
				}
			};

			typedef mpl::int_<
				mpl::size< type_list>::value
			> size;

			template <class Functor>
			static void for_each(Functor f)
			{
				call_for_each(f, mpl::int_<0>());
			}
		private:
			template <class Functor, class ParamIndex>
			static inline void call_for_each(
				Functor func, 
				ParamIndex pi
			)
			{
				func((params*)0, pi);
				call_for_each(
					func, 
					typename mpl::next<ParamIndex>::type()
				);
			}

			template <class Functor>
			static inline void call_for_each(const Functor&, size)
			{
			}
		};
	};
};

} // namespace mirror
} // namespace boost

#endif //include guard

