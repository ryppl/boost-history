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

#ifndef BOOST_SYNCHO_LOCKABLE_LOCK_UNTIL_HPP
#define BOOST_SYNCHO_LOCKABLE_LOCK_UNTIL_HPP

//-----------------------------------------------------------------------------
#include <boost/synchro/time.hpp>
//-----------------------------------------------------------------------------

namespace boost { namespace synchro {

    template< typename Lockable>
    inline bool lock_until(Lockable& lockable, system_time const& target_time) {
        return lockable.lock_until(target_time);
    }

}}
#endif // BOOST_SYNCHO_LOCKABLE_LOCK_UNTIL_HPP


