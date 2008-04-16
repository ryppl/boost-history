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

namespace boost {
namespace mirror {

/** Specialization of name_to_stream_helper for the global scope meta-namespace
 */
template <class namespace_name> 
struct name_to_stream_helper<meta_namespace<void, namespace_name> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		return ldng_dbl_cln? s << BOOST_STR_LIT("::") : s;
	}
};

/** Specialization of name_to_stream_helper for the top level namespaces
 */
template <class namespace_name> 
struct name_to_stream_helper<meta_namespace<namespaces::_, namespace_name> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		return (ldng_dbl_cln? s << BOOST_STR_LIT("::") : s) << 
			   meta_namespace<namespaces::_, namespace_name>::base_name();
	}
};

/** Specialization of name_to_stream_helper for nested namespaces
 */
template <class parent_meta_ns, class namespace_name> 
struct name_to_stream_helper<meta_namespace<parent_meta_ns, namespace_name> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		return name_to_stream<parent_meta_ns>::put(s, ldng_dbl_cln) <<
		       BOOST_STR_LIT("::") << 
			   meta_namespace<parent_meta_ns, namespace_name>::base_name();
	}
};


} // namespace mirror
} // namespace boost

#endif //include guard

