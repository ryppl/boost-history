// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_TRAITS_HPP
#define BOOST_CLONEABLE_TRAITS_HPP

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/cloneable/detail/prefix.hpp>

namespace boost
{
	namespace cloneable
	{
		namespace impl
		{
			template <class T, bool>
			struct traits
			{
				typedef T derived_type;
				typedef T base_type;
				typedef unknown_construction default_constructable_type;
				typedef T abstract_base_type;

				BOOST_STATIC_CONSTANT(bool, is_cloneable = false);
				BOOST_STATIC_CONSTANT(bool, has_default_ctor = false);	// this really should be ternary: yes, no or unknown
			};

			template <class T>
			struct traits<T, true>
			{
				typedef typename T::derived_type derived_type;
				typedef typename T::base_type base_type;
				typedef typename T::default_constructable_type default_constructable_type;
				typedef typename T::abstract_base_type abstract_base_type;

				BOOST_STATIC_CONSTANT(bool, is_cloneable = true);
				typedef is_same<default_constructable_type, default_construction> same_type;
				BOOST_STATIC_CONSTANT(bool, has_default_ctor = same_type::value);
			};

			template <class T>
			struct get_traits : traits<T, is_convertible<T *, is_cloneable_tag *>::value> { };

		} // namespace impl

		template <class T>
		struct traits : impl::get_traits<T> { };

		template <class T>
		struct is_cloneable : mpl::bool_<traits<T>::is_cloneable> { };

		template <class T>
		struct is_default_constructable : mpl::bool_<traits<T>::has_default_ctor> { };

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_TRAITS_HPP

//EOF
