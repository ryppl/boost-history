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
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/remove_if.hpp>
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
			 *  order as registered.
			 */
			typedef typename mpl::remove_if<
				typename base_classes::list,
				mpl::lambda<
					mpl::not_<reflects_virtual_inheritance<mpl::_1> >
				>::type
			>::type list_of_regular_base_classes;

			/** The list of all virtual base classes with duplicates.
			 */
			typedef typename mpl::remove_if<
				typename base_classes::list,
				mpl::lambda<
					reflects_virtual_inheritance<mpl::_1>
				>::type
			>::type list_of_all_virtual_base_classes;

			/** The list of virtual base classes with duplicates
			 *  removed.
			 * TODO: 
			 */
			typedef typename list_of_all_virtual_base_classes
				list_of_virtual_base_classes;
			
			/** This tells whether the virtual base classes
			 *  go first or last into the list of base classes.
			 */
			typedef true_type virtual_first;

			/** The list of base classes of the reflected_class.
			 */
			typedef typename mpl::joint_view<
				typename mpl::if_<
					virtual_first,
					list_of_virtual_base_classes,
					list_of_regular_base_classes
				>::type,
				typename mpl::if_<
					virtual_first,
					list_of_regular_base_classes,
					list_of_virtual_base_classes
				>::type
			>::type list_of_base_classes;

			/** This template gets the list of attributes types
			 *  when given a meta_inheritance specialization.
			 */
			template <class meta_inheritance>
			struct get_bc_all_attrib_tl
			{
				typedef typename meta_inheritance::meta_class::all_attributes::type_list type;
			};
			
			/** Definition of a list containing the lists
			 *  of all_attributes from the base classes.
			 */
			typedef typename mpl::accumulate<
				typename list_of_base_classes,
				mpl::vector<>,
				mpl::push_back<
					mpl::_1, 
					mpl::lambda<get_bc_all_attrib_tl<mpl::_2> >
				>
			>::type att_type_lists_of_base_classes;
			
			/** Defintion of a mpl::vector containing the member
			 *  attributes inherited from the base classes.
			 */
			typedef typename mpl::accumulate<
				att_type_lists_of_base_classes,
				mpl::vector<>,
				mpl::insert_range<
					mpl::_1,
					mpl::end<mpl::_1>,
					mpl::_2
				>
			>::type inherited_attrib_type_list;
		}; // struct detail
		
		/** The list of types of all attributes including
		 *  the inherited ones.
		 */
		typedef typename mpl::joint_view<
			typename detail::inherited_attrib_type_list,
			typename meta_class<reflected_class, variant_tag>::attributes::type_list
		>::type type_list;
		
		/** The size of the type_list, i.e. the count of all attributes
		 */
		struct size : public mpl::size<type_list> { };
	};

};


/** This macro should be included in the definition of every class
 *  with private or protected members, that should be refleccted
 */
#define BOOST_MIRROR_FRIENDLY_CLASS(CLASS_NAME) \
		friend struct ::boost::mirror::meta_class_attributes<CLASS_NAME>;



} // namespace mirror
} // namespace boost

#endif //include guard

