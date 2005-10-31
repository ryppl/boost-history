// Copyright Eric Niebler 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <list>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/iterator.hpp>
#include <boost/python/stl_iterator.hpp>

using namespace boost::python;

typedef std::list<int> list_int;

void assign(list_int& x, object const& y)
{
    stl_input_iterator<int> begin(y), end;
    x.assign(begin, end);
}

BOOST_PYTHON_MODULE(stl_iterator_ext)
{
    using boost::python::iterator; // gcc 2.96 bug workaround

    class_<list_int>("list_int")
        .def("assign", assign)
        .def("__iter__", iterator<list_int>())
        ;
}

#include "module_tail.cpp"
