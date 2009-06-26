// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_SYSTEM_TRAITS_HPP
#define BOOST_OBJECT_MODEL_SYSTEM_TRAITS_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/string.hpp>
#include <boost/object_model/label.hpp>

BOOST_OM_BEGIN

template <class Ch = char, class Al = std::allocator<Ch> >
struct system_traits
{
	typedef Ch char_type;
	typedef std::char_traits<char_type> char_traits;
	typedef Al allocator_type;
	typedef string<allocator_type, char_type, char_traits> string_type;
	typedef label<string_type> label_type;
	typedef system_traits<Ch,Al> this_type;

	template <class T>
	struct rebind_class
	{
		typedef klass<T, this_type> type;
	};
	template <class T>
	struct rebind_allocator
	{
		typedef typename allocator_type::template rebind<T>::other type;
	};
};

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_SYSTEM_TRAITS_HPP

//EOF
