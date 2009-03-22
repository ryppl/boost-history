//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKER_OPTIONS__HPP
#define BOOST_SYNCHRO_LOCKER_OPTIONS__HPP

namespace boost { namespace synchro {

    struct force_lock_t
    {};
    struct defer_lock_t
    {};
    struct try_to_lock_t
    {};
    struct adopt_lock_t
    {};
    struct throw_timeout_t
    {};
    struct nothrow_timeout_t
    {};

    const force_lock_t force_lock={};
    const defer_lock_t defer_lock={};
    const try_to_lock_t try_to_lock={};
    const adopt_lock_t adopt_lock={};
    const throw_timeout_t throw_timeout={};
    const nothrow_timeout_t nothrow_timeout={};

}}

#endif
