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


template <class MetaClass, class Context> class deep_traversal_of;
template <class MetaClass, class Context> class flat_traversal_of;

namespace detail {

	template <
		class MetaClass,
		class Context,
		class MetaAttributes,
		template <class, class> class TraversalType
	>
	struct traversal_utils
	{
	protected:
		typedef typename mpl::push_back<Context, MetaClass>::type
			ClassContext;

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
				visitor.enter_attributes(
					MetaClass(), 
					MetaAttributes(), 
					ClassContext()
				);
			}
	
			~attribute_traversal(void)
			{
				visitor.leave_attributes(
					MetaClass(), 
					MetaAttributes(), 
					ClassContext()
				);
			}
	
			template <class MetaAttribute>
			void operator ()(MetaAttribute ma) const
			{
				// update the traversal context
				typename mpl::push_back<
					ClassContext, 
					MetaAttributes
				>::type ctx;
				//
				// process a single attribute
				process_single(
					ma, 
					ctx, 
					typename VisitorType::works_on_instances()
				);
			}
		private:
			VisitorType& visitor;
			typename MetaClass::reflected_type* ptr_to_inst;

			// process single attribute WITH an instance
			template <class MetaAttribute, class AttribsContext>
			void process_single(
				MetaAttribute ma, 
				AttribsContext ctx, 
				mpl::bool_<true>
			) const
			{
				// enter the attribute
				visitor.enter_attribute(ma, ctx);
				// 
				// get the type of the attribute
				typedef typename MetaAttribute::type attrib_type;
				//
				// the poiner has to be valid
				assert(ptr_to_inst != 0);
				// 
				// get an attribute instance
				typedef BOOST_TYPEOF(ma.get(*ptr_to_inst)) instance_type;
				instance_type instance(ma.get(*ptr_to_inst));
				//
				// traverse the attribute
				TraversalType<
					BOOST_MIRROR_REFLECT_CLASS(attrib_type),
					typename mpl::push_back<
						AttribsContext, 
						MetaAttribute
					>::type
				>::accept(visitor, &instance);
				//
				// leave the attribute
				visitor.leave_attribute(ma, ctx);
			}

			// process single attribute W/O an instance
			template <class MetaAttribute, class AttribsContext>
			void process_single(
				MetaAttribute ma, 
				AttribsContext ctx, 
				mpl::bool_<false>
			) const
			{
				// enter the attribute
				visitor.enter_attribute(ma, ctx);
				//
				// traverse the attributes
				typedef typename MetaAttribute::type attrib_type;
				TraversalType<
					BOOST_MIRROR_REFLECT_CLASS(attrib_type),
					typename mpl::push_back<
						AttribsContext, 
						MetaAttribute
					>::type
				>::accept(visitor, 0);
				//
				// leave the attributes
				visitor.leave_attribute(ma, ctx);
			}
		};
	
		// attribute_traversal factory function
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
				visitor.enter_base_classes(
					MetaClass(), 
					ClassContext()
				);
			}
	
			~base_class_traversal(void)
			{
				visitor.leave_base_classes(
					MetaClass(), 
					ClassContext()
				);
			}
	
			template <class MetaInheritance>
			void operator ()(MetaInheritance mbc) const
			{
				ClassContext ctx;
				//
				// enter the base cass
				visitor.enter_base_class(mbc, ctx);
				//
				// get the meta-class of the base class
				typedef typename MetaInheritance::meta_base_class
					meta_base_class;
				// traverse the base class
				TraversalType<
					meta_base_class,
					typename mpl::push_back<
						ClassContext, 
						MetaInheritance
					>::type
				>::accept(visitor, ptr_to_inst);
				// leave the base class
				visitor.leave_base_class(mbc, ctx);
			}
		private:
			VisitorType& visitor;
			typename MetaClass::reflected_type* ptr_to_inst;
		};

		// base class traversal factory function
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
		visitor.get().visit_instance(mc, ctx, ptr_to_inst);
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
		visitor.get().visit_instance(mc, ctx, ptr_to_inst);
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

