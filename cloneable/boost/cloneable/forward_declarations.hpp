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

#define BOOST_CLONEABLE_SCOPE_EXIT(locals)	\
		this_type *self = this;			\
		BOOST_SCOPE_EXIT(locals(self))

#define BOOST_CLONEABLE_SCOPE_EXIT_END \
		BOOST_SCOPE_EXIT_END

namespace boost
{

	namespace cloneable
	{
		typedef std::allocator<char> default_allocator;

		/// an abstract interface for an allocator that can allocate and de-allocate
		/// byte sequences
		struct abstract_allocator;

		/// empty structure with a virtual destructor used if the user does not
		/// wish to use a custom base type
		struct default_base_type;

		struct default_construction : mpl::true_ {};
		struct no_default_construction : mpl::false_ {};
		struct unknown_construction {};

		/// provides a set of pure-virtual methods for allocation, de-allocation, and cloning
		template <class Base = default_base_type>
		struct abstract_base;

		struct is_cloneable_tag { };

		template <class Derived, class Base = default_base_type>
		struct is_derived ;

		/// a structure derived from this, with type Derived, is correctly
		/// cloneable from a base pointer, given an abstract_allocator.
		template <
			class Derived
			, class Base = default_base_type
			, class Ctor = default_construction>
		struct base;

		/// an adaptor for an existing class.
		///
		/// this is a type that can be used correctly in an homogenous container,
		/// of effective type T, where T does not inherit from heterogenous::base.
		template <
			class T
			, class Base = default_base_type
			>//, class AbstractBase = abstract_cloneable<Base> >
		struct adaptor;

		// TODO: move to boost/heterogenous/foward
		/// a heterogenous vector of objects
		template 
		<
			class Base = default_base_type
			, class Alloc = monotonic::allocator<int>
		>
		struct vector;

		/// a heterogenous list of objects
		template 
		<
			class Base = default_base_type
			, class Alloc = monotonic::allocator<int>
		>
		struct list;
		
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

		/// a mapping of heterogenous objects to heterogenous objects
		template 
		<
			class Base = default_base_type
			, class Pred = indirect<std::less<Base> >
			, class Alloc = monotonic::allocator<int>
		>
		struct map;

		/// a set of heterogenous objects
		template 
		<
			class Base = default_base_type
			, class Pred = std::less<Base>
			, class Alloc = monotonic::allocator<int>
		>
		struct set;

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_FORWARD_DECLARATIONS_HPP

//EOF
