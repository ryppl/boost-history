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
#include <boost/mirror/detail/visitor_traits.hpp>
//
#include <boost/ref.hpp>
#include <boost/type_traits/remove_reference.hpp>
//
#include <assert.h>

namespace boost {
namespace mirror {


template <
	class MetaStructure, 
	class NodePath = mpl::vector0<> 
> class deep_traversal_of; 

template <
	class MetaStructure, 
	class NodePath = mpl::vector0<>
> class flat_traversal_of;

namespace detail {

template <
	class MetaClass,
	class NodePath,
	class MetaAttributes,
	template <class, class> class TraversalType
>
struct class_traversal_utils
{
protected:
	typedef typename mpl::push_back<NodePath, MetaClass>::type
		ClassNodePath;


	template <class VisitorType, class InstancePtr>
	class attribute_traversal
	{
	public:
		inline attribute_traversal(
			reference_wrapper<VisitorType> _visitor,
			InstancePtr _ptr_to_inst
		)
		: visitor(_visitor)
		, ptr_to_inst(_ptr_to_inst)
		{
			lead_into_attribute_list(visitor);
		}

		inline ~attribute_traversal(void)
		{
			lead_out_of_attribute_list(visitor);
		}

		template <class MetaAttribute>
		inline void operator ()(MetaAttribute ma) const
		{
			// update the traversal context
			typename mpl::push_back<
				ClassNodePath, 
				MetaAttributes
			>::type path;
			//
			// process a single attribute
			process_single(ma, path);
		}
	private:
		reference_wrapper<VisitorType> visitor;
		InstancePtr ptr_to_inst;

		// process single attribute WITH an instance
		template <class MetaAttribute, class AttribsNodePath>
		void inline process_single(
			MetaAttribute ma, 
			AttribsNodePath path 
		) const
		{
			// enter the attribute
			lead_into_attribute(visitor, ma, path);
			// 
			traverse_attribute(ma, path);
			// leave the attribute
			lead_out_of_attribute(visitor, ma, path);
		}

		template <class MetaAttribute, class AttribsNodePath>
		inline void traverse_attribute(
			MetaAttribute ma, 
			AttribsNodePath path
		) const
		{
			typedef BOOST_TYPEOF(ma.get(*ptr_to_inst)) instance_type;

			traverse_attribute_if(
				typename VisitorType::works_on_instances(),
				ma,
				path
			);
		}

		template <class MetaAttribute, class AttribsNodePath>
		inline void traverse_attribute_if(
			mpl::true_,
			MetaAttribute ma, 
			AttribsNodePath path
		) const
		{
			// the poiner has to be valid
			assert(ptr_to_inst != 0);
			// 
			// get an attribute instance
			typedef BOOST_TYPEOF(ma.get(*ptr_to_inst)) instance_type;
			instance_type instance(ma.get(*ptr_to_inst));
			//
			// traverse the attribute
			TraversalType<
				typename MetaAttribute::type,
				typename mpl::push_back<
					AttribsNodePath, 
					MetaAttribute
				>::type
			>::accept(visitor, &instance);
		}

		template <class MetaAttribute, class AttribsNodePath>
		inline void traverse_attribute_if(
			mpl::false_,
			MetaAttribute ma, 
			AttribsNodePath path
		) const
		{
			// traverse the attribute
			TraversalType<
				typename MetaAttribute::type,
				typename mpl::push_back<
					AttribsNodePath, 
					MetaAttribute
				>::type
			>::accept(visitor);
		}

		template < class MetaAttribute, class AttribsNodePath>
		static inline void lead_into_attribute(
			reference_wrapper<VisitorType> visitor,
			MetaAttribute ma,
			AttribsNodePath path
		)
		{
			lead_into_attribute_if(
				typename visitor_can_enter_attribute<
					VisitorType,
					MetaAttribute,
					meta_path::empty_context
				>::type(),
				visitor,
				ma,
				path
			);
		}

		template < class MetaAttribute, class AttribsNodePath>
		static inline void lead_into_attribute_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor,
			MetaAttribute ma,
			AttribsNodePath path
		)
		{
			visitor.get().enter_attribute(
				ma,
				meta_path::make_node_context(
					path,
					ma
				)
			);
		}

		template <class MetaAttribute, class AttribsNodePath>
		static inline void lead_into_attribute_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor,
			MetaAttribute ma,
			AttribsNodePath path
		){ }

		template < class MetaAttribute, class AttribsNodePath>
		static inline void lead_out_of_attribute(
			reference_wrapper<VisitorType> visitor,
			MetaAttribute ma,
			AttribsNodePath path
		)
		{
			lead_out_of_attribute_if(
				typename visitor_can_leave_attribute<
					VisitorType,
					MetaAttribute,
					meta_path::empty_context
				>::type(),
				visitor,
				ma,
				path
			);
		}

		template <class MetaAttribute, class AttribsNodePath>
		static inline void lead_out_of_attribute_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor,
			MetaAttribute ma,
			AttribsNodePath path 
		)
		{
			visitor.get().leave_attribute(
				ma,
				meta_path::make_node_context(
					path,
					ma
				)
			);
		}

		template <class MetaAttribute, class AttribsNodePath>
		static inline void lead_out_of_attribute_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor,
			MetaAttribute ma,
			AttribsNodePath path
		){ }

		static inline void lead_into_attribute_list(
			reference_wrapper<VisitorType> visitor
		)
		{
			lead_into_attribute_list_if(
				typename visitor_can_enter_attributes<
					VisitorType,
					MetaClass,
					MetaAttributes,
					meta_path::empty_context
				>::type(),
				visitor
			);
		}

		static inline void lead_into_attribute_list_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor
		)
		{
			visitor.get().enter_attributes(
				MetaClass(), 
				MetaAttributes(), 
				meta_path::make_node_context(
					ClassNodePath(),
					MetaAttributes()
				)
			);
		}

		static inline void lead_into_attribute_list_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor
		){ }

		static inline void lead_out_of_attribute_list(
			reference_wrapper<VisitorType> visitor
		)
		{
			lead_out_of_attribute_list_if(
				typename visitor_can_leave_attributes<
					VisitorType,
					MetaClass,
					MetaAttributes,
					meta_path::empty_context
				>::type(),
				visitor
			);
		}

		static inline void lead_out_of_attribute_list_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor
		)
		{
			visitor.get().leave_attributes(
				MetaClass(), 
				MetaAttributes(), 
				meta_path::make_node_context(
					ClassNodePath(),
					MetaAttributes()
				)
			);
		}

		static inline void lead_out_of_attribute_list_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor
		){ }

	};

	// attribute_traversal factory function
	template <class VisitorType, class InstancePtr>
	static inline attribute_traversal<VisitorType, InstancePtr>
	show_attribs_to(
		reference_wrapper<VisitorType> visitor,
		InstancePtr ptr_to_inst
	)
	{
		return attribute_traversal<VisitorType, InstancePtr>(
			visitor, 
			ptr_to_inst
		);
	}


	template <class VisitorType, class InstancePtr>
	class base_class_traversal
	{
	public:
		inline base_class_traversal(
			reference_wrapper<VisitorType> _visitor,
			InstancePtr _ptr_to_inst
		)
		: visitor(_visitor)
		, ptr_to_inst(_ptr_to_inst)
		{
			lead_into_base_class_list(visitor);
		}

		inline ~base_class_traversal(void)
		{
			lead_out_of_base_class_list(visitor);
		}

		template <class MetaInheritance>
		inline void operator ()(MetaInheritance mbc) const
		{
			BaseClassesNodePath path;
			//
			// enter the base cass
			lead_into_base_class(visitor, mbc, path);
			//
			// get the meta-class of the base class
			typedef typename MetaInheritance::base_class
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
			lead_out_of_base_class(visitor, mbc, path);
		}
	private:
		typedef typename mpl::push_back<
			ClassNodePath,
			typename MetaClass::base_classes
		>::type BaseClassesNodePath;

		reference_wrapper<VisitorType> visitor;
		InstancePtr ptr_to_inst;

		template <class MetaInheritance>
		static inline void lead_into_base_class(
			reference_wrapper<VisitorType> visitor,
			MetaInheritance mbc,
			BaseClassesNodePath path 
		)
		{
			lead_into_base_class_if(
				typename visitor_can_enter_base_class<
					VisitorType,
					MetaInheritance,
					meta_path::empty_context
				>::type(),
				visitor,
				mbc,
				path
			);
		}

		template <class MetaInheritance>
		static inline void lead_into_base_class_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor,
			MetaInheritance mbc,
			BaseClassesNodePath path 
		)
		{
			visitor.get().enter_base_class(
				mbc, 
				meta_path::make_node_context(
					path,
					mbc
				)
			);
		}

		template <class MetaInheritance>
		static inline void lead_into_base_class_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor,
			MetaInheritance mbc,
			BaseClassesNodePath path 
		){ }

		template <class MetaInheritance>
		static inline void lead_out_of_base_class(
			reference_wrapper<VisitorType> visitor,
			MetaInheritance mbc,
			BaseClassesNodePath path 
		)
		{
			lead_out_of_base_class_if(
				typename visitor_can_leave_base_class<
					VisitorType,
					MetaInheritance,
					meta_path::empty_context
				>::type(),
				visitor,
				mbc,
				path
			);
		}

		template <class MetaInheritance>
		static inline void lead_out_of_base_class_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor,
			MetaInheritance mbc,
			BaseClassesNodePath path 
		)
		{
			visitor.get().leave_base_class(
				mbc, 
				meta_path::make_node_context(
					path,
					mbc
				)
			);
		}

		template <class MetaInheritance>
		static inline void lead_out_of_base_class_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor,
			MetaInheritance mbc,
			BaseClassesNodePath path
		){ }

		static inline void lead_into_base_class_list(
			reference_wrapper<VisitorType> visitor
		)
		{
			lead_into_base_class_list_if(
				typename visitor_can_enter_base_classes<
					VisitorType,
					MetaClass,
					typename MetaClass::base_classes,
					meta_path::node_context<
						ClassNodePath,
						typename MetaClass::base_classes
					>
				>::type(),
				visitor
			);
		}

		static inline void lead_into_base_class_list_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor
		)
		{
			visitor.get().enter_base_classes(
				MetaClass(), 
				typename MetaClass::base_classes(),
				meta_path::make_node_context(
					ClassNodePath(),
					typename MetaClass::base_classes()
				)
			);
		}

		static inline void lead_into_base_class_list_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor
		){ }

		static inline void lead_out_of_base_class_list(
			reference_wrapper<VisitorType> visitor
		)
		{
			lead_out_of_base_class_list_if(
				typename visitor_can_leave_base_classes<
					VisitorType,
					MetaClass,
					typename MetaClass::base_classes,
					meta_path::node_context<
						ClassNodePath,
						typename MetaClass::base_classes
					>
				>::type(),
				visitor
			);
		}

		static inline void lead_out_of_base_class_list_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor
		)
		{
			visitor.get().leave_base_classes(
				MetaClass(), 
				typename MetaClass::base_classes(),
				meta_path::make_node_context(
					ClassNodePath(),
					typename MetaClass::base_classes()
				)
			);
		}

		static inline void lead_out_of_base_class_list_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor
		){ }


	};

	// base class traversal factory function
	template <class VisitorType, class InstancePtr>
	static inline base_class_traversal<VisitorType, InstancePtr>
	show_bases_to(
		reference_wrapper<VisitorType> visitor,
		InstancePtr ptr_to_inst
	)
	{
		return base_class_traversal<VisitorType, InstancePtr>(
			visitor, 
			ptr_to_inst
		);
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
		lead_to_instance_if(
			typename VisitorType::works_on_instances(),
			visitor,
			mc,
			path,
			ptr_to_inst
		);
	}

	template <
		class VisitorType, 
		class InstanceType
	>
	inline static void lead_to_instance_if(
		mpl::true_,
		reference_wrapper<VisitorType> visitor,
		MetaClass mc,
		NodePath path,
		InstanceType* ptr_to_inst
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

	template <
		class VisitorType, 
		class InstanceType
	>
	inline static void lead_to_instance_if(
		mpl::false_,
		reference_wrapper<VisitorType> visitor,
		MetaClass mc,
		NodePath path,
		InstanceType* ptr_to_inst
	) { }

	template <class VisitorType>
	static inline void lead_into_type(
                reference_wrapper<VisitorType> visitor,
		MetaClass mc,
                NodePath path
        )
        {
		lead_into_type_if(
			typename visitor_can_enter_type<
				VisitorType,
				MetaClass,
				meta_path::empty_context
			>::type(),
			visitor,
			mc,
			path
		);
	}

        template < class VisitorType>
        static inline void lead_into_type_if(
		mpl::true_,
                reference_wrapper<VisitorType> visitor,
                MetaClass mc,
                NodePath path
        )
        {
                visitor.get().enter_type(
                        mc,
                        meta_path::make_node_context(
                                path,
                                mc
                        )
                );
        }

        template < class VisitorType>
        static inline void lead_into_type_if(
		mpl::false_,
                reference_wrapper<VisitorType> visitor,
                MetaClass mc,
                NodePath path
        ){ }

	template <class VisitorType>
	static inline void lead_out_of_type(
                reference_wrapper<VisitorType> visitor,
		MetaClass mc,
                NodePath path
        )
        {
		lead_out_of_type_if(
			typename visitor_can_leave_type<
				VisitorType,
				MetaClass,
				meta_path::empty_context
			>::type(),
			visitor,
			mc,
			path
		);
	}

        template < class VisitorType>
        static inline void lead_out_of_type_if(
		mpl::true_,
                reference_wrapper<VisitorType> visitor,
                MetaClass mc,
                NodePath path
        )
        {
                visitor.get().leave_type(
                        mc,
                        meta_path::make_node_context(
                                path,
                                mc
                        )
                );
        }

        template < class VisitorType>
        static inline void lead_out_of_type_if(
		mpl::false_,
                reference_wrapper<VisitorType> visitor,
                MetaClass mc,
                NodePath path
        ){ }

};

/** Base implementation of namespace traversal
 */
template <
	class MetaNamespace,
	class NodePath,
	template <class, class> class TraversalType
> class namespace_traversal_utils
{
private:
	typedef typename mpl::push_back<
		NodePath,
		MetaNamespace
	>::type NamespaceNodePath;
protected:
	template <class VisitorType, class Members>
	struct namespace_member_traversal
	{
		reference_wrapper<VisitorType> visitor;

		typedef typename mpl::push_back<
			NamespaceNodePath,
			Members
		>::type MembersNodePath;
		
		inline namespace_member_traversal(
			reference_wrapper<VisitorType> _visitor
		): visitor(_visitor)
		{
			lead_into_namespace_member_list(
				visitor,
				Members(),
				NamespaceNodePath()
			);
		}

		template <class MetaObject>
		inline void operator ()(MetaObject mo) const
		{
                        TraversalType<
                                MetaObject,
				MembersNodePath
                        >::accept(visitor);
		}

		inline ~namespace_member_traversal(void)
		{
			lead_out_of_namespace_member_list(
				visitor,
				Members(),
				NamespaceNodePath()
			);
		}

		static inline void lead_into_namespace_member_list(
	                reference_wrapper<VisitorType> visitor,
			Members m,
			NamespaceNodePath path
	        )
	        {
			lead_into_namespace_member_list_if(
				typename visitor_can_enter_namespace_members<
					VisitorType,
					Members,
					meta_path::empty_context
				>::type(),
				visitor,
				m,
				path
			);
		}

		static inline void lead_into_namespace_member_list_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor,
			Members m,
			NamespaceNodePath path
		)
		{
			visitor.get().enter_namespace_members(
				m,
				meta_path::make_node_context(
					path,
					m
				)
			);
		}

		static inline void lead_into_namespace_member_list_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor,
			Members m,
			NamespaceNodePath path
		){ }

		static inline void lead_out_of_namespace_member_list(
	                reference_wrapper<VisitorType> visitor,
			Members m,
			NamespaceNodePath path
	        )
	        {
			lead_out_of_namespace_member_list_if(
				typename visitor_can_leave_namespace_members<
					VisitorType,
					Members,
					meta_path::empty_context
				>::type(),
				visitor,
				m,
				path
			);
		}

		static inline void lead_out_of_namespace_member_list_if(
			mpl::true_,
			reference_wrapper<VisitorType> visitor,
			Members m,
			NamespaceNodePath path
		)
		{
			visitor.get().leave_namespace_members(
				m,
				meta_path::make_node_context(
					path,
					m
				)
			);
		}

		static inline void lead_out_of_namespace_member_list_if(
			mpl::false_,
			reference_wrapper<VisitorType> visitor,
			Members m,
			NamespaceNodePath path
		){ }
	};

	template <class Members, class VisitorType>
	static inline namespace_member_traversal<VisitorType, Members>
	show_namespace_members_to(reference_wrapper<VisitorType> _visitor, Members)
	{
		return namespace_member_traversal<VisitorType, Members>(_visitor);
	}

	
	template <class VisitorType>
	static inline void lead_into_namespace(
                reference_wrapper<VisitorType> visitor,
		MetaNamespace mn,
                NodePath path
        )
        {
		lead_into_namespace_if(
			typename visitor_can_enter_namespace<
				VisitorType,
				MetaNamespace,
				meta_path::empty_context
			>::type(),
			visitor,
			mn,
			path
		);
	}
 
        template <class VisitorType>
        static inline void lead_into_namespace_if(
		mpl::true_,
                reference_wrapper<VisitorType> visitor,
		MetaNamespace mn,
                NodePath path
        )
        {
                visitor.get().enter_namespace(
                        mn,
                        meta_path::make_node_context(
                                path,
                                mn
                        )
                );
        }

        template < class VisitorType>
        static inline void lead_into_namespace_if(
		mpl::false_,
                reference_wrapper<VisitorType> visitor,
		MetaNamespace mn,
                NodePath path
        ){ }

       	template <class VisitorType>
	static inline void lead_out_of_namespace(
                reference_wrapper<VisitorType> visitor,
		MetaNamespace mn,
                NodePath path
        )
        {
		lead_out_of_namespace_if(
			typename visitor_can_leave_namespace<
				VisitorType,
				MetaNamespace,
				meta_path::empty_context
			>::type(),
			visitor,
			mn,
			path
		);
	}
 
	template < class VisitorType>
        static inline void lead_out_of_namespace_if(
		mpl::true_,
                reference_wrapper<VisitorType> visitor,
		MetaNamespace mn,
                NodePath path
        )
        {
                visitor.get().leave_namespace(
                        mn,
                        meta_path::make_node_context(
                                path,
                                mn
                        )
                );
        }

        template < class VisitorType>
        static inline void lead_out_of_namespace_if(
		mpl::false_,
                reference_wrapper<VisitorType> visitor,
		MetaNamespace mn,
                NodePath path
        ){ }

};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

