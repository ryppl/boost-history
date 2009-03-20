/**
 * \file boost/mirror/detail/visitor_traits.hpp
 * Auto-detection of visitor's member function availability
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_DETAIL_VISITOR_TRAITS_HPP
#define BOOST_MIRROR_DETAIL_VISITOR_TRAITS_HPP

#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <typename MemFnPtr, MemFnPtr mem_fn_ptr>
struct member_fn_ptr_wrapper { };

#define BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY( \
	MEM_FN_NAME, \
	PARAM_COUNT \
) \
template <class Visitor, BOOST_PP_ENUM_PARAMS(PARAM_COUNT, typename P)> \
char visitor_has_##MEM_FN_NAME( \
	member_fn_ptr_wrapper< \
		void (Visitor::*)(BOOST_PP_ENUM_PARAMS(PARAM_COUNT, P)), \
		&Visitor::template MEM_FN_NAME<BOOST_PP_ENUM_PARAMS(PARAM_COUNT, P)> \
	>* \
); \
 \
template <class Visitor, BOOST_PP_ENUM_PARAMS(PARAM_COUNT, typename P)> \
double visitor_has_##MEM_FN_NAME(...); \
 \
template <class Visitor, BOOST_PP_ENUM_PARAMS(PARAM_COUNT, typename P)> \
struct visitor_can_##MEM_FN_NAME\
{ \
	enum {value = \
		sizeof(char) == \
		sizeof(visitor_has_##MEM_FN_NAME< \
			Visitor, \
			BOOST_PP_ENUM_PARAMS(PARAM_COUNT, P) \
		>(0)) \
	}; \
	typedef mpl::bool_<value> type;	\
};

BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(enter_namespace, 2)
BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(leave_namespace, 2)

BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(enter_namespace_members, 2)
BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(leave_namespace_members, 2)

BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(enter_type, 2)
BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(leave_type, 2)

BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(enter_attribute, 2)
BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(leave_attribute, 2)

BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(enter_attributes, 3)
BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(leave_attributes, 3)

BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(enter_base_class, 2)
BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(leave_base_class, 2)

BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(enter_base_classes, 3)
BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY(leave_base_classes, 3)


#undef BOOST_MIRROR_DETAIL_IMPLEMENT_VISITOR_MEM_FN_QUERY

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

