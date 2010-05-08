// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_FROM_PYTHON_CONTAINER_HPP
#define BOOST_PYTHON_FROM_PYTHON_CONTAINER_HPP

#include <boost/python/from_python/iterator.hpp>

namespace boost {
namespace python {

/**
 *  @brief An rvalue from-python converter that creates a container (or anything
 *         else that can be constructed from a pair of iterators) from an arbitrary
 *         Python sequence.
 *
 *  For overloaded functions, this converter will match any zero-length sequence
 *  or any sequence in which the first element is the correct type.  It will not
 *  match Python objects which do not have a __len__ special method, or 
 *  sequences in which the first element is the wrong type, allowing
 *  other converters to attempt these.  It will raise an exception if the first
 *  element is the correct type and a subsequent element is not, or if the object
 *  has __len__ but not __iter__ or __getitem__.
 */
template <typename Container, typename Value=typename Container::value_type>
struct container_from_python_sequence {

    static void declare() {
        converter::registry::push_back(
            &convertible,
            &construct,
            type_id< Container >()
        );
    }

    static void* convertible(PyObject * obj) {
        try {
            object sequence(handle<>(borrowed(obj)));
            if (len(sequence) > 1) {
                if (!extract<Value>(sequence[0]).check())
                    return NULL;
            }
            return obj;
        } catch (error_already_set & err) {
            handle_exception();
            return NULL;
        }
    }

    static void construct(PyObject* obj, converter::rvalue_from_python_stage1_data* data) {
        object sequence(handle<>(borrowed(obj)));
        typedef converter::rvalue_from_python_storage<Container> storage_t;
        storage_t* storage = reinterpret_cast<storage_t*>(data);
        void* bytes = storage->storage.bytes;
        new (bytes) Container(
            from_python_iterator<Value>(sequence), 
            from_python_iterator<Value>()
        );
        data->convertible = bytes;
    }
};

} // namespace boost::python
} // namespace boost

#endif // !BOOST_PYTHON_FROM_PYTHON_BOOST_FUSION_HPP
