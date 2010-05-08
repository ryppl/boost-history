// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_FROM_PYTHON_ITERATOR_HPP
#define BOOST_PYTHON_FROM_PYTHON_ITERATOR_HPP

#include <boost/python.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace boost {
namespace python {

template <typename Value>
class from_python_iterator : public boost::iterator_facade<from_python_iterator<Value>,
                                                           Value,
                                                           boost::forward_traversal_tag,
                                                           Value
                                                           > 
{
    bool _at_end;
    object _iter;
    object _item;
    
    Value dereference() const {
        return extract<Value>(_item);
    }

    void increment() {
        PyObject * item = PyIter_Next(_iter.ptr());
        if (item == NULL) {
            if (PyErr_Occurred()) {
                throw_error_already_set();
            } else {
                _item = object();
                _at_end = true;
            }
        } else {
            _item = object(handle<>(item));
        }
    }

    bool equal(from_python_iterator const & other) const {
        return other._at_end && this->_at_end;
    }

    friend class boost::iterator_core_access;
public:

    from_python_iterator() : _at_end(true), _iter(), _item() {}

    explicit from_python_iterator(object const & iterable) : _at_end(false), _iter(), _item() {
        PyObject * iter = PyObject_GetIter(iterable.ptr());
        if (iter == NULL) throw_error_already_set();
        _iter = object(handle<>(iter));
        increment();
    }

};

} // namespace boost::python
} // namespace boost

#endif // !BOOST_PYTHON_FROM_PYTHON_ITERATOR_HPP
