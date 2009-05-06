/**
 * \file boost/mirror/detail/function_meta_data.hpp
 *  
 *  Base template used in meta-constructors, meta-member-functions
 *  
 *  Copyright 2009 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_FUNCTION_META_DATA_HPP
#define BOOST_MIRROR_META_DETAIL_FUNCTION_META_DATA_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
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
namespace detail {

template <class BaseMetaData, class FunctionIndex>
struct meta_function
{
	typedef typename BaseMetaData::template get_scope<FunctionIndex>::type
		scope;
	// returns the name of the member function
	template <bool FullOrBase, class CharT>
	inline static const ::std::basic_string<CharT>&
	get_name(
		mpl::bool_<FullOrBase> full_or_base,
		const ::std::char_traits<CharT>& traits
	)
	{
		return BaseMetaData::get_function_name(
			full_or_base,
			traits,
			FunctionIndex()
		);
	}

	// returns the base name of the member function
	inline static const cts::bstring&
	base_name(void)
	{
		return BaseMetaData::get_function_name(
			mpl::false_(),
			cts::bchar_traits(),
			FunctionIndex()
		);
	}


	// meta-data about the parameters of the function
	struct params
	{
	private:
		typedef typename mpl::at<
			typename BaseMetaData::param_type_lists,
			FunctionIndex
		>::type type_list;
	public:
		// some meta-data about the i-th param
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
				return BaseMetaData::get_param_name(
					full_or_base,
					traits,
					FunctionIndex(),
					ParamIndex()
				);
			}

			inline static const cts::bstring& base_name(void)
			{
				return BaseMetaData::get_param_name(
					mpl::false_(),
					cts::bchar_traits(),
					FunctionIndex(),
					ParamIndex()
				);
			}
		};

		// the count of parameters
		typedef mpl::int_<
			mpl::size< type_list>::value
		> size;

		// executes the given binary functor on all
		// parameters of the i-th function
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

/** Registers the parameter name of the j-th parameter
 *  of the i-th constructor
 */
#define BOOST_MIRROR_IMPLEMENT_META_FUNCTION_GET_PARAM_NAME( \
        FUNCTION, \
        PARAM, \
        NAME, \
        CHAR_T \
) \
inline static const ::std::basic_string< CHAR_T >& get_param_name( \
        mpl::false_, \
        const ::std::char_traits< CHAR_T >&, \
        mpl::int_< FUNCTION >, \
        mpl::int_< PARAM > \
) \
{ \
        static ::std::basic_string< CHAR_T > result( \
                BOOST_CTS_STRINGIZE_CHAR_T( CHAR_T, NAME ) \
        ); \
        return result; \
}

/** Helper macro which unpacks the parameters before
 *  calling the BOOST_MIRROR_IMPLEMENT_FUNCTION_GET_PARAM_NAME
 *  macro
 */
#define BOOST_MIRROR_IMPLEMENT_META_FUNCTION_GET_PARAM_NAME_HELPER(\
        R, \
	PARAMS, \
	I, \
	CHAR_T \
) BOOST_MIRROR_IMPLEMENT_META_FUNCTION_GET_PARAM_NAME( \
        BOOST_PP_ARRAY_ELEM(0, PARAMS), \
        BOOST_PP_ARRAY_ELEM(1, PARAMS), \
        BOOST_PP_ARRAY_ELEM(2, PARAMS), \
        CHAR_T \
)

/** Registers the function parameter name for all character types
 */
#define BOOST_MIRROR_REG_META_FUNCTION_PARAM_NAME_HELPER( \
        R, \
        FUNCTION, \
        PARAM, \
        NAME\
) BOOST_CTS_FOR_EACH_CHAR_T_2( \
        BOOST_MIRROR_IMPLEMENT_META_FUNCTION_GET_PARAM_NAME_HELPER, \
        (3, (FUNCTION, PARAM, NAME)) \
)

/** Helper macro which expands into the type of the j-th parameter
 *  preceeded by a comma if it is not the first parameter
 */
#define BOOST_MIRROR_REG_META_FUNCTION_EXTRACT_PARAM_TYPE( \
        R, X, \
        PARAM_INDEX, \
        TYPE_AND_NAME \
) BOOST_PP_COMMA_IF(PARAM_INDEX) BOOST_PP_SEQ_HEAD(TYPE_AND_NAME)



/** Calls the BOOST_MIRROR_REG_META_FUNCTION_PARAM_NAME macro
 *  in repetitions.
 */
#define BOOST_MIRROR_REG_META_FUNCTION_PARAM_NAME_HELPER_2(\
        R, \
        FUNCTION_INDEX, \
        PARAM_INDEX, \
        TYPE_AND_NAME \
) BOOST_MIRROR_REG_META_FUNCTION_PARAM_NAME_HELPER( \
        R, \
        FUNCTION_INDEX, \
        PARAM_INDEX, \
        BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(TYPE_AND_NAME)) \
)

#define BOOST_MIRROR_REG_META_FUNCTION_PARAM_NAMES( \
        FUNCTION_INDEX, \
        PARAM_SEQ \
) BOOST_PP_SEQ_FOR_EACH_I( \
        BOOST_MIRROR_REG_META_FUNCTION_PARAM_NAME_HELPER_2, \
        FUNCTION_INDEX, \
        PARAM_SEQ \
) \

/** Adds the parameter typelist to the list storing
 *  storing the parameter types for the individual constructors
 */
#define BOOST_MIRROR_REG_META_FUNCTION_PUSH_BACK_PARAM_TYPES( \
        FUNCTION_INDEX, \
        TYPENAME_KW \
)       typedef TYPENAME_KW mpl::push_back< \
                BOOST_PP_CAT(param_type_lists_, FUNCTION_INDEX), \
                BOOST_PP_CAT(BOOST_PP_CAT(function_, FUNCTION_INDEX), _params)\
        >::type

/** Decides whether the PARAM_SEQ is a argument sequence for
 *  a function without parameters
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

#define BOOST_MIRROR_REG_META_FUNCTIONS_IMPLEMENT_GET_SCOPE() \
        template <class FunctionIndex> \
        struct get_scope \
        { \
                typedef BOOST_MIRRORED_CLASS(_detail_class) type; \
        }; 
 

/** Begins the registering of template class' members
 */
#define BOOST_MIRROR_REG_TEMPLATE_META_FUNCTIONS_BEGIN( \
        TEMPLATE, \
        TEMPL_ARG_COUNT, \
        META_FUNCTIONS_BASE \
)\
template < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, typename T) >  \
struct META_FUNCTIONS_BASE< \
        TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) > \
> \
{ \
	typedef TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) > \
		_detail_class; \
	BOOST_MIRROR_REG_META_FUNCTIONS_IMPLEMENT_GET_SCOPE() \
	typedef mpl::vector0<>


/** Begins the registering of class' constructors
 */
#define BOOST_MIRROR_REG_META_FUNCTIONS_BEGIN(CLASS, META_FUNCTIONS_BASE) \
template <> \
struct META_FUNCTIONS_BASE < CLASS > \
{ \
	typedef CLASS _detail_class; \
	BOOST_MIRROR_REG_META_FUNCTIONS_IMPLEMENT_GET_SCOPE() \
        typedef mpl::vector0<>

#define BOOST_MIRROR_REG_META_FUNCTION_ENUM_PARAM_TYPES(PARAM_SEQ) \
	BOOST_PP_SEQ_FOR_EACH_I( \
        	BOOST_MIRROR_REG_META_FUNCTION_EXTRACT_PARAM_TYPE, \
		0, \
		PARAM_SEQ \
	) 

#define BOOST_MIRROR_REG_META_FUNCTION_DEFINE_PARAM_TYPELIST( \
	FUNC_INDEX, \
	PARAM_SEQ \
) \
typedef BOOST_PP_CAT(mpl::vector, BOOST_PP_SEQ_SIZE(PARAM_SEQ)) < \
BOOST_MIRROR_REG_META_FUNCTION_ENUM_PARAM_TYPES(PARAM_SEQ) \
> BOOST_PP_CAT(BOOST_PP_CAT(function_, FUNC_INDEX), _params) ;

#define BOOST_MIRROR_REG_META_FUNCTION_DEFINE_EMPTY_PARAM_TYPELIST(FUNC_INDEX) \
        typedef mpl::vector0< \
        > BOOST_PP_CAT(BOOST_PP_CAT(function_, FUNC_INDEX), _params) ;


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

