// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_TYPE_SPECIFIER_HPP
#define BOOST_OBJECT_MODEL_TYPE_SPECIFIER_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/string.hpp>
#include <boost/object_model/type/modifiers.hpp>

BOOST_OM_BEGIN

namespace type
{
	struct specifier : modifiers
	{
		number type_number;

		specifier(number N = number::None, modifiers M = modifiers()) : modifiers(M), type_number(N) { }

		number get_number() const { return type_number; }

		//template <class Alloc, template <class Al = Alloc, class Ch = char, class Tr = std::char_traits<Ch> > String = string>
		//bool to_string(const registry<Alloc> &, string<Alloc> ) const;
	};

	template <class T>
	struct is_const : mpl::true_ { };

	template <class T>
	struct is_const<const T> : mpl::true_ { };

	template <class T>
	struct is_const<const T&> : mpl::true_ { };

	template <class T>
	specifier make_specifier()
	{
		modifiers mods;
		mods.set_const(is_const<T>::value);	// what is wrong with boost::is_const<T> ??
		mods.set_reference(boost::is_reference<T>::value);
		return specifier(traits<typename base_type<T>::Type>::type_number, mods);
	};

} // namespace type

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_TYPE_SPECIFIER_HPP

//EOF
