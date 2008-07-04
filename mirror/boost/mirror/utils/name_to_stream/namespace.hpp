/**
 * \file boost/mirror/utils/name_to_stream/namespace.hpp
 * Putting names of registered namespaces to streams
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_UTILS_NAME_TO_STREAM_NAMESPACE_HPP
#define BOOST_MIRROR_UTILS_NAME_TO_STREAM_NAMESPACE_HPP

// common definitions
#include <boost/mirror/utils/name_to_stream/common.hpp>
// meta namespaces (includes boost/char_type_switch/string.hpp)
#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/traits/reflects_global_scope.hpp>

namespace boost {
namespace mirror {

/** Specialization of name_to_stream_helper for the global scope meta-namespace
 */
template <> 
struct name_to_stream_helper< BOOST_MIRRORED_GLOBAL_SCOPE() >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		return ldng_dbl_cln? s << BOOST_STR_LIT("::") : s;
	}
};

/** Specialization of name_to_stream_helper for the top level namespaces
 */
template <class NamespacePlaceholder> 
struct name_to_stream_helper<meta_namespace<NamespacePlaceholder> >
{
	typedef meta_namespace<NamespacePlaceholder> meta_ns;
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		typedef typename meta_ns::scope parent_ns;
		// let the printer print out the  base name of the parent namespace
		name_to_stream<parent_ns>::put(s, ldng_dbl_cln);
		// if the parent is not the global scope
		if(!reflects_global_scope<parent_ns>::value)
			s << BOOST_STR_LIT("::");
		// let the printer print out the  base name of the parent namespace
		return s << meta_ns::base_name();
	}
};


} // namespace mirror
} // namespace boost

#endif //include guard

