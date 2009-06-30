// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_FORWARD_DECLARATIONS_HPP
#define BOOST_HETEROGENOUS_FORWARD_DECLARATIONS_HPP

#include <functional>
#include <boost/monotonic/allocator.hpp>
#include <boost/heterogenous/detail/prefix.hpp>

namespace boost
{
	namespace heterogenous
	{
		struct abstract_allocator;

		struct default_base_type;

		template <class Base>
		struct abstract_base;

		template <
			class Derived
			, class Base = default_base_type
			, class AbstractBase = abstract_base<Base> >
		struct base;

		template <
			class Base = default_base_type
			, class Alloc = monotonic::allocator<int>
			, class AbstractBase = abstract_base<Base> >
		struct vector;

		template <
			class Base = default_base_type
			, class Pred = std::less<Base>
			, class Alloc = monotonic::allocator<int>
			, class AbstractBase = abstract_base<Base> >
		struct map;

	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_FORWARD_DECLARATIONS_HPP

//EOF
