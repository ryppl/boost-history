//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2007 Anthony Williams
// (C) Copyright Ion Gaztanaga 2005-2009.
// (C) Copyright Vicente J. Botet Escriba 2010. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying fileLICENSE_1_0.txt 
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNC_LOCK_OPTIONS_HPP
#define BOOST_SYNC_LOCK_OPTIONS_HPP


//!\file
//!Describes the lock options associated with lock constructors.

namespace boost {

//!Type to indicate to a mutex lock constructor that must not lock the mutex.
struct defer_lock_t{};
//!Type to indicate to a mutex lock constructor that must try to lock the mutex.
struct try_to_lock_t {};
//!Type to indicate to a mutex lock constructor that the mutex is already locked.
struct adopt_lock_t{};

//!An object indicating that the locking
//!must be deferred.
static const defer_lock_t      defer_lock      = {};

//!An object indicating that a try_lock()
//!operation must be executed.
static const try_to_lock_t     try_to_lock    = {};

//!An object indicating that the ownership of lockable
//!object must be accepted by the new owner.
static const adopt_lock_t  adopt_lock = {};

} // namespace boost{


#endif // BOOST_SYNC_LOCK_OPTIONS_HPP
