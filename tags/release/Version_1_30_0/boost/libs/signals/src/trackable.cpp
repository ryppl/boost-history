// Boost.Signals library
//
// Copyright (C) 2001, 2002 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// For more information, see http://www.boost.org

#define BOOST_SIGNALS_IN_LIBRARY_SOURCE

#include <boost/signals/trackable.hpp>
#include <algorithm>

namespace boost {
  namespace BOOST_SIGNALS_NAMESPACE {
    void trackable::signal_disconnected(void* obj, void* data)
    {
      trackable* self = reinterpret_cast<trackable*>(obj);
      connection_iterator* signal = 
        reinterpret_cast<connection_iterator*>(data);

      // If we're dying, don't bother erasing the connection from the list;
      // it'll be gone anyway
      if (!self->dying) {
        self->connected_signals.erase(*signal);
      }

      // This iterator pointer won't ever be used again
      delete signal;
    }

    void 
    trackable::signal_connected(connection c, 
                                BOOST_SIGNALS_NAMESPACE::detail::bound_object& binding) const
    {
      // Insert the connection
      connection_iterator pos = 
        connected_signals.insert(connected_signals.end(), c);

      // Make this copy of the object disconnect when destroyed
      pos->set_controlling();

      binding.obj = const_cast<void*>(reinterpret_cast<const void*>(this));
      binding.data = reinterpret_cast<void*>(new connection_iterator(pos));
      binding.disconnect = &signal_disconnected;
    }

    trackable::~trackable()
    {
      dying = true;
    }
  } // end namespace BOOST_SIGNALS_NAMESPACE
}

#ifndef BOOST_MSVC
// Explicit instantiations to keep in the library
template class std::list<boost::BOOST_SIGNALS_NAMESPACE::connection>;
#endif
