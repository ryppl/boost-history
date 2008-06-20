/**
 * \file boost/mirror/detail/traversal.hpp
 * Traversal of meta classes by a visitor
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_DETAIL_TRAVERSAL_HPP
#define BOOST_MIRROR_DETAIL_TRAVERSAL_HPP

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>
#include <boost/mirror/meta_path/node_context.hpp>
//
#include <boost/ref.hpp>
//
#include <assert.h>

namespace boost {
namespace mirror {


template <class MetaClass, class NodePath> class deep_traversal_of;
template <class MetaClass, class NodePath> class flat_traversal_of;

namespace detail {

	template <
		class MetaClass,
		class NodePath,
		class MetaAttributes,
		template <class, class> class TraversalType
	>
	struct traversal_utils
	{
	protected:
		typedef typename mpl::push_back<NodePath, MetaClass>::type
			ClassNodePath;

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
					meta_path::make_node_context(
						ClassNodePath(),
						MetaAttributes()
					)
				);
			}
	
			~attribute_traversal(void)
			{
				visitor.leave_attributes(
					MetaClass(), 
					MetaAttributes(), 
					meta_path::make_node_context(
						ClassNodePath(),
						MetaAttributes()
					)
				);
			}
	
			template <class MetaAttribute>
			void operator ()(MetaAttribute ma) const
			{
				// update the traversal context
				typename mpl::push_back<
					ClassNodePath, 
					MetaAttributes
				>::type path;
				//
				// process a single attribute
				process_single(
					ma, 
					path,
					typename VisitorType::works_on_instances()
				);
			}
		private:
			VisitorType& visitor;
			typename MetaClass::reflected_type* ptr_to_inst;

			// process single attribute WITH an instance
			template <class MetaAttribute, class AttribsNodePath>
			void process_single(
				MetaAttribute ma, 
				AttribsNodePath path, 
				mpl::bool_<true>
			) const
			{
				// enter the attribute
				visitor.enter_attribute(
					ma, 
					meta_path::make_node_context(
						path,
						ma
					)
				);
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
						AttribsNodePath, 
						MetaAttribute
					>::type
				>::accept(visitor, &instance);
				//
				// leave the attribute
				visitor.leave_attribute(
					ma, 
					meta_path::make_node_context(
						path,
						ma
					)
				);
			}

			// process single attribute W/O an instance
			template <class MetaAttribute, class AttribsNodePath>
			void process_single(
				MetaAttribute ma, 
				AttribsNodePath path, 
				mpl::bool_<false>
			) const
			{
				// enter the attribute
				visitor.enter_attribute(
					ma, 
					meta_path::make_node_context(
						path,
						ma
					)
				);
				//
				// traverse the attributes
				typedef typename MetaAttribute::type attrib_type;
				TraversalType<
					BOOST_MIRROR_REFLECT_CLASS(attrib_type),
					typename mpl::push_back<
						AttribsNodePath, 
						MetaAttribute
					>::type
				>::accept(visitor, 0);
				//
				// leave the attributes
				visitor.leave_attribute(
					ma, 
					meta_path::make_node_context(
						path,
						ma
					)
				);
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
					typename MetaClass::base_classes(),
					meta_path::make_node_context(
						ClassNodePath(),
						typename MetaClass::base_classes()
					)
				);
			}
	
			~base_class_traversal(void)
			{
				visitor.leave_base_classes(
					MetaClass(), 
					typename MetaClass::base_classes(),
					meta_path::make_node_context(
						ClassNodePath(),
						typename MetaClass::base_classes()
					)
				);
			}
	
			template <class MetaInheritance>
			void operator ()(MetaInheritance mbc) const
			{
				typedef typename mpl::push_back<
					ClassNodePath,
					typename MetaClass::base_classes
				>::type BaseClassesNodePath;
				BaseClassesNodePath path;
				//
				// enter the base cass
				visitor.enter_base_class(
					mbc, 
					meta_path::make_node_context(
						path,
						mbc
					)
				);
				//
				// get the meta-class of the base class
				typedef typename MetaInheritance::meta_base_class
					meta_base_class;
				// traverse the base class
				TraversalType<
					meta_base_class,
					typename mpl::push_back<
						BaseClassesNodePath, 
						MetaInheritance
					>::type
				>::accept(visitor, ptr_to_inst);
				// leave the base class
				visitor.leave_base_class(
					mbc, 
					meta_path::make_node_context(
						path,
						mbc
					)
				);
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

		template <
			class VisitorType, 
			class InstanceType
		>
		inline static void lead_to_instance(
			reference_wrapper<VisitorType> visitor,
			MetaClass mc,
			NodePath path,
			InstanceType* ptr_to_inst
		)
		{
			do_lead_to_instance(
				visitor,
				mc,
				path,
				ptr_to_inst,
				typename VisitorType::works_on_instances()
			);
		}
	
	private:
		template <
			class VisitorType, 
			class InstanceType
		>
		inline static void do_lead_to_instance(
			reference_wrapper<VisitorType> visitor, 
			MetaClass,
			NodePath, 
			InstanceType* ptr_to_inst,
			mpl::bool_<false>
		){ }

		template <
			class VisitorType, 
			class InstanceType
		>
		inline static void do_lead_to_instance(
			reference_wrapper<VisitorType> visitor, 
			MetaClass mc,
			NodePath path,
			InstanceType* ptr_to_inst,
			mpl::bool_<true>
		)
		{
			visitor.get().visit_instance(
				mc, 
				meta_path::make_node_context(
					path,
					mc
				),
				ptr_to_inst
			);
		}

	};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

