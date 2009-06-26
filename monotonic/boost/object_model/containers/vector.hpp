// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_CONTAINERS_VECTOR_HPP
#define BOOST_OBJECT_MODEL_CONTAINERS_VECTOR_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/generic/object.hpp>

BOOST_OM_BEGIN

namespace containers
{
	template <class Alloc>
	struct vector
	{
		//typedef boost::containers::vector<object, Alloc> implementation;
		typedef std::vector<object, Alloc> implementation;
		typedef typename implementation::value_type value_type;
		typedef typename implementation::iterator iterator;
		typedef typename implementation::const_iterator const_iterator;

	private:
		implementation impl;

	public:
	};

}

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_CONTAINERS_VECTOR_HPP

//EOF
