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


template <class MetaClass, class Context = mpl::vector0<> >
class deep_traversal_of 
: detail::traversal_utils<
	MetaClass,
	Context,
	typename MetaClass::attributes,
	deep_traversal_of
>
{
public:
	template <class VisitorType>
	static void accept(
		VisitorType visitor,
		typename MetaClass::reflected_type* ptr_to_inst = 0
	)
	{
		do_accept(ref<VisitorType>(visitor), ptr_to_inst);
	}

	template <class VisitorType>
	static void accept(
		reference_wrapper<VisitorType> visitor,
		typename MetaClass::reflected_type* ptr_to_inst = 0
	)
	{
		do_accept(visitor, ptr_to_inst);
	}
private:
	template <class VisitorType>
	static void do_accept(
		reference_wrapper<VisitorType> visitor,
		typename MetaClass::reflected_type* ptr_to_inst
	)
	{
		MetaClass mc;
		Context ctx;
		// enter the type
		visitor.get().enter_type(mc, ctx);
		// visit the instance
		lead_to_instance(visitor, mc, ctx, ptr_to_inst);
		// go through the base classes
		for_each<typename MetaClass::base_classes>(
			cref(show_bases_to(visitor, ptr_to_inst))
		);
		// go through the own class' attributes
		for_each<typename MetaClass::attributes>(
			cref(show_attribs_to(visitor, ptr_to_inst))
		);
		// leave the type
		visitor.get().leave_type(mc, ctx);
	}
};

template <class MetaClass, class Context = mpl::vector0<> >
class flat_traversal_of
: detail::traversal_utils<
	MetaClass, 
	Context,
	typename MetaClass::all_attributes,
	flat_traversal_of
>
{
public:
	template <class VisitorType>
	static void accept(
		VisitorType visitor,
		typename MetaClass::reflected_type* ptr_to_inst = 0
	)
	{
		do_accept(ref<VisitorType>(visitor), ptr_to_inst);
	}
	template <class VisitorType>
	static void accept(
		reference_wrapper<VisitorType> visitor,
		typename MetaClass::reflected_type* ptr_to_inst = 0
	)
	{
		do_accept(visitor, ptr_to_inst);
	}
private:
	template <class VisitorType>
	static void do_accept(
		reference_wrapper<VisitorType> visitor, 
		typename MetaClass::reflected_type* ptr_to_inst
	)
	{
		MetaClass mc;
		Context ctx;
		// enter the type
		visitor.get().enter_type(mc, ctx);
		// visit the instance
		lead_to_instance(visitor, mc, ctx, ptr_to_inst);
		// go through all of the class' attributes
		for_each<typename MetaClass::all_attributes>(
			cref(show_attribs_to(visitor, ptr_to_inst))
		);
		// leave the type
		visitor.get().leave_type(mc, ctx);
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard

