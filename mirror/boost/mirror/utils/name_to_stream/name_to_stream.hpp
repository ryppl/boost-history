/**
 * \file boost/mirror/utils/name_to_stream.hpp
 * Putting names of reflected nammespaces, classes, etc. to streams
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_UTILS_NAME_TO_STREAM
#define BOOST_MIRROR_UTILS_NAME_TO_STREAM

// meta namespaces (includes boost/char_type_switch/string.hpp)
#include <boost/mirror/meta_namespace.hpp>
// meta types 
#include <boost/mirror/meta_type.hpp>

namespace boost {
namespace mirror {

/** name_to_stream_helper function object
 */
template <class meta_object> 
struct name_to_stream_helper;

/** Specialization for the global scope meta-namespace
 */
template <class namespace_name> 
struct name_to_stream_helper<meta_namespace<void, namespace_name> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
	{
		return ldng_dbl_cln? s << BOOST_STR_LIT("::") : s;
	}
};

/** Specialization for the top level namespaces
 */
template <class namespace_name> 
struct name_to_stream_helper<meta_namespace<namespaces::_, namespace_name> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
	{
		return (ldng_dbl_cln? s << BOOST_STR_LIT("::") : s) << 
			   meta_namespace<namespaces::_, namespace_name>::base_name();
	}
};

/** Specialization of meta_namespace for nested namespaces
 */
template <class parent_meta_ns, class namespace_name> 
struct name_to_stream_helper<meta_namespace<parent_meta_ns, namespace_name> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
	{
		return name_to_stream<parent_meta_ns>::put(s, ldng_dbl_cln) <<
		       BOOST_STR_LIT("::") << 
			   meta_namespace<parent_meta_ns, namespace_name>::base_name();
	}
};

/** Specialization for meta-types
 */
template <typename base_type> 
struct name_to_stream_helper<meta_type<base_type> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
	{
		if(!is_global_scope<typename meta_type<base_type>::scope>::value)
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
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
	{
		return name_to_stream_helper<meta_type<pointee_type> >::put(s,ldng_dbl_cln) << BOOST_STR_LIT("*");
	}
};

/** Specialization for meta-types for references
 */
template <typename refered_to_type> 
struct name_to_stream_helper<meta_type<refered_to_type&> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
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
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
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
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
	{
		s << BOOST_STR_LIT("volatile ");
		return name_to_stream_helper<meta_type<non_volatile_type> >::put(s,ldng_dbl_cln);
	}
};

/** Specialization for meta-types for volatile types
 */
template <typename non_cv_type> 
struct name_to_stream_helper<meta_type<const volatile non_cv_type> >
{
	template <class out_stream>
	static out_stream& put(out_stream& s, bool ldng_dbl_cln = false)
	{
		s << BOOST_STR_LIT("const volatile ");
		return name_to_stream_helper<meta_type<non_cv_type> >::put(s,ldng_dbl_cln);
	}
};


/** Specialization for meta-classes
 */
template <typename base_type> 
struct name_to_stream_helper<meta_class<base_type> > :
public name_to_stream_helper<meta_type<base_type> > { };

/** name_to_stream function object
 *  Puts the whole name of the meta object into the given stream.
 *  There are three ways how to use this function object
 *  1) invocation of static member function put(...)
 *     name_to_stream<meta_object>::put(stream); 
 *  2) construction with a stream as argument
 *     name_to_stream<meta_object> nts(stream);
 *  3) construction and calling of the function call operator
 *     name_to_stream<meta_object>()(stream); or
 *     name_to_stream<meta_object> nts; nts();
 */
template <typename meta_object> 
struct name_to_stream : public name_to_stream_helper<meta_object>
{
	// default constructor 
	name_to_stream(void){ }
	// this constructor invokes the put operation on the given stream
	template <class out_stream>
	name_to_stream(out_stream& s, bool ldng_dbl_cln = false){put(s, ldng_dbl_cln);}
	// function call operator
	template <class out_stream>
	out_stream& operator()(out_stream& s, bool ldng_dbl_cln = false){return put(s, ldng_dbl_cln);}
};


} // namespace mirror
} // namespace boost

#endif //include guard

