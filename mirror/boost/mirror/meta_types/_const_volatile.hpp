/**
 * \file boost/mirror/meta_types/_const_volatile.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_CONST_VOLATILE_HPP
#define BOOST_MIRROR_META_TYPES_CONST_VOLATILE_HPP

// registering of meta-types
#include <boost/mirror/detail/meta_type_registering.hpp>
// type name decorations
#include <boost/mirror/detail/decorated_type_name.hpp>

namespace boost {
namespace mirror {

/** Meta-types for const types
 */
template <class NonConstType>
struct meta_type<const NonConstType> : detail::decorated_type_name<
	meta_type<NonConstType>,
	detail::type_name_decorator<const NonConstType>
>
{
	typedef typename meta_type<NonConstType>::scope scope;
	typedef const NonConstType reflected_type; 
};

/** Meta-types for volatile types
 */
template <class NonVolatileType>
struct meta_type<volatile NonVolatileType> : detail::decorated_type_name<
	meta_type<NonVolatileType>,
	detail::type_name_decorator<volatile NonVolatileType>
>
{
	typedef typename meta_type<NonVolatileType>::scope scope;
	typedef volatile NonVolatileType reflected_type; 
};

/** Meta-types for const volatile types
 */
template <class NonCVType>
struct meta_type<const volatile NonCVType> : detail::decorated_type_name<
	meta_type<NonCVType>,
	detail::type_name_decorator<const volatile NonCVType>
>
{
	typedef typename meta_type<NonCVType>::scope scope;
	typedef const volatile NonCVType reflected_type; 
};

} // namespace mirror
} // namespace boost

#endif //include guard

