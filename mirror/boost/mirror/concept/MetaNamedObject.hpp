/**
 * \file boost/mirror/concept/MetaNamedObject.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaNamedObject concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_NAMED_OBJECT_HPP
#define BOOST_MIRROR_CONCEPT_META_NAMED_OBJECT_HPP

#include <boost/mirror/concept/MetaObject.hpp>
#include <boost/char_type_switch/string.hpp>
#include <boost/preprocessor/cat.hpp>

namespace boost {
namespace mirror {

#define BOOST_MIRROR_MNO_CONCEPT_TEST_HELPER(Z, X, I, CHAR_T) \
	, const ::std::basic_string< CHAR_T >* BOOST_PP_CAT(_z, I) = \
	&X::get_name( mpl::false_(), ::std::char_traits< CHAR_T >())

template <typename T>
struct is_MetaNamedObject 
{
	BOOST_MIRROR_CONCEPT_TESTER_BEGIN

	template <class X> 
	static match test(
		X*, 
		// check for base_name member function
		const ::boost::cts::bstring* _y1 = &X::base_name()
		// check for get_name member function
		BOOST_CTS_FOR_EACH_CHAR_T(BOOST_MIRROR_MNO_CONCEPT_TEST_HELPER,X)
	);

	typedef is_MetaObject<T> other_constraints;

	BOOST_MIRROR_CONCEPT_TESTER_END

};

#undef BOOST_MIRROR_MNO_CONCEPT_TEST_HELPER

} // namespace mirror
} // namespace boost

#endif //include guard

