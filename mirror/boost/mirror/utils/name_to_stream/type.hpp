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
template <typename Type> 
struct name_to_stream_helper<meta_type<Type> >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		if(!reflects_global_scope<typename meta_type<Type>::scope>::value)
			name_to_stream<
				typename meta_type<Type>::scope
			>::put(s, ldng_dbl_cln) << BOOST_STR_LIT("::");
		else if(ldng_dbl_cln) s << BOOST_STR_LIT("::");
		return s << meta_type<Type>::base_name();
	}
};

/** Specialization for meta-types for pointers
 */
template <typename PointeeType> 
struct name_to_stream_helper<meta_type<PointeeType*> >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		return name_to_stream_helper<meta_type<PointeeType> >::put(s,ldng_dbl_cln) << BOOST_STR_LIT("*");
	}
};

/** Specialization for meta-types for non-cv element arrays
 */
template <typename ElementType, size_t Size> 
struct name_to_stream_helper<meta_type<ElementType[Size]> >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		return name_to_stream_helper<meta_type<ElementType> >::put(s,ldng_dbl_cln) << 
			BOOST_STR_LIT("[") <<
			Size << 
			BOOST_STR_LIT("]");
	}
};

/** Specialization for meta-types for const element arrays
 */
template <typename ElementType, size_t Size> 
struct name_to_stream_helper<meta_type<const ElementType[Size]> >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		
		s << BOOST_STR_LIT("const ");
		return name_to_stream_helper<
			meta_type<ElementType[Size]> 
		>::put(s, ldng_dbl_cln);
	}
};

/** Specialization for meta-types for references
 */
template <typename ReferredToType> 
struct name_to_stream_helper<meta_type<ReferredToType&> >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		return name_to_stream_helper<meta_type<ReferredToType> >::put(s,ldng_dbl_cln) << BOOST_STR_LIT("&");
	}
};

/** Specialization for meta-types for const types
 */
template <typename NonConstType> 
struct name_to_stream_helper<meta_type<const NonConstType> >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		s << BOOST_STR_LIT("const ");
		return name_to_stream_helper<meta_type<NonConstType> >::put(s,ldng_dbl_cln);
	}
};

/** Specialization for meta-types for volatile types
 */
template <typename NonVolatileType> 
struct name_to_stream_helper<meta_type<volatile NonVolatileType> >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		s << BOOST_STR_LIT("volatile ");
		return name_to_stream_helper<meta_type<NonVolatileType> >::put(s,ldng_dbl_cln);
	}
};

/** Specialization for meta-types for const volatile types
 */
template <typename NonCVType> 
struct name_to_stream_helper<meta_type<const volatile NonCVType> >
{
	template <class OutStream>
	static OutStream& put(OutStream& s, bool ldng_dbl_cln)
	{
		s << BOOST_STR_LIT("const volatile ");
		return name_to_stream_helper<meta_type<NonCVType> >::put(s,ldng_dbl_cln);
	}
};


} // namespace mirror
} // namespace boost

#endif //include guard

