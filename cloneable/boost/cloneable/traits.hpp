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
				BOOST_STATIC_CONSTANT(bool, is_cloneable = false);
				BOOST_STATIC_CONSTANT(bool, has_default_ctor = false);	// this really should be ternary: yes, no or unknown
				typedef void construction_tag_type;
			};

			template <class T>
			struct traits<T, true>
			{
				BOOST_STATIC_CONSTANT(bool, is_cloneable = true);
				typedef is_convertible<T *, default_construction *> has_default_ctor_type;
				typedef is_convertible<T *, no_default_construction *> has_no_default_ctor_type;
				// T is default constructable only if it is convertibel to def_constr and 
				// it is also not convertible to no_def_constr. This ensures that a type
				// that inherits from a type that is not default constructible, is also not
				// default constructible no matter what.
				BOOST_STATIC_CONSTANT(bool, has_default_ctor = has_default_ctor_type::value || !has_no_default_ctor_type::value);
				typedef typename mpl::if_<mpl::bool_<has_default_ctor>
						, default_construction
						, no_default_construction>
					::type construction_tag;
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
