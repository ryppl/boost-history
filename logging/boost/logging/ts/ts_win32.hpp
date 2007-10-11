// defs.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

#ifndef JT_BOOST_LOG_TS_WIN32_HPP
#define JT_BOOST_LOG_TS_WIN32_HPP


#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

// many thanks to Terris Linerbach!
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>


namespace boost { namespace logging { namespace threading {

class scoped_lock_win32 ;

class mutex_win32 {

    mutex_win32 & operator = ( const mutex_win32 & Not_Implemented);
    mutex_win32( const mutex_win32 & From);
public:
    typedef scoped_lock_win32 scoped_lock;

    mutex_win32() {
        InitializeCriticalSection( GetCriticalSectionPtr() );
    }
    ~mutex_win32() {
        DeleteCriticalSection(GetCriticalSectionPtr());
    }
    void Lock() {
        EnterCriticalSection( GetCriticalSectionPtr());
    }
    void Unlock() {
        LeaveCriticalSection( GetCriticalSectionPtr());
    }
private:
    LPCRITICAL_SECTION GetCriticalSectionPtr() const { return &m_cs; }
    mutable CRITICAL_SECTION m_cs;
};

class scoped_lock_win32 {
    scoped_lock_win32 operator=( scoped_lock_win32 & Not_Implemented);
    scoped_lock_win32( const scoped_lock_win32 & Not_Implemented);
public:
    scoped_lock_win32( mutex_win32 & cs) : m_cs( cs)                { m_cs.Lock(); }
    ~scoped_lock_win32()                                      { m_cs.Unlock(); }
private:
    mutex_win32 & m_cs;
};

}}}

#endif

