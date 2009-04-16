/**
 * \file boost/mirror/traversal.hpp
 * Traversal of meta classes by a visitor
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_TRAVERSAL_HPP
#define BOOST_MIRROR_TRAVERSAL_HPP

#include <boost/mirror/detail/traversal.hpp>

namespace boost {
namespace mirror {


template <
	class MetaClass, 
	class NodePath
>
class deep_traversal_of_class
: detail::class_traversal_utils<
	MetaClass,
	NodePath,
	typename MetaClass::attributes,
	deep_traversal_of
>
{
private:
	typedef typename remove_reference<
		typename MetaClass::reflected_type
	>::type InstType;
	typedef InstType* InstancePtr;
	typedef const InstType* ConstInstancePtr;
public:
	template <class VisitorType>
	static inline void accept(
		VisitorType visitor,
		InstancePtr ptr_to_inst = 0
	)
	{
		do_accept(::boost::ref<VisitorType>(visitor), ptr_to_inst);
	}

	template <class VisitorType>
	static inline void accept(
		::boost::reference_wrapper<VisitorType> visitor,
		InstancePtr ptr_to_inst = 0
	)
	{
		do_accept(visitor, ptr_to_inst);
	}

	template <class VisitorType>
	static inline void accept(
		VisitorType visitor,
		ConstInstancePtr ptr_to_inst
	)
	{
		do_accept(::boost::ref<VisitorType>(visitor), ptr_to_inst);
	}

	template <class VisitorType>
	static inline void accept(
		::boost::reference_wrapper<VisitorType> visitor,
		ConstInstancePtr ptr_to_inst
	)
	{
		do_accept(visitor, ptr_to_inst);
	}
private:
	template <class VisitorType, class InstanceType>
	static inline void do_accept(
		::boost::reference_wrapper<VisitorType> visitor,
		InstanceType* ptr_to_inst
	)
	{
		MetaClass mc;
		NodePath path;
		// enter the type
		lead_into_type(visitor, mc, path);
		// visit the instance
		lead_to_instance(visitor, mc, path, ptr_to_inst);
		// go through the base classes
		for_each<typename MetaClass::base_classes>(
			::boost::cref(show_bases_to(visitor, ptr_to_inst))
		);
		// go through the own class' attributes
		for_each<typename MetaClass::attributes>(
			::boost::cref(show_attribs_to(visitor, ptr_to_inst))
		);
		// leave the type
		lead_out_of_type(visitor, mc, path);
	}
};

template <
	class MetaClass, 
	class NodePath
>
class flat_traversal_of_class
: detail::class_traversal_utils<
	MetaClass,
	NodePath,
	typename MetaClass::all_attributes,
	flat_traversal_of
>
{
private:
	typedef typename remove_reference<
		typename MetaClass::reflected_type
	>::type InstType;
	typedef InstType* InstancePtr;
	typedef const InstType* ConstInstancePtr;
public:
	template <class VisitorType>
	static inline void accept(
		VisitorType visitor,
		InstancePtr ptr_to_inst = 0
	)
	{
		do_accept(::boost::ref<VisitorType>(visitor), ptr_to_inst);
	}

	template <class VisitorType>
	static inline void accept(
		::boost::reference_wrapper<VisitorType> visitor,
		InstancePtr ptr_to_inst = 0
	)
	{
		do_accept(visitor, ptr_to_inst);
	}

	template <class VisitorType>
	static inline void accept(
		VisitorType visitor,
		ConstInstancePtr ptr_to_inst
	)
	{
		do_accept(::boost::ref<VisitorType>(visitor), ptr_to_inst);
	}

	template <class VisitorType>
	static inline void accept(
		::boost::reference_wrapper<VisitorType> visitor,
		ConstInstancePtr ptr_to_inst
	)
	{
		do_accept(visitor, ptr_to_inst);
	}
private:
	template <class VisitorType, class InstanceType>
	static inline void do_accept(
		::boost::reference_wrapper<VisitorType> visitor, 
		InstanceType* ptr_to_inst
	)
	{
		MetaClass mc;
		NodePath path;
		// enter the type
		lead_into_type(visitor, mc, path);
		// visit the instance
		lead_to_instance(visitor, mc, path, ptr_to_inst);
		// go through all of the class' attributes
		for_each<typename MetaClass::all_attributes>(
			::boost::cref(show_attribs_to(visitor, ptr_to_inst))
		);
		// leave the type
		lead_out_of_type(visitor, mc, path);
	}
};

template < class Class, class VariantTag, class NodePath > 
class deep_traversal_of<meta_class<Class, VariantTag>, NodePath>
: public deep_traversal_of_class<meta_class<Class, VariantTag>, NodePath>
{ };

template < class Type, class NodePath > 
class deep_traversal_of<meta_type<Type>, NodePath>
: public deep_traversal_of_class<meta_class<Type>, NodePath>
{ };

template < class Class, class VariantTag, class NodePath > 
class flat_traversal_of<meta_class<Class, VariantTag>, NodePath>
: public flat_traversal_of_class<meta_class<Class, VariantTag>, NodePath>
{ };

template < class Type, class NodePath > 
class flat_traversal_of<meta_type<Type>, NodePath>
: public flat_traversal_of_class<meta_class<Type>, NodePath>
{ };

/** Traversal of namespace 
 */
template <
	class MetaNamespace, 
	class NodePath, 
	template <class, class> class TraversalType
> class traversal_of_namespace
: detail::namespace_traversal_utils<MetaNamespace, NodePath, TraversalType>
{
public:
	template <class VisitorType>
	static inline void accept(VisitorType visitor)
	{
		do_accept(::boost::ref<VisitorType>(visitor));
	}

	template <class VisitorType>
	static void accept(::boost::reference_wrapper<VisitorType> visitor)
	{
		do_accept(visitor);
	}
private:

	template <class VisitorType>
	static void do_accept(::boost::reference_wrapper<VisitorType> visitor)
	{
		MetaNamespace mn;
		NodePath path;
		// let the visitor enter the namespace
		lead_into_namespace(visitor, mn, path);
		typedef typename MetaNamespace::template members<>::type 
			members;
		// show the visitor through all the members of 
		// the namespace
		for_each<members>(
			::boost::cref(show_namespace_members_to(
				visitor, 
				members()
			))
		);
		// the visitor leaves the namespace
		lead_out_of_namespace(visitor, mn, path);
	}
};

template <class Placeholder, class NodePath>
class deep_traversal_of<meta_namespace<Placeholder>, NodePath>
 : public traversal_of_namespace<
	meta_namespace<Placeholder>, 
	NodePath, 
	deep_traversal_of
>{ };

template <class Placeholder, class NodePath>
class flat_traversal_of<meta_namespace<Placeholder>, NodePath>
 : public traversal_of_namespace<
	meta_namespace<Placeholder>, 
	NodePath,
	flat_traversal_of
>{ };

} // namespace mirror
} // namespace boost

#endif //include guard

