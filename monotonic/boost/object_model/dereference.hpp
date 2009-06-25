// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_DEREFERENCE_HPP
#define BOOST_OBJECT_MODEL_DEREFERENCE_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/generic/object.hpp>

BOOST_OM_BEGIN

template <class T>
typename type::traits<T>::reference deref(generic::object &object)
{
	throw;
}

template <class T>
typename type::traits<T>::const_reference const_deref(generic::object const &object)
{
	throw;
}

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_DEREFERENCE_HPP

//EOF
