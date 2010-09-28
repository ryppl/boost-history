// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_PYTHON_TO_PYTHON_COPY_TO_TUPLE_HPP
#define BOOST_PYTHON_TO_PYTHON_COPY_TO_TUPLE_HPP

#include <boost/python/to_python/copy_to_list.hpp>

namespace boost { namespace python {

/**
 *  @brief A model of ResultConverterGenerator (see Boost.Python docs) that copies an arbitrary iterator
 *         range into a Python tuple.
 *
 *  Useful for functions that return an STL container that one would like transformed into a Python tuple;
 *  use return_value_policy<copy_to_tuple>().
 */
struct copy_to_tuple {

    template <typename Container_>
    struct converter {
        
        typedef typename boost::remove_const< 
            typename boost::remove_reference< Container_ >::type >::type Container;

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
            return PySequence_Tuple(result.ptr());
        }

        inline PyTypeObject const * get_pytype() const { return &PyTuple_Type; }

    };

    template <typename Container>
    struct apply {
        typedef converter<Container> type;
    };

};

}}

#endif // !BOOST_PYTHON_TO_PYTHON_COPY_TO_TUPLE_HPP
