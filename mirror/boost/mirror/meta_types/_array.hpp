/**
 * \file boost/mirror/meta_types/_array.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_ARRAY_HPP
#define BOOST_MIRROR_META_TYPES_ARRAY_HPP

// registering of meta-types
#include <boost/mirror/detail/meta_type_registering.hpp>
// type name decorations
#include <boost/mirror/detail/decorated_type_name.hpp>

namespace boost {
namespace mirror {


/** Meta-types for arrays
 */
template <class ElementType, size_t Size>
struct meta_type<ElementType[Size]> : detail::decorated_type_name<
	meta_type<ElementType>,
	detail::type_name_decorator<ElementType[Size]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef typename meta_type<ElementType>::reflected_type 
		reflected_type[Size];
};


template <class ElementType, size_t Size>
struct meta_type<const ElementType[Size]> : detail::decorated_type_name<
	meta_type<const ElementType>,
	detail::type_name_decorator<ElementType[Size]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef const typename meta_type<ElementType>::reflected_type 
		reflected_type[Size];
};

template <class ElementType, size_t Size>
struct meta_type<volatile ElementType[Size]> : detail::decorated_type_name<
	meta_type<volatile ElementType>,
	detail::type_name_decorator<ElementType[Size]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef volatile typename meta_type<ElementType>::reflected_type 
		reflected_type[Size];
};

template <class ElementType, size_t Size>
struct meta_type<const volatile ElementType[Size]>
: detail::decorated_type_name<
	meta_type<const volatile ElementType>,
	detail::type_name_decorator<ElementType[Size]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef const volatile typename meta_type<ElementType>::reflected_type 
		reflected_type[Size];
};

template <class ElementType>
struct meta_type<ElementType[]> : detail::decorated_type_name<
	meta_type<ElementType>,
	detail::type_name_decorator<ElementType[]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef typename meta_type<ElementType>::reflected_type 
		reflected_type[];
};

template <class ElementType>
struct meta_type<const ElementType []> : detail::decorated_type_name<
	meta_type<const ElementType>,
	detail::type_name_decorator<ElementType []>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef typename meta_type<ElementType>::reflected_type const 
		reflected_type[];
};

template <class ElementType>
struct meta_type<volatile ElementType []> : detail::decorated_type_name<
	meta_type<volatile ElementType>,
	detail::type_name_decorator<ElementType []>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef typename meta_type<ElementType>::reflected_type volatile
		reflected_type[];
};

template <class ElementType>
struct meta_type<const volatile ElementType []> 
: detail::decorated_type_name<
	meta_type<const volatile ElementType>,
	detail::type_name_decorator<ElementType []>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef typename meta_type<ElementType>::reflected_type const volatile
		reflected_type[];
};


} // namespace mirror
} // namespace boost

#endif //include guard

