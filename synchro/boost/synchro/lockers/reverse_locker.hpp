//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_REVERSE_LOCKER__HPP
#define BOOST_SYNCHRO_REVERSE_LOCKER__HPP

#include <boost/mpl/bool.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/concept_check.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/locker_concepts.hpp>

namespace boost { namespace synchro {



/*
 *
 * mutex mtx;
 * // ... some read operations
 * { // non locked block
 *         reverse_locker<boost::mutex> > rlock(mutex);
 *      // ... some code not needing
 * } // locked again
 * // ...
 */

template <typename Lockable>
class reverse_locker
{
    reverse_locker(Lockable& mtx):  mtx_(mtx) {mtx_.unlock();}
    ~reverse_locker() {mtx_.lock();}
    BOOST_DEFAULT_CONSTRUCTOR_DELETE(reverse_locker) /*< disable default construction >*/
    BOOST_COPY_CONSTRUCTOR_DELETE(reverse_locker) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(reverse_locker) /*< disable copy asignement >*/

protected:
    Lockable& mtx_;
};


}
}

#endif
