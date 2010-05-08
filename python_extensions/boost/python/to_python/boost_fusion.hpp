// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_TO_PYTHON_BOOST_FUSION_HPP
#define BOOST_PYTHON_TO_PYTHON_BOOST_FUSION_HPP

#include <boost/python.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace boost {
namespace python {

namespace detail {

struct boost_fusion_to_python_append {
    mutable boost::python::list sequence;
    template <typename T> void operator()(T const & x) const { sequence.append(x); }
};

} // namespace boost::python::detail

template <typename Sequence>
struct boost_fusion_to_python {

    static PyObject * convert(Sequence const & input) {
        detail::boost_fusion_to_python_append func;
        boost::fusion::for_each(input,func);
        return boost::python::incref(boost::python::tuple(func.sequence).ptr());
    }

    static PyTypeObject const * get_pytype() { return &PyTuple_Type; }

    static void declare() {
        boost::python::to_python_converter<Sequence,boost_fusion_to_python,true>();
    }

};

} // namespace boost::python
} // namespace boost

#endif // !BOOST_PYTHON_TO_PYTHON_BOOST_FUSION_HPP
