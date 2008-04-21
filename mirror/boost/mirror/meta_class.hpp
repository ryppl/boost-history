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

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/joint_view.hpp>
//
// basic meta types
#include <boost/mirror/meta_type.hpp>
// reflection of class inheritance
#include <boost/mirror/meta_inheritance.hpp>
// inheritance trait
#include <boost/mirror/reflects_virtual_inheritance.hpp>
// reflection of class member attributes
#include <boost/mirror/meta_attributes.hpp>

namespace boost {
namespace mirror {

/** Meta class - specializes the meta_type for classes
 */
template <
	class reflected_class, 
	class variant_tag
>
struct meta_class : public meta_type<reflected_class>
{
	/** The base classes of a class.
	 *  The member base_classes::list is a mpl::vector of 
	 *  meta_inheritance<> specializations, one for every 
	 *  base class.
	 */
	typedef meta_base_classes<reflected_class, variant_tag> base_classes;

	/** The member attributes of the class (not includeing the inherited
	 *  member attribs. 
	 *  The attributes::type_list is a mpl::vector of types 
	 *  of the attributes.
	 *
	 *  The attributes::get(mpl::int_<I>) functions allow to get the value
	 *  of the I-th attribute, if the attribute is not write-only, where:
	 *  0 <= I < N; N = $mpl::size<attributes::type_list>::value
	 *
	 * The attributes::set(mpl::int_<I>), T val) functions allow to set
	 * the value of the I-th attribute, if it's not read-only, where
	 *  0 <= I < N; N = $mpl::size<attributes::type_list>::value
	 * 
	 */
	typedef meta_class_attributes<reflected_class, variant_tag> attributes;

	/** This is basically the same as the "attributes" structure
	 *  but allows to work with all member attributes including
	 *  the inherited ones. 
	 */
	struct all_attributes
	{
		/** This struct "hides" the internal helpers
		 */
		struct detail 
		{
			/** The list of non-virtual base classes in the same
			 *  order as they were registered.
			 */
			typedef typename mpl::remove_if<
				typename base_classes::list,
				mpl::lambda<
					reflects_virtual_inheritance<mpl::_1>
				>::type
			>::type list_of_regular_base_classes;

			/** The list of directly inhertied virtual base classes.
			 */
			typedef typename mpl::remove_if<
				typename base_classes::list,
				mpl::lambda<
					mpl::not_<reflects_virtual_inheritance<mpl::_1> >
				>::type
			>::type list_of_virtual_base_classes;

			/** This template gets the regular_base_class_layout
			 *  of a base class when given a meta_inheritance 
			 *  specialization for this base class.
			 */
			template <class meta_inheritance>
			struct get_base_class_regular_layout
			{
				typedef typename 
					meta_inheritance:: 
					meta_class:: 
					all_attributes:: 
					detail:: 
					regular_base_class_layout type;
			};
			
			/** The layout of non-virtual base classes 
			 *  of this class, stored as a mpl::vector 
			 *  of meta_inheritances.
			 */
			typedef typename mpl::accumulate<
				list_of_regular_base_classes,
				mpl::vector<>,
				mpl::push_back<
					mpl::insert_range<
						mpl::_1,
						mpl::end<mpl::_1>,
						get_base_class_regular_layout<
							mpl::_2
						>
					>,
					mpl::_2
				>
			>::type regular_base_class_layout;
			
			/** This template gets the virtual_base_class_layout
			 *  of a base class when given a meta_inheritance 
			 *  specialization for this base class.
			 */
			template <class meta_inheritance>
			struct get_base_class_virtual_layout
			{
				typedef typename 
					meta_inheritance:: 
					meta_class:: 
					all_attributes:: 
					detail:: 
					virtual_base_class_layout type;
			};
	
			/** This template gets the base_class_layout
			 *  of a base class when given a meta_inheritance 
			 *  specialization for this base class.
			 */
			template <class meta_inheritance>
			struct get_base_class_layout
			{
				typedef typename 
					meta_inheritance:: 
					meta_class:: 
					all_attributes:: 
					detail:: 
					base_class_layout type;
			};
	
			/** The list of vitual base classes of this class.
			 *  This list still contains dupplicates, that are
			 *  removed to form virtual_base_class_layout
			 */
			typedef typename mpl::accumulate<
				typename base_classes::list,
				mpl::vector<>,
				mpl::if_<
					reflects_virtual_inheritance<
						mpl::_2
					>,
					mpl::push_back<
						mpl::insert_range<
							mpl::_1,
							mpl::end<mpl::_1>,
							get_base_class_layout<
								mpl::_2
							>
						>,
						mpl::_2
					>,
					mpl::insert_range<
						mpl::_1,
						mpl::end<mpl::_1>,
						get_base_class_virtual_layout<
							mpl::_2
						>
					>
				>
			>::type virtual_base_class_layout_w_dups;
			
			/** The layout of virtual base classes 
			 *  of this class, stored as a mpl::vector 
			 *  of meta_inheritances.
			 */
			typedef typename mpl::fold<
				virtual_base_class_layout_w_dups,
				mpl::vector0<>,
				mpl::if_<
					mpl::contains<
						mpl::_1,
						mpl::_2
					>,
					mpl::_1,
					mpl::push_back<
						mpl::_1,
						mpl::_2
					>
				>
			>::type virtual_base_class_layout;
			
			/** This template gets the list of member
			 *  attrbute types of a base class when 
			 *  given a meta_inheritance<> specialization
			 */
			template <class meta_inheritance>
			struct get_base_class_attrib_type_list
			{
				typedef typename 
					meta_inheritance::
					meta_class::
					attributes::
					type_list type;
	
			};

			typedef typename mpl::joint_view<
				virtual_base_class_layout,
				regular_base_class_layout
			>::type base_class_layout;

			/** The list of inherited member attributes
			 *  of the reflected class. 
			 *  NOTE: this implementation puts the
			 *  members of the virtual bases before
			 *  the other members
			 */
			typedef typename mpl::accumulate<
				base_class_layout,
				mpl::vector0<>,
				mpl::insert_range<
					mpl::_1,
					mpl::end<mpl::_1>,
					get_base_class_attrib_type_list<
						mpl::_2
					>
				>
			>::type inherited_member_attrib_type_list;

			/** The list of types of all attributes including
		 	*  the inherited ones.
		 	*/
			typedef typename mpl::joint_view<
				typename detail::inherited_member_attrib_type_list,
				typename meta_class<reflected_class, variant_tag>::attributes::type_list
			>::type member_attrib_type_list;

			/** The count of virtually inherited attributes
			 */
			typedef typename mpl::accumulate<
				typename mpl::transform<
					virtual_base_class_layout,
					get_base_class_attrib_type_list<
						mpl::_1
					>
				>::type,
				mpl::int_<0>,
				mpl::plus<
					mpl::_1,
					mpl::size< mpl::_2 >
				>
			>::type virtual_attrib_count;
		

			/** Is a true type if the I-th member attribute is
			 *  virtually inherited.
			 */
			template <int I>
			struct is_virtually_inherited 
			: mpl::less<
				mpl::int_<I>,
				virtual_attrib_count
			>::type { };
		
			/** This template is used to query the return value
			 *  type of the getter for the I-th member attribute
			 */
			template <int I>
			struct result_of_get
			{
				typedef typename mpl::at<
						member_attrib_type_list, 
						mpl::int_<I> 
					>::type type;
			};		
	
			/** This function is used to get the members of virtual
		 	 *  base classes.
		 	 */
			template <class a_class, int I>
			static typename result_of_get<I>::type
			get(a_class context, mpl::int_<I> pos, mpl::bool_<true>)
			{
				typedef meta_class<a_class, variant_tag> meta_class;
				
				return meta_class::attributes::get(context, pos);
			}
		
			/** This function is used to get the members of regular
		 	 *  base classes.
		 	 */
			template <class a_class, int I>
			static typename result_of_get<I>::type
			get(a_class context, mpl::int_<I> pos, mpl::bool_<false>)
			{
				typedef meta_class<a_class, variant_tag> meta_class;

				return meta_class::attributes::get(context, pos);

			}
		}; // struct detail
		
		/** The list of inherited attribute types
		 */
		typedef typename detail::inherited_member_attrib_type_list inherited_type_list;

		/** The size of the type_list, i.e. the count of inherited attributes
		 */
		struct inherited_size : public mpl::size<inherited_type_list> { };

		/** The list of types of all attributes including
		 *  the inherited ones.
		 */
		typedef typename detail::member_attrib_type_list type_list;
		
		/** The size of the type_list, i.e. the count of all attributes
		 */
		struct size : public mpl::size<type_list> { };
		
		/**
		template <class a_class, int I>
		static typename detail::result_of_get<I>::type
		get(a_class context, mpl::int_<I> pos)
		{
			// is the attrib virtually inherited
			typedef typename detail::
				is_virtually_inherited<I> is_virtual;		
			//
			// the index of the attribute
			typedef typename mpl::if<
					is_virtual,
					pos,
					mpl::minus<
						pos
					>
				>::type att_pos;
			//
			return detail::get(
				context, 
				att_pos,
				is_virtual
			);
		}
		 */
	}; // all_attrbutes

};


/** This macro should be included in the definition of every class
 *  with private or protected members, that should be refleccted
 */
#define BOOST_MIRROR_FRIENDLY_CLASS(CLASS_NAME) \
		friend struct ::boost::mirror::meta_class_attributes<CLASS_NAME>;



} // namespace mirror
} // namespace boost

#endif //include guard

