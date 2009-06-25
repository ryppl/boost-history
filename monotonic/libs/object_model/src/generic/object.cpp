// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#include <boost/object_model/generic/object.hpp>

BOOST_OM_BEGIN

namespace generic
{
	object_base::object_base() 
		: reg(0), type(0), number(0)
	{
	}

	object_base::object_base(const object_base &other)
		: reg(other.reg), type(other.type), number(other.number)
	{
	}

	void object_base::construct(registry &r, klass const &k, handle h)
	{
		reg = &r;
		type = &k;
		number = h;
	}

	const_object::const_object()
	{
	}

	object::object()
	{
	}

	object::object(const storage &store)
	{
	}

}

const generic::object null_object;

BOOST_OM_END

//EOF
