/**
 * \file boost/mirror/meta_types/_ptr_ref.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_PTR_REF_HPP
#define BOOST_MIRROR_META_TYPES_PTR_REF_HPP

// registering of meta-types
#include <boost/mirror/detail/meta_type_registering.hpp>
// type name decorations
#include <boost/mirror/detail/decorated_type_name.hpp>

namespace boost {
namespace mirror {

/** Meta-types for pointers
 */
template <class PointeeType>
struct meta_type<PointeeType*> : detail::decorated_type_name<
	meta_type<PointeeType>,
	detail::type_name_decorator<PointeeType*>
>
{
	typedef typename meta_type<PointeeType>::scope scope;
	typedef PointeeType* reflected_type; 
};

/** Meta-types for references
 */
template <class ReferredToType>
struct meta_type<ReferredToType&> : detail::decorated_type_name<
	meta_type<ReferredToType>,
	detail::type_name_decorator<ReferredToType&>
>
{
	typedef typename meta_type<ReferredToType>::scope scope;
	typedef ReferredToType& reflected_type; 
};

} // namespace mirror
} // namespace boost

#endif //include guard

