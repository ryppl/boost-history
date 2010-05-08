// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_TO_PYTHON_STD_AUTO_PTR_HPP
#define BOOST_PYTHON_TO_PYTHON_STD_AUTO_PTR_HPP

#include <boost/python.hpp>
#include <memory>

namespace boost {
namespace python {

template <typename U>
struct to_python_value< std::auto_ptr<U> > : detail::builtin_to_python {

    typename boost::python::copy_const_reference::apply<boost::shared_ptr<U> const &>::type _shared_converter;

    inline PyObject* operator()(std::auto_ptr<U> x) const {
        boost::shared_ptr<U> shared(x);
        return _shared_converter(shared);
    }

    inline PyTypeObject const* get_pytype() const {
        return _shared_converter.get_pytype();
    }

};

} // namespace boost::python
} // namespace boost

#endif // !BOOST_PYTHON_TO_PYTHON_STD_AUTO_PTR_HPP
