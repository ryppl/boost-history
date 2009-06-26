// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_FORWARD_DECLARATIONS_HPP
#define BOOST_OBJECT_MODEL_FORWARD_DECLARATIONS_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/exceptions.hpp>

BOOST_OM_BEGIN

namespace type
{
	/// traits for a given type
	template <class T>
	struct traits;

	/// a type number
	struct number;
}

/// a unique identifier of an object
struct handle;

/// a name for an object
template <class Text>
struct label;

/// a mapping of label to object
template <class Label>
struct dictionary;

/// generic bases for type-specific derived types
namespace generic
{
	struct base;

	struct object_base;

	struct const_object;

	struct mutable_object;

	struct object;

	/// common for all const storage for an instance
	template <class Traits>
	struct const_storage;

	/// common for all storage for an instance
	template <class Traits>
	struct storage;

	/// common for types that have a `self` pointer
	struct reflected;

	/// common for a all methods
	struct method;

	/// common for all class properties
	struct property;

	/// common for all classes
	//template <class Label>
	struct klass;

	//template <class Label>
	struct registry;
}

template <class T>
struct const_object;

template <class T>
struct const_reference;

template <class T>
struct const_pointer;

template <class T>
struct mutable_object;

template <class T>
struct mutable_reference;

template <class T>
struct mutable_pointer;

template <class T>
struct object;

template <class T>
struct pointer;

template <class T>
struct reference;

/// storage for a specific type
template <class T, class Traits>
struct storage;

/// const storage for a specific type
template <class T, class Traits>
struct const_storage;

template <class T>
struct reflected;

template <class Al = default_allocator, class Ch = char, class Tr = std::char_traits<Ch> >
struct string;

template <class Str = string<> >
struct label;

/// a specific method of class type, with given signature
template <class T, class Signature>
struct method;

/// a specific property
template <class T, class Val>
struct property;

/// class of a given type
template <class T, class Traits>
struct klass;

template <class T, class Traits>
struct builder;

///// an object registry (factory)
//template <class Al, class Traits>
//struct registry;

/// a sequence of executable objects
struct continuation;

/// executes continuations
struct exectutor;

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_FORWARD_DECLARATIONS_HPP

//EOF
