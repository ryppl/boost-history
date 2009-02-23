/**
 * \file boost/mirror/detail/counter.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_COUNTER_HPP
#define BOOST_MIRROR_META_DETAIL_COUNTER_HPP

#include <boost/preprocessor/facilities/empty.hpp>

#include <boost/typeof/typeof.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/type_traits/is_same.hpp>

#ifndef BOOST_MIRROR_COUNTER_LUID
#ifdef __COUNTER__
#define BOOST_MIRROR_COUNTER_LUID __COUNTER__
#else
#define BOOST_MIRROR_COUNTER_LUID __LINE__
#define BOOST_MIRROR_COUNTER_UNRELIABLE
#endif
#endif

namespace boost {
namespace mirror {
namespace counter {

// local null-type
typedef ::boost::mpl::int_<-1> null_;
// the initial value of the counter
typedef ::boost::mpl::int_<0> initial;

// a fallback version of the get_next function
template <class Selector>
null_ get_next(Selector*, ...);

// the get_next overload for the first incrementation
template <class Selector>
::boost::mpl::int_<1> get_next(Selector*, initial);

template <class Selector, class Value, int InstId>
struct current
{
	enum {instantiation_id = InstId};

	typedef BOOST_TYPEOF(get_next((Selector*)0, Value())) next;

	typedef typename ::boost::mpl::eval_if<
		::boost::is_same<next, null_>,
		::boost::mpl::identity<Value>,
		current<Selector, next, InstId>
	>::type type;
};

template <class T> struct selector;

} // namespace counter
} // namespace mirror
} // namespace boost

/** This macro registers a selector for the given fully qualified
 *  type. This macro should be used inside of the ::boost::mirror
 *  namespace.
 */
#define BOOST_MIRROR_REGISTER_COUNTER_SELECTOR(FULLY_QUALIFIED_TYPE) \
namespace counter { \
template <> struct selector < FULLY_QUALIFIED_TYPE > { }; \
} /** namespace counter */


#define BOOST_MIRROR_COUNTER_CURRENT_COUNT(SELECTOR, LUID) \
	::boost::mirror::counter::current< \
		::boost::mirror::counter::selector< SELECTOR >, \
		::boost::mirror::counter::initial, LUID \
	>::type

#define BOOST_MIRROR_COUNTER_PREVIOUS_COUNT_BASE(SELECTOR, LUID, TYPENAME_KW) \
	TYPENAME_KW ::boost::mpl::prior< \
		TYPENAME_KW ::boost::mirror::counter::current< \
			::boost::mirror::counter::selector< SELECTOR >, \
			::boost::mirror::counter::initial, LUID \
		>::type \
	>:: type

#define BOOST_MIRROR_COUNTER_PREVIOUS_COUNT(SELECTOR, LUID) \
	BOOST_MIRROR_COUNTER_PREVIOUS_COUNT_BASE(SELECTOR, LUID, BOOST_PP_EMPTY())


#define BOOST_MIRROR_COUNTER_INCREMENT_COUNTER(SELECTOR, LUID) \
	::boost::mpl::next< \
		current< \
			selector< SELECTOR>, \
			initial, \
			LUID \
		>::type \
	>::type get_next( \
		selector< SELECTOR>*, \
		current< selector< SELECTOR>, initial, LUID >::type \
	); 

#define BOOST_MIRROR_COUNTER_CURRENT(SELECTOR) \
	BOOST_MIRROR_COUNTER_CURRENT_COUNT( \
		SELECTOR, \
		BOOST_MIRROR_COUNTER_LUID \
	)
#define BOOST_MIRROR_COUNTER_PREVIOUS(SELECTOR) \
	BOOST_MIRROR_COUNTER_PREVIOUS_COUNT(SELECTOR, BOOST_MIRROR_COUNTER_LUID)
#define BOOST_MIRROR_COUNTER_INCREMENT(SELECTOR) \
	BOOST_MIRROR_COUNTER_INCREMENT_COUNTER(SELECTOR, BOOST_MIRROR_COUNTER_LUID)

#endif //include guard

