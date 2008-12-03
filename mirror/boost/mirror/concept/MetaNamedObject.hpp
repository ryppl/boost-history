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

namespace boost {
namespace mirror {

template <typename T>
struct is_MetaNamedObject 
{
	BOOST_MIRROR_CONCEPT_TESTER_BEGIN

	template <class X> 
	static match test(
		X*, 
		// check for base_name member function
		const ::boost::cts::bstring* _y1 = &X::base_name(),
		// check for get_name member function
		const ::std::string* _y2 = &X::get_name(mpl::false_(), ::std::char_traits<char>()),
		const ::std::wstring* _y3 = &X::get_name(mpl::false_(), ::std::char_traits<wchar_t>())
	);

	typedef is_MetaObject<T> other_constraints;

	BOOST_MIRROR_CONCEPT_TESTER_END

};



} // namespace mirror
} // namespace boost

#endif //include guard

