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

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>
//
#include <boost/ref.hpp>
//
#include <assert.h>

namespace boost {
namespace mirror {


template <class MetaClass> class deep_traversal_of;
template <class MetaClass> class flat_traversal_of;

namespace detail {

	template <
		class MetaClass, 
		class MetaAttributes,
		template <class> class TraversalType
	>
	struct traversal_utils
	{
	protected:
		template <class VisitorType>
		class attribute_traversal
		{
		public:
			attribute_traversal(
				reference_wrapper<VisitorType> _visitor,
				typename MetaClass::reflected_type* _ptr_to_inst
			)
			: visitor(_visitor)
			, ptr_to_inst(_ptr_to_inst)
			{
				visitor.enter_attributes(MetaClass(), MetaAttributes());
			}
	
			~attribute_traversal(void)
			{
				visitor.leave_attributes(MetaClass(), MetaAttributes());
			}
	
			template <class MetaAttribute>
			void operator ()(MetaAttribute ma)
			{
				process_single(ma, typename VisitorType::works_on_instances());
			}
		private:
			VisitorType& visitor;
			typename MetaClass::reflected_type* ptr_to_inst;

			// process single attribute WITH an instance
			template <class MetaAttribute>
			void process_single(MetaAttribute ma, mpl::bool_<true>)
			{
				visitor.enter_attribute(ma);
				typedef typename MetaAttribute::type attrib_type;
				assert(ptr_to_inst != 0);
				typedef BOOST_TYPEOF(ma.get(*ptr_to_inst)) instance_type;
				instance_type instance(ma.get(*ptr_to_inst));
				TraversalType<
					BOOST_MIRROR_REFLECT_CLASS(attrib_type)
				>::accept(visitor, &instance);
				visitor.leave_attribute(ma);
			}

			// process single attribute W/O an instance
			template <class MetaAttribute>
			void process_single(MetaAttribute ma, mpl::bool_<false>)
			{
				visitor.enter_attribute(ma);
				typedef typename MetaAttribute::type attrib_type;
				TraversalType<
					BOOST_MIRROR_REFLECT_CLASS(attrib_type)
				>::accept(visitor, 0);
				visitor.leave_attribute(ma);
			}
		};
	
		template <class VisitorType>
		static inline attribute_traversal<VisitorType>
		show_attribs_to(
			reference_wrapper<VisitorType> visitor,
			typename MetaClass::reflected_type* ptr_to_inst
		)
		{
			return attribute_traversal<VisitorType>(visitor, ptr_to_inst);
		}
	

		template <class VisitorType>
		class base_class_traversal
		{
		public:
			base_class_traversal(
				reference_wrapper<VisitorType> _visitor,
				typename MetaClass::reflected_type* _ptr_to_inst
			)
			: visitor(_visitor)
			, ptr_to_inst(_ptr_to_inst)
			{
				visitor.enter_base_classes(MetaClass());
			}
	
			~base_class_traversal(void)
			{
				visitor.leave_base_classes(MetaClass());
			}
	
			template <class MetaInheritance>
			void operator ()(MetaInheritance mbc)
			{
				visitor.enter_base_class(mbc);
				typedef MetaInheritance meta_inheritance;
				typedef typename meta_inheritance::meta_base_class
					meta_base_class;
				TraversalType<meta_base_class>::accept(visitor, ptr_to_inst);
				visitor.leave_base_class(mbc);
			}
		private:
			VisitorType& visitor;
			typename MetaClass::reflected_type* ptr_to_inst;
		};

		template <class VisitorType>
		static inline base_class_traversal<VisitorType>
		show_bases_to(
			reference_wrapper<VisitorType> visitor,
			typename MetaClass::reflected_type* ptr_to_inst
		)
		{
			return base_class_traversal<VisitorType>(visitor, ptr_to_inst);
		}
	};


} // namespace detail

template <class MetaClass>
class deep_traversal_of 
: detail::traversal_utils<
	MetaClass, 
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
		// enter the type
		visitor.get().enter_type(mc);
		// visit the instance
		visitor.get().visit_instance(mc, ptr_to_inst);
		// go through the base classes
		for_each<typename MetaClass::base_classes>(
			show_bases_to(visitor, ptr_to_inst)
		);
		// go through the own class' attributes
		for_each<typename MetaClass::attributes>(
			show_attribs_to(visitor, ptr_to_inst)
		);
		// leave the type
		visitor.get().leave_type(mc);
	}
};

template <class MetaClass>
class flat_traversal_of
: detail::traversal_utils<
	MetaClass, 
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
		// enter the type
		visitor.get().enter_type(mc);
		// visit the instance
		visitor.get().visit_instance(mc, ptr_to_inst);
		// go through all of the class' attributes
		for_each<typename MetaClass::all_attributes>(
			show_attribs_to(visitor, ptr_to_inst)
		);
		// leave the type
		visitor.get().leave_type(mc);
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard

