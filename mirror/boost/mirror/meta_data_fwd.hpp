/**
 * \file boost/mirror/meta_data_fwd.hpp
 * Forward declarations of meta-data templates
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DATA_FWD_HPP
#define BOOST_MIRROR_META_DATA_FWD_HPP

namespace boost {
namespace mirror {

/** Meta-namespace forward template declaration
 */
template<class namespace_alias> 
struct meta_namespace;

/** Macro that expands into the meta_namespace for the 
 *  namespace with the given alias.
 */
#define BOOST_MIRROR_REFLECT_NAMESPACE(NAMESPACE_ALIAS) \
	::boost::mirror::meta_namespace<namespaces::NAMESPACE_ALIAS>



/** Meta-type forward template declaration
 */
template <class base_type> 
struct meta_type;


/** Macro that expands into the meta_type for the 
 *  given type or class.
 */
#define BOOST_MIRROR_REFLECT_TYPE(TYPE) \
	::boost::mirror::meta_type<TYPE>

/** Macro that expands into the meta_type for the 
 *  given typedefined type.
 */
#define BOOST_MIRROR_REFLECT_TYPEDEFD(NAMESPACE_ALIAS, TYPEDEFD) \
	::boost::mirror::meta_type< BOOST_MIRROR_TYPEDEFD_SELECTOR(\
		NAMESPACE_ALIAS##_##TYPEDEFD, \
		TYPEDEFD\
	) >

/** Macro that expands into the meta_type for the 
 *  type of the given expression.
 *  To get this going <boost/typeof/typeof.hpp>
 *  has to be included.
 */
#define BOOST_MIRROR_REFLECT_TYPEOF(EXPRESSION) \
	::boost::mirror::meta_type<BOOST_TYPEOF(EXPRESSION)>



namespace detail {

	/** Because one class can have several different meta_classes 
	 *  we require some means to distinguish between them when
	 *  selecting the proper meta_class<> template instantiation.
	 *  This type is used to mark the default meta-class variant.
	 */ 
	struct default_meta_class_variant{ };
} //namespace detail


/** Meta-class template forward declaration
 */
template <
	class a_class, 
	class variant_tag = detail::default_meta_class_variant
>
struct meta_class;

/** Macro that expands into the meta_class for the 
 *  given type or class.
 */
#define BOOST_MIRROR_REFLECT_CLASS(CLASS) \
	::boost::mirror::meta_class<CLASS>

/** Macro that expands into the meta_class for the 
 *  given type or class.
 */
#define BOOST_MIRROR_REFLECT_CLASS_VT(CLASS, VARIANT_TAG) \
	::boost::mirror::meta_class<\
		CLASS, \
		VARIANT_TAG\
>



} // namespace mirror
} // namespace boost

#endif //include guard

