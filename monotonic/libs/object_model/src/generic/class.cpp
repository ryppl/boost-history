// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

//#include <boost/object_model/generic/object.hpp>
#include <boost/object_model/generic/class.hpp>

BOOST_OM_BEGIN

namespace generic
{
	klass::klass(const label &ident, type::number num)
		: name(ident), type_number(num)
	{
	}

	bool klass::has_method(label const &name) const
	{
		return methods.find(name) != methods.end();
	}

	bool klass::has_field(label const &name) const
	{
		return properties.find(name) != properties.end();
	}

}

BOOST_OM_END

//EOF


