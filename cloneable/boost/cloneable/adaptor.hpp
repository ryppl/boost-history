// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_ADAPTOR_HPP
#define BOOST_CLONEABLE_ADAPTOR_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/base.hpp>

namespace boost
{
	namespace cloneable
	{
		/// an adaptor for an existing class T, making it cloneable
		template <class T, class Base>
		struct adaptor : T, base<adaptor<T, Base>, Base>
		{
			adaptor() { }

			template <class A0>
			adaptor(A0 a0) : T(a0)
			{
			}
			template <class A0, class A1>
			adaptor(A0 a0, A1 a1) : T(a0, a1)
			{
			}
			template <class A0, class A1, class A2>
			adaptor(A0 a0, A1 a1, A2 a2) : T(a0, a1, a2)
			{
			}
		};

		/// an adaptor for an existing class T, making it cloneable
		template <class T, class D, class Base>
		struct adaptor_base : T, base<adaptor<T, Base>, Base>
		{
			adaptor_base() { }

			template <class A0>
			adaptor_base(A0 a0) : T(a0)
			{
			}
			template <class A0, class A1>
			adaptor_base(A0 a0, A1 a1) : T(a0, a1)
			{
			}
			template <class A0, class A1, class A2>
			adaptor_base(A0 a0, A1 a1, A2 a2) : T(a0, a1, a2)
			{
			}
		};
	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_ADAPTOR_HPP

//EOF
