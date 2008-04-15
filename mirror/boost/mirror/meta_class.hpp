/**
 * \file boost/mirror/meta_class.hpp
 * Registering and reflection of classes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_CLASS_HPP
#define BOOST_MIRROR_META_CLASS_HPP

// basic meta types
#include <boost/mirror/meta_type.hpp>
// reflection of class inheritance
#include <boost/mirror/meta_inheritance.hpp>
// reflection of class member attributes
#include <boost/mirror/meta_attributes.hpp>

namespace boost {
namespace mirror {

namespace detail {

	/** Because one class can have several different meta_classes 
	 *  we require some means to distinguish between them when
	 *  selecting the proper meta_class<> template instantiation.
	 *  This type is used to mark the default meta-class variant.
	 */ 
	struct default_meta_class_variant;
} //namespace detail

/** Meta class - specializes the meta_type for classes
 */
template <class a_class, class variant_tag = detail::default_meta_class_variant>
struct meta_class : public meta_type<a_class>
{
	typedef typename meta_base_classes<a_class, variant_tag>::list base_classes;
	typedef typename meta_class_attributes<a_class, variant_tag> attributes;
};

/** This macro should be included in the definition of every class
 *  with private or protected members, that should be refleccted
 */
#define BOOST_MIRROR_FRIENDLY_CLASS(CLASS_NAME) \
		friend struct ::boost::mirror::meta_class_attributes<CLASS_NAME>;

} // namespace mirror
} // namespace boost

#endif //include guard

