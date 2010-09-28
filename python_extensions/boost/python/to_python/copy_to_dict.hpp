// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_PYTHON_TO_PYTHON_COPY_TO_DICT_HPP
#define BOOST_PYTHON_TO_PYTHON_COPY_TO_DICT_HPP

#include <boost/python/to_python/copy_to_list.hpp>

namespace boost { namespace python {

/**
 *  @brief A model of ResultConverterGenerator (see Boost.Python docs) that copies an arbitrary iterator
 *         range into a Python dict.  The iterator must dereference to std::pair, or some other type
 *         with "first" and "second" data members.
 *
 *  Useful for functions that return an STL pair-associative container (i.e. std::map) that one would
 *  like transformed into a Python dict; use return_value_policy<copy_to_dict>().
 */
struct copy_to_dict {

    template <typename Container_>
    struct converter {
        
        typedef typename boost::remove_const< 
            typename boost::remove_reference< Container_ >::type >::type Container;

        typedef typename boost::range_const_iterator<Container>::type Iterator;

        inline bool convertible() const { return true; }

        inline PyObject * operator()(Container const & container) const {
            dict result;
            try {
                for (Iterator i = boost::begin(container); i != boost::end(container); ++i) {
                    object key(i->first);
                    object value(i->second);
                    result[key] = value;
                }
            } catch (error_already_set & exc) {
                handle_exception();
                return NULL;
            }
            Py_INCREF(result.ptr());
            return result.ptr();
        }

        inline PyTypeObject const * get_pytype() const { return &PyDict_Type; }

    };

    template <typename Container>
    struct apply {
        typedef converter<Container> type;
    };

};

}}

#endif // !BOOST_PYTHON_TO_PYTHON_COPY_TO_DICT_HPP
