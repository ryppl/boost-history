// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_BUILDER_HPP
#define BOOST_OBJECT_MODEL_BUILDER_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/class.hpp>

BOOST_OM_BEGIN

template <class T, class Allocator>
struct builder
{
	typedef type::traits<T> traits;

private:
	registry<Allocator> &reg;
	klass<T> *my_klass;

public:
	builder(registry<Allocator> &R) : reg(R)
	{
		my_klass = reg.register_class<T>();
	}
	struct methods_type
	{
		struct fields_type
		{
			template <class Field>
			fields_type &operator()(const char *name, Field field)
			{
				return *this;
			}
		} fields;

		template <class Method>
		methods_type &operator()(const char *name, Method method)
		{
			return *this;
		}
		
	} methods;
};

BOOST_OM_END

#include <boost/object_model/detail/prefix.hpp>

#endif // BOOST_OBJECT_MODEL_BUILDER_HPP

//EOF
