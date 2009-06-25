// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_DICTIONARY_HPP
#define BOOST_OBJECT_MODEL_DICTIONARY_HPP

#include <map>
#include <boost/object_model/detail/prefix.hpp>

BOOST_OM_BEGIN

struct dictionary
{
	typedef std::map<label, generic::object> contents_type;

	contents_type contents;
};

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_DICTIONARY_HPP

//EOF
