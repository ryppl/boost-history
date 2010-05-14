// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_PYTHON_COPY_TO_LIST_HPP
#define BOOST_PYTHON_COPY_TO_LIST_HPP

#include <boost/python.hpp>
#include <boost/range.hpp>

namespace boost { namespace python {

/**
 *  @brief A model of ResultConverterGenerator (see Boost.Python docs) that copies any valid iterator
 *         range into a Python list.
 *
 *  Useful for functions that return a STL container that one would like transformed into a Python list;
 *  use return_value_policy<copy_to_list>().
 */
struct copy_to_list {

    template <typename Container>
    struct converter {
        
        typedef typename boost::range_const_iterator<Container>::type Iterator;

        inline bool convertible() const { return true; }

        inline PyObject * operator()(Container const & container) const {
            boost::python::list result;
            try {
                for (Iterator i = boost::begin(container); i != boost::end(container); ++i) {
                    result.append(boost::python::object(*i));
                }
            } catch (error_already_set & exc) {
                handle_exception();
                return NULL;
            }
            Py_INCREF(result.ptr());
            return result.ptr();
        }

        inline PyTypeObject const * get_pytype() const { return &PyList_Type; }

    };

    template <typename Container>
    struct apply {
        typedef converter<Container> type;
    };

};

}}

#endif // !BOOST_PYTHON_COPY_TO_LIST_HPP
