// -*- mode:c++ -*-
//
// Header file slice_handler.hpp
//
// Copyright (c) 2003 Raoul M. Gough
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)
//
// History
// =======
// 2003/ 9/ 9   rmg     File creation
//
// $Id$
//

#ifndef slice_handler_rmg_20030909_included
#define slice_handler_rmg_20030909_included

#include <boost/python/object.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/make_function.hpp>
#include <boost/mpl/apply.hpp>
#include <algorithm>

#include <boost/python/suite/indexing/slice.hpp>
#include <boost/python/suite/indexing/python_iterator.hpp>

namespace boost { namespace python { namespace indexing {
  template<class Algorithms, class Policy>
  struct slice_handler
  {
    static boost::python::object make_getitem (Policy const &);
    static boost::python::object make_setitem (Policy const &);
    static boost::python::object make_delitem (Policy const &);
    static boost::python::object make_extend  (Policy const &);

  private:
    typedef typename Algorithms::container container;
    typedef typename Algorithms::reference reference;
    typedef typename Algorithms::slice_helper slice_helper;

    static boost::python::list get_slice (container &, slice);
    static void set_slice (container &, slice, boost::python::object);
    static void del_slice (container &, slice);
    static void extend (container &, boost::python::object);

    struct postcall_override
    {
      // This class overrides our Policy's postcall function and
      // result_conveter to handle the list returned from get_slice.
      // The Policy's result_converter is removed, since it gets
      // applied within get_slice. Our postcall override applies the
      // original postcall to each element of the Python list returned
      // from get_slice.

      typedef boost::python::default_result_converter result_converter;
      typedef typename Policy::argument_package argument_package; // ?

      postcall_override (Policy const &p);

      bool precall (PyObject *args);
      PyObject* postcall (PyObject *args, PyObject *result);

    private:
      Policy mBase;
    };
  };

  //////////////////////////////////////////////////////////////////////////
  // postcall_override constructor
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  slice_handler<Algorithms, Policy>
  ::postcall_override::postcall_override (Policy const &p)
    : mBase (p)
  {
  }

  //////////////////////////////////////////////////////////////////////////
  // precall forwarder
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  bool
  slice_handler<Algorithms, Policy>
  ::postcall_override::precall (PyObject *args)
  {
    return mBase.precall (args);
  }

  //////////////////////////////////////////////////////////////////////////
  // Apply base postcall to each element of the list returend by get_slice
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  PyObject *
  slice_handler<Algorithms, Policy>
  ::postcall_override::postcall (PyObject *args, PyObject *result)
  {
    int size = PyList_Size (result);

    for (int count = 0; count < size; ++count)
      {
        mBase.postcall (args, PyList_GetItem (result, count));
      }

    return result;
  }

  //////////////////////////////////////////////////////////////////////////
  // Return a function object that implements the slice version of __getitem__
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  boost::python::object
  slice_handler<Algorithms, Policy>
  ::make_getitem (Policy const &policy)
  {
    return
      boost::python::make_function (get_slice, postcall_override (policy));
  }

  //////////////////////////////////////////////////////////////////////////
  // Return a function object that implements the slice version of __setitem__
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  boost::python::object
  slice_handler<Algorithms, Policy>
  ::make_setitem (Policy const &policy)
  {
    // should we try to get funky with policy::precall?
    return boost::python::make_function (set_slice, policy);
  }

  //////////////////////////////////////////////////////////////////////////
  // Return a function object that implements the slice version of __delitem__
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  boost::python::object
  slice_handler<Algorithms, Policy>
  ::make_delitem (Policy const &policy)
  {
    // should we try to get funky with policy::precall?
    return boost::python::make_function (del_slice, policy);
  }

  //////////////////////////////////////////////////////////////////////////
  // Return a function object that implements extend
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  boost::python::object
  slice_handler<Algorithms, Policy>
  ::make_extend (Policy const &policy)
  {
    // should we try to get funky with policy::precall?
    return boost::python::make_function (extend, policy);
  }

  //////////////////////////////////////////////////////////////////////////
  // Implementation for the slice version of __getitem__
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  boost::python::list
  slice_handler<Algorithms, Policy>
  ::get_slice (container &c, slice sl)
  {
    typedef typename Policy::result_converter converter_type;
    typedef typename Algorithms::reference reference;
    typename boost::mpl::apply1<converter_type, reference>::type converter;

    boost::python::list result;

    slice_helper helper (sl, c);

    while (helper.next())
      {
        // Apply the result converter (only) to each element before
        // appending. postcall is done in postcall_override

        result.append (boost::python::handle<> (converter (helper.current())));
      }

    return result;
  }

  //////////////////////////////////////////////////////////////////////////
  // Implementation for the slice version of __setitem__
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  void
  slice_handler<Algorithms, Policy>
  ::set_slice (container &c, slice sl, boost::python::object values)
  {
    std::auto_ptr<python_iterator> read_ptr (make_iterator (values));

    if (!read_ptr.get())
      {
        PyErr_SetString (PyExc_TypeError
                         , "Type assigned to slice must be a sequence");

        boost::python::throw_error_already_set();
      }

    // Try two kinds of extractors - the first is more efficient (using
    // a reference to existing object, if possible and sensible) and the
    // second allowing implicit conversions.

    typedef boost::python::extract<typename Algorithms::value_param>
      extractor1;

    typedef boost::python::extract<typename Algorithms::value_type>
      extractor2;

    // Note: any error during this operation will probably leave the
    // container partially updated. This can occur (for example) if the
    // replacement sequence is of a different length to the original
    // slice and the container does not support insertion/deletion.
    // This could be prevented if the length of the replacement sequence
    // is known in advance (via __len__, for example) but not otherwise.

    slice_helper write_helper (sl, c);

    // Overwrite and/or insert elements
    while (read_ptr->next())
      {
        extractor1 ex1 (read_ptr->current());

        if (ex1.check())
          {
            write_helper.write (ex1);
          }

        else
          {
            write_helper.write (extractor2 (read_ptr->current()));
          }
      }

    if (write_helper.next())
      {
        // We've run out of elements to read, but write_helper is not
        // finished. Erase the remaining element(s) in the slice
        write_helper.erase_remaining();
      }
  }

  //////////////////////////////////////////////////////////////////////////
  // Implementation for the slice version of __delitem__
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  void
  slice_handler<Algorithms, Policy>
  ::del_slice (container &c, slice sl)
  {
    slice_helper helper (sl, c);

    if (helper.next())
      {
        helper.erase_remaining();
      }
  }

  //////////////////////////////////////////////////////////////////////////
  // Implementation of extend
  //////////////////////////////////////////////////////////////////////////

  template<class Algorithms, class Policy>
  void
  slice_handler<Algorithms, Policy>
  ::extend (container &c, boost::python::object values)
  {
    // Set up a slice at the tail end of the container, and use
    // set_slice to do all the hard work.

    boost::python::object length
      ((boost::python::handle<>
        (PyInt_FromLong (Algorithms::size (c)))));

    slice sl
      ((boost::python::handle<>
        (PySlice_New
         (length.ptr()
          , boost::python::object().ptr()
          , boost::python::object().ptr()))));

    set_slice (c, sl, values);
  }
} } }

#endif // slice_handler_rmg_20030909_included
