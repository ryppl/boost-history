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


template <class MetaClass, class NodePath = mpl::vector0<> >
class deep_traversal_of 
: detail::traversal_utils<
	MetaClass,
	NodePath,
	typename MetaClass::attributes,
	deep_traversal_of
>
{
private:
	typedef typename remove_reference<
		typename MetaClass::reflected_type
	>::type* InstancePtr;
public:
	template <class VisitorType>
	static inline void accept(
		VisitorType visitor,
		InstancePtr ptr_to_inst = 0
	)
	{
		do_accept(ref<VisitorType>(visitor), ptr_to_inst);
	}

	template <class VisitorType>
	static inline void accept(
		reference_wrapper<VisitorType> visitor,
		InstancePtr ptr_to_inst = 0
	)
	{
		do_accept(visitor, ptr_to_inst);
	}
private:
	template <class VisitorType>
	static inline void do_accept(
		reference_wrapper<VisitorType> visitor,
		InstancePtr ptr_to_inst
	)
	{
		MetaClass mc;
		NodePath path;
		// enter the type
		visitor.get().enter_type(
			mc, 
			meta_path::make_node_context(
				path,
				mc
			)
		);
		// visit the instance
		lead_to_instance(visitor, mc, path, ptr_to_inst);
		// go through the base classes
		for_each<typename MetaClass::base_classes>(
			cref(show_bases_to(visitor, ptr_to_inst))
		);
		// go through the own class' attributes
		for_each<typename MetaClass::attributes>(
			cref(show_attribs_to(visitor, ptr_to_inst))
		);
		// leave the type
		visitor.get().leave_type(
			mc, 
			meta_path::make_node_context(
				path,
				mc
			)
		);
	}
};

template <class MetaClass, class NodePath = mpl::vector0<> >
class flat_traversal_of
: detail::traversal_utils<
	MetaClass, 
	NodePath,
	typename MetaClass::all_attributes,
	flat_traversal_of
>
{
private:
	typedef typename remove_reference<
		typename MetaClass::reflected_type
	>::type* InstancePtr;
public:
	template <class VisitorType>
	static inline void accept(
		VisitorType visitor,
		InstancePtr ptr_to_inst = 0
	)
	{
		do_accept(ref<VisitorType>(visitor), ptr_to_inst);
	}
	template <class VisitorType>
	static inline void accept(
		reference_wrapper<VisitorType> visitor,
		InstancePtr ptr_to_inst = 0
	)
	{
		do_accept(visitor, ptr_to_inst);
	}
private:
	template <class VisitorType>
	static inline void do_accept(
		reference_wrapper<VisitorType> visitor, 
		InstancePtr ptr_to_inst
	)
	{
		MetaClass mc;
		NodePath path;
		// enter the type
		visitor.get().enter_type(
			mc, 
			meta_path::make_node_context(
				path,
				mc
			)
		);
		// visit the instance
		lead_to_instance(visitor, mc, path, ptr_to_inst);
		// go through all of the class' attributes
		for_each<typename MetaClass::all_attributes>(
			cref(show_attribs_to(visitor, ptr_to_inst))
		);
		// leave the type
		visitor.get().leave_type(
			mc, 
			meta_path::make_node_context(
				path,
				mc
			)
		);
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard

