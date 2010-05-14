// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_FROM_PYTHON_BOOST_FUSION_HPP
#define BOOST_PYTHON_FROM_PYTHON_BOOST_FUSION_HPP

#include <boost/python.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/for_each.hpp>

namespace boost {
namespace python {

namespace detail {

struct boost_fusion_from_python_failed {};

struct boost_fusion_from_python_check {
    object iter;
    list copy;

    template <typename T> void operator()(T const & x) {
        handle<> item_handle(allow_null(PyIter_Next(iter.ptr())));
        if (!item_handle) {
            PyErr_Clear();
            throw boost_fusion_from_python_failed();
        }
        object item(item_handle);
        extract<T> can_extract(item);
        if (!can_extract.check()) throw boost_fusion_from_python_failed();
        copy.append(item);
    }

    explicit boost_fusion_from_python_check(object const & iter_) :
        iter(iter_) {}
};

struct boost_fusion_from_python_set {
    list sequence;
    mutable int index;

    template <typename T> void operator()(T & x) const {
        x = extract<T>(sequence[index++]);
    }

    explicit boost_fusion_from_python_set(object const & sequence_) :
        sequence(sequence_), index(0) {}
};

} // namespace detail

/**
 *  @brief An rvalue from-python converter that converts any Python iterable to a Boost.Fusion sequence.
 *
 *  The converter will only match if all of the elements in the iterable are convertable to
 *  the corresponding types in the Boost.Fusion sequence.  
 */
template <typename Sequence>
struct boost_fusion_from_python {

    boost_fusion_from_python() {
        converter::registry::push_back(
            &convertible,
            &construct,
            type_id< Sequence >()
        );
    }

    static void* convertible(PyObject* obj_ptr) {
        handle<> iter_handle(allow_null(PyObject_GetIter(obj_ptr)));
        if (!iter_handle) {
            PyErr_Clear();
            return NULL;
        }
        detail::boost_fusion_from_python_check functor =
            detail::boost_fusion_from_python_check(object(iter_handle));
        try {
            boost::mpl::for_each<Sequence>(functor);
            handle<> p(allow_null(PyIter_Next(functor.iter.ptr())));
            if (p) return NULL;  // Python sequence is too long; require exact match.
        } catch (detail::boost_fusion_from_python_failed&) {
            return NULL;
        }
        return incref(functor.copy.ptr());
    }

    static void construct(
        PyObject* obj_ptr,
        converter::rvalue_from_python_stage1_data* data
    ) {
        typedef converter::rvalue_from_python_storage<Sequence> storage_t;
        storage_t* storage = reinterpret_cast<storage_t*>(data);
        void* bytes = storage->storage.bytes;
        Sequence * r = new (bytes) Sequence;
        handle<> copy(reinterpret_cast<PyObject*>(data->convertible));
        detail::boost_fusion_from_python_set functor 
            = detail::boost_fusion_from_python_set(object(copy));
        boost::fusion::for_each(*r,functor);
        data->convertible = bytes;
    }

};

} // namespace boost::python
} // namespace boost

#endif // !BOOST_PYTHON_FROM_PYTHON_BOOST_FUSION_HPP
