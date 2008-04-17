/**
 * \file boost/mirror/utils/name_to_stream/type.hpp
 * Putting names of reflected type to streams
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_UTILS_NAME_TO_STREAM_TYPE_HPP
#define BOOST_MIRROR_UTILS_NAME_TO_STREAM_TYPE_HPP

// include the name to stream helpers for namespaces
#include <boost/mirror/utils/name_to_stream/namespace.hpp>
// meta types 
#include <boost/mirror/meta_type.hpp>

namespace boost {
namespace mirror {

/** Specialization for meta-types
 */
template <typename base_type> 
struct name_to_stream_helper<meta_type<base_type> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		if(!reflects_global_scope<typename meta_type<base_type>::scope>::value)
			name_to_stream<meta_type<base_type>::scope>::put(s, ldng_dbl_cln) << BOOST_STR_LIT("::");
		else if(ldng_dbl_cln) s << BOOST_STR_LIT("::");
		return s << meta_type<base_type>::base_name();
	}
};

/** Specialization for meta-types for pointers
 */
template <typename pointee_type> 
struct name_to_stream_helper<meta_type<pointee_type*> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		return name_to_stream_helper<meta_type<pointee_type> >::put(s,ldng_dbl_cln) << BOOST_STR_LIT("*");
	}
};

/** Specialization for meta-types for arrays
 */
template <typename element_type, size_t size> 
struct name_to_stream_helper<meta_type<const element_type[size]> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		s << BOOST_STR_LIT("const ");
		return name_to_stream_helper<meta_type<element_type> >::put(s,ldng_dbl_cln) << 
			BOOST_STR_LIT("[") <<
			size << 
			BOOST_STR_LIT("]");
	}
};

/** Specialization for meta-types for references
 */
template <typename refered_to_type> 
struct name_to_stream_helper<meta_type<refered_to_type&> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		return name_to_stream_helper<meta_type<refered_to_type> >::put(s,ldng_dbl_cln) << BOOST_STR_LIT("&");
	}
};

/** Specialization for meta-types for const types
 */
template <typename non_const_type> 
struct name_to_stream_helper<meta_type<const non_const_type> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		s << BOOST_STR_LIT("const ");
		return name_to_stream_helper<meta_type<non_const_type> >::put(s,ldng_dbl_cln);
	}
};

/** Specialization for meta-types for volatile types
 */
template <typename non_volatile_type> 
struct name_to_stream_helper<meta_type<volatile non_volatile_type> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		s << BOOST_STR_LIT("volatile ");
		return name_to_stream_helper<meta_type<non_volatile_type> >::put(s,ldng_dbl_cln);
	}
};

/** Specialization for meta-types for const volatile types
 */
template <typename non_cv_type> 
struct name_to_stream_helper<meta_type<const volatile non_cv_type> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln)
	{
		s << BOOST_STR_LIT("const volatile ");
		return name_to_stream_helper<meta_type<non_cv_type> >::put(s,ldng_dbl_cln);
	}
};


} // namespace mirror
} // namespace boost

#endif //include guard

