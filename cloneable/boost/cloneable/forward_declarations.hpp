// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_FORWARD_DECLARATIONS_HPP
#define BOOST_CLONEABLE_FORWARD_DECLARATIONS_HPP

#include <functional>
#include <memory>
#include <boost/scope_exit.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/pointee.hpp>
#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/exception.hpp>

namespace boost
{
	namespace cloneable
	{
		/// allocator to use for clone or create operations when no allocator is provided
		typedef std::allocator<char> default_allocator;

		/// an abstract interface for an allocator that can allocate and de-allocate
		/// byte sequences
		struct abstract_allocator;

		/// empty structure with a virtual destructor used if the user does not
		/// wish to use a custom base type
		struct base_type;

		namespace detail { struct ctag { }; }

		/// {@ tags to inform about default-constructability
		struct default_construction_tag : detail::ctag, mpl::true_ {};
		struct no_default_construction_tag : detail::ctag, mpl::false_ {};
		struct unknown_construction_tag : detail::ctag {};
		/// @}

		/// provides a set of pure-virtual methods for allocation, de-allocation, and cloning
		template <class Base = base_type>
		struct abstract_base;

		struct is_cloneable_tag { };

		struct nil { };

		/// a structure derived from this, with type Derived, is correctly
		/// cloneable from a base pointer, given an abstract_allocator.
		/// TODO: allow Ctor and Base to be given in any order
		template <
			class Derived
			, class Base = base_type
			, class DefaultConstructionTag = default_construction_tag
			//, class Base = nil//base_type
			//, class DefaultConstructionTag = nil//default_construction
		>
		struct base;

		/// an adaptor for an existing class.
		///
		/// this is a type that can be used correctly in an homogenous container,
		/// of effective type T, where T does not inherit from heterogenous::base.
		template <
			class T
			, class Base = base_type>
		struct adaptor;

		// TODO: move to boost/heterogenous/foward
		/// a heterogenous vector of objects
		template 
		<
			class Base = base_type
			, class Alloc = monotonic::allocator<int>
		>
		struct vector;

		/// a heterogenous list of objects
		template 
		<
			class Base = base_type
			, class Alloc = monotonic::allocator<int>
		>
		struct list;
		
		// TODO: use ptr_container::detail::indirect_fun<>
		template <class Fun>
		struct indirect
		{
			Fun fun;

			indirect() { }
			indirect(Fun F) : fun(F) { }

			template <class Left, class Right>
			//typename result_of< Fun(typename pointee<Left>::type, typename pointee<Right>::type) >::type
			bool
			operator()(const Left &left, const Right &right) const
			{
				return fun(*left, *right);
			}
		};

		/// a mapping of Key to heterogenous object
		template 
		<
			class Key
			, class Base = base_type
			, class Pred = std::less<Key>
			, class Alloc = monotonic::allocator<int>
		>
		struct map;

		/// a mapping of heterogenous object to heterogenous object
		template 
		<
			class Base = base_type
			, class Pred = indirect<std::less<Base> >
			, class Alloc = monotonic::allocator<int>
		>
		struct heterogenous_map;

		/// a set of heterogenous objects
		template 
		<
			class Base = base_type
			, class Pred = std::less<Base>
			, class Alloc = monotonic::allocator<int>
		>
		struct set;

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_FORWARD_DECLARATIONS_HPP

//EOF
