//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHO_LOCKER_VAR_LOCK_GUARD_HPP
#define BOOST_SYNCHO_LOCKER_VAR_LOCK_GUARD_HPP

//-----------------------------------------------------------------------------
#include <stdarg.h>
//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------
#include <boost/synchro/tags.hpp>
#include <boost/synchro/exceptions.hpp>
#include <boost/synchro/lockable/lock.hpp>
#include <boost/synchro/lockable/unlock.hpp>
//-----------------------------------------------------------------------------

namespace boost { namespace synchro {

    //-----------------------------------------------------------------------------
    template <typename T>
    class var_lock_guard {
    public:

       typedef T lock_type;
       typedef std::list<lock_type*> list_type;

       //--------------------------------------------------------------------------
       var_lock_guard(lock_type *l, ...)  {
          va_list ap;
          va_start(ap, l);
          while (l) {
             lockList_.push_back(l);
             l = va_arg(ap, lock_type*);
          }
          va_end(ap);

          for (typename list_type::iterator it = lockList_.begin(); 
                it != lockList_.end(); ++it) {
             synchro::lock(**it);
          }
       }

       //--------------------------------------------------------------------------
       ~var_lock_guard() {
          for (typename list_type::iterator it = lockList_.begin(); 
              it != lockList_.end(); ++it) {
             synchro::unlock(**it);
          }
       }

    private:
       list_type lockList_;

    };

}}
#endif // BOOST_SYNCHO_LOCKER_VAR_LOCK_GUARD_HPP


