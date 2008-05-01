/**
 * \file boost/mirror/meta_attributes.hpp
 * Registering and reflection of class attributes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_ATTRIBUTES_HPP
#define BOOST_MIRROR_META_ATTRIBUTES_HPP

// macros for declaration of simple meta attributes
#include <boost/mirror/detail/meta_attribs_base.hpp>
// macros for declaration of meta attributes accessed through getters/setters
#include <boost/mirror/detail/meta_attribs_getset.hpp>

namespace boost {
namespace mirror {

	/** This is basically the same as the "attributes" structure
	 *  but allows to work with all member attributes including
	 *  the inherited ones. 
	 */
	template <class meta_class>
	struct meta_class_all_attributes
	: detail::meta_class_attrib_utils<meta_class, meta_class_all_attributes<meta_class> >
	{
		/** This struct "hides" the internal helpers
		 */
		struct detail 
		{
			/** The list of non-virtual base classes in the same
			 *  order as they were registered.
			 */
			typedef typename mpl::remove_if<
				typename meta_class::base_classes::list,
				mpl::lambda<
					reflects_virtual_inheritance<mpl::_1>
				>::type
			>::type list_of_regular_base_classes;

			/** The list of directly inhertied virtual base classes.
			 */
			typedef typename mpl::remove_if<
				typename meta_class::base_classes::list,
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
				mpl::vector0<>,
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
				typename meta_class::base_classes::list,
				mpl::vector0<>,
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
				typedef typename meta_inheritance::
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
			 *  the other members.
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
				typename meta_class::attributes::type_list
			>::type member_attrib_type_list;


			/** This template gets the list of the owner classes 
			 *  for the inherited attributes.
			 */
			template <class current_list, class meta_inheritance>
			struct get_base_class_attrib_owner_and_offs
			{
				typedef typename meta_inheritance::
						meta_class base_meta_class;

				typedef typename base_meta_class::
						attributes::
						type_list type_list;

				typedef typename mpl::size<
						current_list
				>::type offset;
				typedef typename mpl::pair<
						base_meta_class,
						offset		
					> pair;
				
				template<typename T>
				struct get_pair
				{
					typedef pair type;
				};

				typedef typename mpl::accumulate<
					type_list,
					current_list,
					mpl::push_back<
						mpl::_1,
						get_pair<mpl::_>
					>
				>::type type;
			};

			/** This is a list that contains a pair of owner meta_class
			 *  and the index offset for every inherited attribute.
			 */
			typedef typename mpl::accumulate<
				base_class_layout,
				mpl::vector0<>,
				get_base_class_attrib_owner_and_offs<
					mpl::_1,
					mpl::_2
				>
			>::type inherited_attrib_owners_and_offsets;

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

			template <int I>
			struct inherited_attrib_meta_class_and_pos
			{
				typedef typename mpl::at<
					typename detail::inherited_attrib_owners_and_offsets,
					mpl::int_<I>
				>::type owner_and_offset;

				typedef typename owner_and_offset::first meta_class;
				typedef typename mpl::int_<mpl::minus<
					mpl::int_<I>,
					typename owner_and_offset::second
				>::value> position;
			};

			template <int I>
			struct own_attrib_meta_class_and_pos
			{
				typedef typename mpl::int_<mpl::minus<
					mpl::int_<I>,
					typename mpl::size<inherited_member_attrib_type_list>::type
				>::value> position;
			};

			/** This function is used to get the names of the member 
		 	 *  attributes from the base classes.
		 	 */
			template <int I>
			static const bchar*
			base_name(mpl::int_<I> pos, mpl::bool_<true>)
			{
				typedef typename inherited_attrib_meta_class_and_pos<I>
					::meta_class meta_class;
				typedef typename inherited_attrib_meta_class_and_pos<I>
					::position new_pos_type;

				return meta_class::attributes::base_name(new_pos_type());
			}

			template <int I>
			static const bchar*
			base_name(mpl::int_<I> pos, mpl::bool_<false>)
			{
				typedef typename own_attrib_meta_class_and_pos<I>
					::position new_pos_type;

				return meta_class::attributes::base_name(new_pos_type());
			}

		
			/** This function is used to get the member attributes 
		 	 *  from the base classes.
		 	 */
			template <class a_class, int I>
			static typename result_of_get<I>::type
			get(a_class context, mpl::int_<I> pos, mpl::bool_<true>)
			{
				typedef typename inherited_attrib_meta_class_and_pos<I>
					::meta_class meta_class;
				typedef typename inherited_attrib_meta_class_and_pos<I>
					::position new_pos_type;

				return meta_class::attributes::get(context, new_pos_type());

			}
			template <class a_class, int I>
			static typename result_of_get<I>::type
			get(a_class context, mpl::int_<I> pos, mpl::bool_<false>)
			{
				typedef typename own_attrib_meta_class_and_pos<I>
					::position new_pos_type;
				return meta_class::attributes::get(context, new_pos_type());
			}

		
			/** This function is used to query the member attributes 
		 	 *  from the base classes.
		 	 */
			template <class a_class, int I, typename dest_type>
			static dest_type&
			query(a_class context, mpl::int_<I> pos, dest_type& dest, mpl::bool_<true>)
			{
				typedef typename inherited_attrib_meta_class_and_pos<I>
					::meta_class meta_class;
				typedef typename inherited_attrib_meta_class_and_pos<I>
					::position new_pos_type;

				return meta_class::attributes::query(context, new_pos_type(), dest);

			}

			template <class a_class, int I, typename dest_type>
			static dest_type&
			query(a_class context, mpl::int_<I> pos, dest_type& dest, mpl::bool_<false>)
			{
				typedef typename own_attrib_meta_class_and_pos<I>
					::position new_pos_type;
				return meta_class::attributes::query(context, new_pos_type(), dest);
			}

		
			/** This function is used to query the member attributes 
		 	 *  from the base classes.
		 	 */
			template <class a_class, int I, typename value_type>
			static void
			set(a_class& context, mpl::int_<I> pos, value_type value, mpl::bool_<true>)
			{
				typedef typename inherited_attrib_meta_class_and_pos<I>
					::meta_class meta_class;
				typedef typename inherited_attrib_meta_class_and_pos<I>
					::position new_pos_type;

				meta_class::attributes::set(context, new_pos_type(), value);
			}

			template <class a_class, int I, typename value_type>
			static void
			set(a_class& context, mpl::int_<I> pos, value_type value, mpl::bool_<false>)
			{
				typedef typename own_attrib_meta_class_and_pos<I>
					::position new_pos_type;
				meta_class::attributes::set(context, new_pos_type(), value);
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
		
		/** Gets the name of the I-th member (including
		 *  the inherited ones)
		 */
		template <int I>
		static const bchar* 
		base_name(mpl::int_<I> pos)
		{
			typedef typename mpl::less<
				mpl::int_<I>,
				inherited_size
			>::type is_inherited;

			return detail::base_name(pos, is_inherited());
		}

		/** Gets the value of the I-th member (including 
		 *  the inherited ones)
		 */
		template <class a_class, int I>
		static typename detail::template result_of_get<I>::type
		get(a_class context, mpl::int_<I> pos)
		{
			typedef typename mpl::less<
				mpl::int_<I>,
				inherited_size
			>::type is_inherited;

			return detail::get(context, pos, is_inherited());
		}

		/** Queries the value of the I-th member (including 
		 *  the inherited ones)
		 */
		template <class a_class, int I, typename dest_type>
		static dest_type&
		query(a_class context, mpl::int_<I> pos, dest_type& dest)
		{
			typedef typename mpl::less<
				mpl::int_<I>,
				inherited_size
			>::type is_inherited;

			return detail::query(context, pos, dest, is_inherited());
		}

		/** Sets the value of the I-th member (including 
		 *  the inherited ones)
		 */
		template <class a_class, int I, typename value_type>
		static void
		set(a_class& context, mpl::int_<I> pos, value_type value)
		{
			typedef typename mpl::less<
				mpl::int_<I>,
				inherited_size
			>::type is_inherited;

			detail::set(context, pos, value, is_inherited());
		}
	}; // all_attributes



} // namespace mirror
} // namespace boost

#endif //include guard

