// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_OBJECT_HPP
#define BOOST_OBJECT_MODEL_OBJECT_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/generic/object.hpp>
#include <boost/object_model/dereference.hpp>

BOOST_OM_BEGIN

/// an object with a specific type
template <class T>
struct object : generic::object
{
	typedef type::traits<T> traits;

	object() {}

	object(const generic::object &obj) : generic::object(obj)
	{
	}

	typename traits::reference_type reference()
	{
		return deref<T>(*this);
	}

	typename traits::reference_type operator*()
	{
		return reference();
	}
};

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_OBJECT_HPP

//EOF
