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
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/empty.hpp>

namespace boost {
namespace mirror {

template <class Class /*, class VariantTag*/ >
struct meta_constructors;


#define BOOST_MIRROR_REG_TEMPLATE_CONSTRUCTORS_BEGIN(TEMPLATE, TEMPL_ARG_COUNT) \
template < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, typename T) >  \
struct meta_constructors< \
	TEMPLATE < BOOST_PP_ENUM_PARAMS(TEMPL_ARG_COUNT, T) > \
> \
{ \
        typedef mpl::vector0<>


#define BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN(CLASS) \
template <> \
struct meta_constructors< CLASS > \
{ \
        typedef mpl::vector0<>

#define BOOST_MIRROR_REG_CONSTRUCTOR_PARAM_NAME(CONSTRUCTOR, PARAM, NAME) \
inline static const ::std::string& get_param_name( \
        mpl::false_, \
        const ::std::char_traits<char>&, \
        mpl::int_< CONSTRUCTOR >, \
        mpl::int_< PARAM > \
) \
{ \
        static ::std::string result( BOOST_PP_STRINGIZE( NAME ) ); \
        return result; \
} \
inline static const ::std::wstring& get_param_name( \
        mpl::false_, \
        const ::std::char_traits<wchar_t>&, \
        mpl::int_< CONSTRUCTOR >, \
        mpl::int_< PARAM > \
) \
{ \
        static ::std::wstring result( BOOST_PP_WSTRINGIZE( NAME ) ); \
        return result; \
}

#define BOOST_MIRROR_REG_CONSTRUCTORS_END \
        param_type_lists; \
        template <class ConstrIndex, class ParamIndex> \
        inline static const cts::bstring& base_param_name(ConstrIndex ci, ParamIndex pi) \
        {\
                return get_param_name( \
                        mpl::false_(), \
                        ::std::char_traits< cts::bchar >(), \
                        ci, \
                        pi \
                ); \
        } \
};

#define BOOST_MIRROR_REG_CONSTR_EXTRACT_PARAM_TYPE(R, X, TYPE_AND_NAME) \
        BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(R, 2) ) \
        BOOST_PP_SEQ_HEAD(TYPE_AND_NAME)


#define BOOST_MIRROR_REG_CONSTR_REG_CALL_PARAM_NAME(R, CONSTR_INDEX, TYPE_AND_NAME) \
        BOOST_MIRROR_REG_CONSTRUCTOR_PARAM_NAME( \
                CONSTR_INDEX, \
                R-2, \
                BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(TYPE_AND_NAME)) \
        )

#define BOOST_MIRROR_REG_DEFAULT_CONSTRUCTOR(CONSTR_INDEX) \
        param_type_lists_ ## CONSTR_INDEX ; \
        typedef mpl::vector0< \
        > BOOST_PP_CAT(BOOST_PP_CAT(constr_, CONSTR_INDEX), _params) ;\
        typedef mpl::push_back< \
                BOOST_PP_CAT(param_type_lists_, CONSTR_INDEX), \
                BOOST_PP_CAT(BOOST_PP_CAT(constr_, CONSTR_INDEX), _params) \
        >::type

#define BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR(CONSTR_INDEX, PARAM_SEQ, TYPENAME_KW) \
        param_type_lists_ ## CONSTR_INDEX ; \
        typedef BOOST_PP_CAT(mpl::vector, BOOST_PP_SEQ_SIZE(PARAM_SEQ)) < \
                BOOST_PP_SEQ_FOR_EACH(BOOST_MIRROR_REG_CONSTR_EXTRACT_PARAM_TYPE, 0, PARAM_SEQ) \
        > BOOST_PP_CAT(BOOST_PP_CAT(constr_, CONSTR_INDEX), _params) ;\
        BOOST_PP_SEQ_FOR_EACH(BOOST_MIRROR_REG_CONSTR_REG_CALL_PARAM_NAME, CONSTR_INDEX, PARAM_SEQ) \
        typedef TYPENAME_KW mpl::push_back< \
                BOOST_PP_CAT(param_type_lists_, CONSTR_INDEX), \
                BOOST_PP_CAT(BOOST_PP_CAT(constr_, CONSTR_INDEX), _params) \
        >::type

#define BOOST_MIRROR_REG_CONSTRUCTOR(CONSTR_INDEX, PARAM_SEQ) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR(CONSTR_INDEX, PARAM_SEQ, BOOST_PP_EMPTY())
 
#define BOOST_MIRROR_REG_TEMPLATE_CONSTRUCTOR(CONSTR_INDEX, PARAM_SEQ) \
	BOOST_MIRROR_REG_CLASS_OR_TEMPL_CONSTRUCTOR(CONSTR_INDEX, PARAM_SEQ, typename)
 
#define BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(TYPE)\
BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN( TYPE ) \
        BOOST_MIRROR_REG_DEFAULT_CONSTRUCTOR(0) \
        BOOST_MIRROR_REG_CONSTRUCTOR(1, ((TYPE)(init))) \
BOOST_MIRROR_REG_CONSTRUCTORS_END 

BOOST_MIRROR_REGISTER_NATIVE_TYPE_CONSTRUCTORS(void)
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

} // namespace mirror
} // namespace boost

#endif //include guard

