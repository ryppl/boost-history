// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_BASE_HPP
#define BOOST_CLONEABLE_BASE_HPP

#include <boost/aligned_storage.hpp>
#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/base_type.hpp>
#include <boost/cloneable/abstract_base.hpp>
#include <boost/cloneable/detail/mixin.hpp>
#include <boost/cloneable/detail/create_new.hpp>

namespace boost
{
	namespace cloneable
	{
		namespace impl
		{
			using mpl::bool_;
			using mpl::if_;
			using mpl::or_;

			template <class Derived, class Base, class DefaultCtorTag>
			struct base 
				: detail::mixin<Derived, Base>
				, virtual DefaultCtorTag
			{
				typedef Derived derived_type;
				typedef Base base_type;
				typedef DefaultCtorTag default_constructable_type;
				typedef base<derived_type, base_type, default_constructable_type> this_type;

				virtual this_type *create_new(abstract_allocator &alloc) const 
				{
					return detail::create_new<Derived, DefaultCtorTag>::given(this, alloc, alignment);
				}
			};

			template <class T>
			struct is_nil : is_same<T,nil> { };

			template <class D, class A, class B>
			struct make_base
			{
				template <class T>
				struct get_base_type : if_<is_nil<T>, base_type, T> { };

				template <class T>
				struct get_default_construction_tag : if_<is_nil<T>, default_construction, T> { };

				typedef is_convertible<A *, detail::ctag *> a_is_tag;
				typedef is_convertible<B *, detail::ctag *> b_is_tag;

				typedef typename if_<
					is_nil<A>
						, base<D, typename get_base_type<A>::type, typename get_default_construction_tag<B>::type>
						, base<D, typename get_base_type<B>::type, typename get_default_construction_tag<A>::type>
				>::type type1;

				typedef base<D, typename get_base_type<A>::type, typename get_default_construction_tag<B>::type> type;
			};
		}

		/// base for the given derived type, using the given base class and default construction tag
		template <class D, class A, class B>
		struct base : impl::make_base<D,A,B>::type 
		{ 
			typedef typename impl::make_base<D,A,B>::type parent_type;
			typedef base<D,A,B> this_type;
			typedef typename parent_type::base_type base_type;

			BOOST_STATIC_ASSERT(is_cloneable<this_type>::value);
		};

		/*
		template <class Derived, class A, class B>
		struct base : impl::base<Derived,A,B>
		{
		};
		*/
	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_BASE_HPP

//EOF
