/**
 * \file boost/mirror/meta_types/_free_fn.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_FREE_FN_HPP
#define BOOST_MIRROR_META_TYPES_FREE_FN_HPP

// preprocessor helpers
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp> 

// registering of meta-types
#include <boost/mirror/detail/meta_type_registering.hpp>
// type name decorations
#include <boost/mirror/detail/function_type_name.hpp>

namespace boost {
namespace mirror {

namespace calling_convention {

#define BOOST_MIRROR_IMPLEMENT_CALLING_CONVENTION_NAME(R, CC, I, CHAR_T) \
	inline static const ::std::basic_string< CHAR_T >& \
	name(::std::char_traits< CHAR_T >) \
	{ \
		static ::std::basic_string<CHAR_T> cc_name( \
			BOOST_CTS_STRINGIZE_CHAR_T(CHAR_T, __##CC) \
		);\
		return cc_name; \
	} 

#define BOOST_MIRROR_IMPLEMENT_DEFAULT_CALLING_CONVENTION_NAME(R,CC,I, CHAR_T)\
	inline static ::std::basic_string< CHAR_T > \
	name(::std::char_traits< CHAR_T >) \
	{ \
		return ::std::basic_string< CHAR_T >(); \
	} 

#define BOOST_MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER(CC) \
	struct __##CC##_ \
	{ \
		BOOST_CTS_FOR_EACH_CHAR_T( \
			BOOST_MIRROR_IMPLEMENT_CALLING_CONVENTION_NAME, \
			CC \
		) \
	}; 

	BOOST_MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER(cdecl)
	BOOST_MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER(stdcall)
	BOOST_MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER(pascal)

	struct __default_
	{ 
		BOOST_CTS_FOR_EACH_CHAR_T(
			BOOST_MIRROR_IMPLEMENT_DEFAULT_CALLING_CONVENTION_NAME,
			_
		)
	}; 

// undefine the helper macros
#undef BOOST_MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER
#undef BOOST_MIRROR_IMPLEMENT_DEFAULT_CALLING_CONVENTION_NAME
#undef BOOST_MIRROR_IMPLEMENT_CALLING_CONVENTION_NAME
}

/** Meta-type for free function types
 */
#define BOOST_MIRROR_IMPLEMENT_FREE_FUNCTION_META_TYPE( \
	X, \
	ARG_COUNT, \
	CALLING_CONVENTION \
) \
template <typename RetVal BOOST_PP_ENUM_TRAILING_PARAMS(ARG_COUNT, class T) > \
struct meta_type<RetVal (BOOST_PP_ENUM_PARAMS(ARG_COUNT, T))>\
: detail::function_type_name< \
	RetVal, \
	calling_convention::__ ## CALLING_CONVENTION ## _, \
	mpl::vector<BOOST_PP_ENUM_PARAMS(ARG_COUNT, T)> \
> \
{ \
	typedef typename meta_type<void>::scope scope; \
	typedef typename meta_type<RetVal>::reflected_type \
		(reflected_type)(BOOST_PP_ENUM_PARAMS(ARG_COUNT, T)); \
};

BOOST_PP_REPEAT(16, BOOST_MIRROR_IMPLEMENT_FREE_FUNCTION_META_TYPE, default)


} // namespace mirror
} // namespace boost

#endif //include guard

