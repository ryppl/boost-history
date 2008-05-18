// -*- mode:c++ -*-
//
// Module python_iterator.cpp
//
// Copyright (c) 2003, 2008 Raoul M. Gough
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)
//
// History
// =======
// 2003-09-10   rmg     File creation
// 2008-05-18   rmg     Rename indexing subdirectory to indexing_v2
//
// $Id$
//

#include <boost/python/suite/indexing_v2/python_iterator.hpp>
#include <boost/python/handle.hpp>

////////////////////////////////////////////////////////////////////////////
// python_iterator constructor
///////////////////////////////////////////////////////////////////////////

boost::python::indexing_v2::python_iterator
::python_iterator (boost::python::object obj)
  : m_iter_obj (handle<> (PyObject_GetIter (obj.ptr()))),
    m_next_method (m_iter_obj.attr ("next")),
    m_current()
{
}

////////////////////////////////////////////////////////////////////////////
// Get our next item (if any)
///////////////////////////////////////////////////////////////////////////

bool boost::python::indexing_v2::python_iterator::next ()
{
  bool result = true; // Assume success

  try
    {
      m_current = m_next_method ();
    }

  catch (boost::python::error_already_set const &)
    {
      if (PyErr_ExceptionMatches (PyExc_StopIteration))
        {
          // Eat this exception
          PyErr_Clear ();
          m_current = boost::python::object (); // No current object
          result = false; // Report failure via return value only
        }

      else
        {
          // Pass it up the line
          throw;
        }
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////
// Return our current item
///////////////////////////////////////////////////////////////////////////

boost::python::object
boost::python::indexing_v2::python_iterator::current () const
{
  return m_current;
}
