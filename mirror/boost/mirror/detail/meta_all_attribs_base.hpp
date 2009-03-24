/**
 * \file boost/mirror/detail/meta_all_attribs_base.hpp
 * Registering and reflection of class attributes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_DETAIL_META_ALL_ATTRIBS_BASE_HPP
#define BOOST_MIRROR_DETAIL_META_ALL_ATTRIBS_BASE_HPP

#include <boost/mirror/access_spec.hpp>
#include <boost/mirror/inherit_spec.hpp>
// macros for declaration of simple meta attributes
#include <boost/mirror/detail/meta_attribs_base.hpp>
//
#include <boost/char_type_switch/string.hpp>
//
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/less.hpp>

namespace boost {
namespace mirror {
namespace detail {

/* contains the basic inheritance information from 
 * a meta_inheritance.
 */
template <
	class InheritanceSpecifier, 
	class MetaClass
> struct base_class_inheritance_info
{
	typedef InheritanceSpecifier inheritance;
	typedef MetaClass base_class;
};


/** This is basically the same as the "attributes" structure
 *  but allows to work with all member attributes including
 *  the inherited ones. 
 */
template <
	class ReflectedType, 
	class VariantTag
>
struct meta_class_all_attributes_base
{
	// the scope of the MetaAttributeSequence
	typedef boost::mirror::meta_class<ReflectedType, VariantTag> 
		scope;

	/** This struct "hides" the internal helpers. Anything inside
	 *  is implementation detail and can change without any
	 *  notice.
	 */
	struct helpers 
	{
		/** Tells whether a base class is inherited virtually
		 */
		template <class BaseInheritanceInfo>
		struct is_inherited_virtually
		 : public is_same<
			typename BaseInheritanceInfo::inheritance,
			virtual_base_
		>{ };

		/* extracts the basic inheritance information from 
		 * a meta_inheritance.
		 */
		template <typename MetaInheritance>
		struct get_base_info_from_meta_inheritance
		{
			typedef ::boost::mirror::detail::base_class_inheritance_info<
				typename MetaInheritance::inheritance,
				typename MetaInheritance::base_class
			> type;
		};

		/** Accumulate the basic inheritance information 
		 *  about the base classes of this class
		 */
		typedef typename mpl::accumulate<
			typename scope::base_classes::list,
			mpl::vector0<>,
			mpl::push_back<
				mpl::_1,
				get_base_info_from_meta_inheritance<
					mpl::_2
				>
			>
		>::type list_of_base_class_infos;

		/** The list of non-virtual base classes in the same
		 *  order as they were registered.
		 */
		typedef typename mpl::remove_if<
			list_of_base_class_infos,
			is_inherited_virtually<mpl::_1>
		>::type list_of_regular_base_classes;

		/** The list of directly inherited virtual base classes.
		 */
		typedef typename mpl::remove_if<
			list_of_base_class_infos,
			mpl::not_<is_inherited_virtually<mpl::_1> >
		>::type list_of_virtual_base_classes;

		/** This template gets the regular_base_class_layout
		 *  of a base class when given a meta_inheritance 
		 *  specialization for this base class.
		 */
		template <class MetaInheritance>
		struct get_base_class_regular_layout
		{
			typedef MetaInheritance meta_inheritance;
			typedef typename 
				meta_inheritance:: 
				base_class:: 
				all_attributes:: 
				helpers:: 
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
		template <class MetaInheritance>
		struct get_base_class_virtual_layout
		{
			typedef MetaInheritance meta_inheritance;
			typedef typename 
				meta_inheritance:: 
				base_class:: 
				all_attributes:: 
				helpers:: 
				virtual_base_class_layout type;
		};

		/** This template gets the base_class_layout
		 *  of a base class when given a meta_inheritance 
		 *  specialization for this base class.
		 */
		template <class MetaInheritance>
		struct get_base_class_layout
		{
			typedef MetaInheritance meta_inheritance;
			typedef typename 
				meta_inheritance:: 
				base_class:: 
				all_attributes:: 
				helpers:: 
				base_class_layout type;
		};

		/** The list of vitual base classes of this class.
		 *  This list still contains dupplicates, that are
		 *  removed to form virtual_base_class_layout
		 */
		typedef typename mpl::accumulate<
			list_of_base_class_infos,
			mpl::vector0<>,
			mpl::if_<
				is_inherited_virtually<
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
		 *  given a meta_inheritance specialization
		 */
		template <class MetaInheritance>
		struct get_base_class_attrib_type_list
		{
			typedef MetaInheritance meta_inheritance;
			typedef typename meta_inheritance::
					base_class::
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
			typename helpers::inherited_member_attrib_type_list,
			typename scope::attributes::type_list
		>::type member_attrib_type_list;


		/** This template gets the list of the owner classes 
		 *  for the inherited attributes.
		 */
		template <class CurrentList, class MetaInheritance>
		struct get_base_class_attrib_owner_and_offs
		{
			typedef CurrentList current_list;
			typedef MetaInheritance meta_inheritance;

			typedef typename meta_inheritance::
					base_class base_class;

			typedef typename base_class::
					attributes::
					type_list type_list;

			typedef typename mpl::size<
					current_list
			>::type offset;
			typedef typename mpl::pair<
					base_class,
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

		/** This template is used to query the return value
		 *  type of the getter for the I-th member attribute
		 */
		template <int I>
		struct type_of_address
		{
			typedef typename mpl::at<
					member_attrib_type_list, 
					mpl::int_<I> 
				>::type* type;
		};

		template <int I>
		struct inherited_attrib_meta_class_and_pos
		{
			typedef typename mpl::at<
				typename helpers::inherited_attrib_owners_and_offsets,
				mpl::int_<I>
			>::type owner_and_offset;

			typedef typename owner_and_offset::first ancestor;
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
		template <int I, typename CharT>
		static inline const ::std::basic_string<CharT>& get_name(
			mpl::int_<I> pos, 
			mpl::false_ full_name,
			::std::char_traits<CharT> cht,
			mpl::bool_<true> is_inherited
		)
		{
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::ancestor ancestor;
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::position new_position;

			return ancestor::attributes::get_name(
				new_position(),
				full_name,
				cht
			);
		}

		template <int I, typename CharT>
		static inline const ::std::basic_string<CharT>& get_name(
			mpl::int_<I> pos, 
			mpl::false_ full_name,
			::std::char_traits<CharT> cht,
			mpl::bool_<false> is_inherited
		)
		{
			typedef typename own_attrib_meta_class_and_pos<I>
				::position new_position;

			return scope::attributes::get_name(
				new_position(),
				full_name,
				cht
			);
		}

	
		/** This function is used to point to the member attributes
		 *  from the base classes.
		 */
		template <class Class, int I>
		static inline typename type_of_address<I>::type
		address(Class& instance, mpl::int_<I> pos, mpl::bool_<true>)
		{
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::ancestor ancestor;
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::position new_position;

			return ancestor::attributes::address(instance, new_position());

		}
		template <class Class, int I>
		static inline typename type_of_address<I>::type
		address(Class& instance, mpl::int_<I> pos, mpl::bool_<false>)
		{
			typedef typename own_attrib_meta_class_and_pos<I>
				::position new_position;
			return scope::attributes::address(instance, new_position());
		}

		/** This function is used to get the member attribute 
		 *  from the base classes.
		 */
		template <class Class, int I>
		static inline typename result_of_get<I>::type
		get(const Class& instance, mpl::int_<I> pos, mpl::bool_<true>)
		{
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::ancestor ancestor;
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::position new_position;

			return ancestor::attributes::get(instance, new_position());

		}
		template <class Class, int I>
		static inline typename result_of_get<I>::type
		get(const Class& instance, mpl::int_<I> pos, mpl::bool_<false>)
		{
			typedef typename own_attrib_meta_class_and_pos<I>
				::position new_position;
			return scope::attributes::get(instance, new_position());
		}

	
		/** This function is used to query the member attributes 
		 *  from the base classes.
		 */
		template <class Class, int I, typename DestType>
		static inline DestType&
		query(Class instance, mpl::int_<I> pos, DestType& dest, mpl::bool_<true>)
		{
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::ancestor ancestor;
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::position new_position;

			return ancestor::attributes::query(instance, new_position(), dest);

		}

		template <class Class, int I, typename DestType>
		static inline DestType&
		query(Class instance, mpl::int_<I> pos, DestType& dest, mpl::bool_<false>)
		{
			typedef typename own_attrib_meta_class_and_pos<I>
				::position new_position;
			return scope::attributes::query(instance, new_position(), dest);
		}

	
		/** This function is used to query the member attributes 
		 *  from the base classes.
		 */
		template <class Class, int I, typename ValueType>
		static inline void
		set(Class& instance, mpl::int_<I> pos, ValueType value, mpl::bool_<true>)
		{
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::ancestor ancestor;
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::position new_position;

			ancestor::attributes::set(instance, new_position(), value);
		}

		template <class Class, int I, typename ValueType>
		static inline void
		set(Class& instance, mpl::int_<I> pos, ValueType value, mpl::bool_<false>)
		{
			typedef typename own_attrib_meta_class_and_pos<I>
				::position new_position;
			scope::attributes::set(instance, new_position(), value);
		}

		template <int I>
		struct inherited_attrib_meta_class
		{
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::ancestor type;
		};

		struct own_attrib_meta_class
		{
			typedef scope type;
		};

		template <int I>
		struct inherited_attrib_position
		{
			typedef typename inherited_attrib_meta_class_and_pos<I>
				::position type;
		};

		template <int I>
		struct own_attrib_position
		{
			typedef typename own_attrib_meta_class_and_pos<I>
				::position type;
		};

		template <int I, class Inherited>
		struct result_of_get_traits
		{
			// get the right meta-class
			typedef typename mpl::eval_if<
				Inherited,
				inherited_attrib_meta_class<I>,
				own_attrib_meta_class
			>::type ancestor;

			// get the position inside of the meta-class' attributes
			typedef typename mpl::eval_if<
				Inherited,
				inherited_attrib_position<I>,
				own_attrib_position<I>
			>::type position;


			// get the return value type of the get_traits function
			BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
				detail, 
				ancestor::attributes::get_traits(position())
			)

			// the traits of the i-th attribute
			typedef typename detail::type type;

		}; // struct result_of_get_traits

		template <
			int I, 
			class Inherited, 
			class MetaClassAttributes,
			template <class, class, class, class> class MetaFunction
		> struct result_of_get_generator_plugin
		{
			// get the right meta-class
			typedef typename mpl::eval_if<
				Inherited,
				inherited_attrib_meta_class<I>,
				own_attrib_meta_class
			>::type ancestor;

			// get the position inside of the meta-class' attributes
			typedef typename mpl::eval_if<
				Inherited,
				inherited_attrib_position<I>,
				own_attrib_position<I>
			>::type position;


			// get the return value type of the get_traits function
			BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
				detail, 
				(ancestor::attributes::template get_generator_plugin<
					MetaClassAttributes,
					MetaFunction
				>(position()))
			)

			// the traits of the i-th attribute
			typedef typename detail::type type;

		}; // struct result_of_get_meta_attrib_holder

	}; // struct detail
	/** The list of inherited attribute types
	 */
	typedef typename helpers::inherited_member_attrib_type_list 
		inherited_type_list;

	/** The size of the type_list, i.e. the count of inherited attributes
	 */
	struct inherited_size : public mpl::size<inherited_type_list> { };

	/** The list of types of all attributes including
	 *  the inherited ones.
	 *  WARNING: this is an implementation detail which
	 *  can change without any notice.
	 */
	typedef typename helpers::member_attrib_type_list type_list;
	
	/** The size of the type_list, i.e. the count of all attributes
	 *  WARNING: this is an implementation detail which
	 *  can change without any notice.
	 */
	struct size : public mpl::size<type_list> { };

private:	
	template <int I>
	struct result_of_is_inherited
	{
		typedef typename mpl::less<
			mpl::int_<I>,
			inherited_size
		>::type type;
	};

	/** Returns mpl::bool_<true> if the i-th attrib is inherited
	 *  returns mpl::bool_<false> otherwise
	 */
	template <int I>
	static typename result_of_is_inherited<I>::type 
	is_inherited(mpl::int_<I>)
	{
		return typename result_of_is_inherited<I>::type();
	}
public:

	/** Gets the name of the I-th member (including
	 *  the inherited ones)
	 */
	template <int I, typename CharT>
	static inline const ::std::basic_string<CharT>& get_name(
		mpl::int_<I> pos, 
		mpl::false_ full_name,
		::std::char_traits<CharT> cht
	)
	{
		return helpers::get_name(
			pos, 
			full_name,
			cht,
			is_inherited(pos)
		);
	}

	/** Gets the address of the I-th member (including 
	 *  the inherited ones)
	 */
	template <class Class, int I>
	static inline typename helpers::template type_of_address<I>::type
	address(Class& instance, mpl::int_<I> pos)
	{
		return helpers::address(
			instance, 
			pos, 
			is_inherited(pos)
		);
	}

	/** Gets the value of the I-th member (including 
	 *  the inherited ones)
	 */
	template <class Class, int I>
	static inline typename helpers::template result_of_get<I>::type
	get(const Class& instance, mpl::int_<I> pos)
	{
		return helpers::get(
			instance, 
			pos, 
			is_inherited(pos)
		);
	}

	/** Queries the value of the I-th member (including 
	 *  the inherited ones)
	 */
	template <class Class, int I, typename DestType>
	static inline DestType&
	query(Class instance, mpl::int_<I> pos, DestType& dest)
	{
		return helpers::query(
			instance, 
			pos, 
			dest, 
			is_inherited(pos)
		);
	}

	/** Sets the value of the I-th member (including 
	 *  the inherited ones)
	 */
	template <class Class, int I, typename ValueType>
	static inline void
	set(Class& instance, mpl::int_<I> pos, ValueType value)
	{
		helpers::set(
			instance, 
			pos, 
			value, 
			is_inherited(pos)
		);
	}

	/** The attribute traits getter
	 */
	template <int I>
	static typename helpers:: template result_of_get_traits<
		I,
		typename result_of_is_inherited<I>::type
	>::type get_traits(mpl::int_<I>);

	/** The meta-attribute generator plugin getter
	 */
	template <
		class MetaClassAttributes, 
		template <class, class, class, class> class MetaFunction,
		int I
	> static typename helpers:: template result_of_get_generator_plugin<
		I,
		typename result_of_is_inherited<I>::type,
		MetaClassAttributes,
		MetaFunction
	>::type get_generator_plugin(mpl::int_<I>);

}; // all_attributes_base

} // namespace detail 
} // namespace mirror
} // namespace boost

#endif //include guard

