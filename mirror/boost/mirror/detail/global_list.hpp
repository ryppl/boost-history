/**
 * \file boost/mirror/detail/global_list.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_GLOBAL_LIST_HPP
#define BOOST_MIRROR_META_DETAIL_GLOBAL_LIST_HPP

#include <boost/mirror/detail/counter.hpp>

#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/not.hpp>

namespace boost {
namespace mirror {
namespace counter {

template <class Selector, class Counter>
struct global_list;

template <class Selector>
struct global_list<Selector, initial>
{
	typedef ::boost::mpl::vector0<> type;
};

template <class Selector, class Counter, class UnaryPredicate>
struct filtered_global_list
{
	typedef typename ::boost::mpl::remove_if<
		typename global_list<
			Selector, 
			Counter
		>::type,
		mpl::not_<
			mpl::apply1<
				UnaryPredicate,
				mpl::_1
			>
		>
	>::type type;
};

} // namespace counter



#define BOOST_MIRROR_REGISTER_GLOBAL_LIST_SELECTOR(FULLY_QUALIFIED_TYPE) \
	BOOST_MIRROR_REGISTER_COUNTER_SELECTOR(FULLY_QUALIFIED_TYPE) 

#define BOOST_MIRROR_GET_GLOBAL_LIST_BASE_WRAPPER(SELECTOR, LUID, TYPENAME_KW) \
                TYPENAME_KW ::boost::mirror::counter::global_list< \
                        ::boost::mirror::counter::selector< SELECTOR>, \
                        BOOST_MIRROR_COUNTER_PREVIOUS_COUNT_BASE( \
                                SELECTOR, \
                                LUID, \
                                TYPENAME_KW \
                        ) \
                >

#define BOOST_MIRROR_GET_FILTERED_GLOBAL_LIST_BASE_WRAPPER( \
	SELECTOR, \
	LUID, \
	TYPENAME_KW, \
	FILTER \
) 		TYPENAME_KW ::boost::mirror::counter::filtered_global_list< \
			::boost::mirror::counter::selector< SELECTOR>, \
			BOOST_MIRROR_COUNTER_PREVIOUS_COUNT_BASE( \
				SELECTOR, \
				LUID, \
				TYPENAME_KW \
			), \
			FILTER \
		>

#define BOOST_MIRROR_GET_GLOBAL_LIST_BASE(SELECTOR, LUID, TYPENAME_KW) \
	BOOST_MIRROR_GET_GLOBAL_LIST_BASE_WRAPPER( \
		SELECTOR, \
		LUID, \
		TYPENAME_KW \
	)::type

#define BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(SELECTOR, ITEM, LUID) \
namespace counter { \
template <> \
struct global_list< \
	::boost::mirror::counter::selector< SELECTOR>, \
	BOOST_MIRROR_COUNTER_CURRENT_COUNT(SELECTOR, LUID) \
> \
{ \
	typedef ::boost::mpl::push_back< \
		BOOST_MIRROR_GET_GLOBAL_LIST_BASE( \
			SELECTOR, \
			LUID, \
			BOOST_PP_EMPTY() \
		), \
		ITEM \
	>::type type; \
}; \
BOOST_MIRROR_COUNTER_INCREMENT_COUNTER(SELECTOR, LUID) \
}


#define BOOST_MIRROR_GET_GLOBAL_LIST(SELECTOR) \
	BOOST_MIRROR_GET_GLOBAL_LIST_BASE( \
		SELECTOR, \
		BOOST_MIRROR_COUNTER_LUID, \
		BOOST_PP_EMPTY() \
	) 

#ifndef BOOST_MIRROR_COUNTER_UNRELIABLE

#define BOOST_MIRROR_ADD_TO_GLOBAL_LIST(SELECTOR, ITEM) \
	BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE( \
		SELECTOR, \
		ITEM, \
		BOOST_MIRROR_COUNTER_LUID \
	) 

#else // BOOST_MIRROR_COUNTER_UNRELIABLE

// if we don't have a reliable counter adding to 
// global lists is disabled
#define BOOST_MIRROR_ADD_TO_GLOBAL_LIST(SELECTOR, ITEM) 

#define BOOST_MIRROR_NO_GLOBAL_LISTS

#endif // BOOST_MIRROR_COUNTER_UNRELIABLE

} // namespace mirror
} // namespace boost

#endif //include guard

