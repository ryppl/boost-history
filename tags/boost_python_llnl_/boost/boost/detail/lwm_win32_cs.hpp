#ifndef BOOST_DETAIL_LWM_WIN32_CS_HPP_INCLUDED
#define BOOST_DETAIL_LWM_WIN32_CS_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/detail/lwm_win32_cs.hpp
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <boost/detail/winapi.hpp>

namespace boost
{

namespace detail
{

class lightweight_mutex
{
private:

    winapi::critical_section cs_;

    lightweight_mutex(lightweight_mutex const &);
    lightweight_mutex & operator=(lightweight_mutex const &);

public:

    lightweight_mutex()
    {
        winapi::InitializeCriticalSection(&cs_);
    }

    ~lightweight_mutex()
    {
        winapi::DeleteCriticalSection(&cs_);
    }

    class scoped_lock;
    friend class scoped_lock;

    class scoped_lock
    {
    private:

        lightweight_mutex & m_;

        scoped_lock(scoped_lock const &);
        scoped_lock & operator=(scoped_lock const &);

    public:

        explicit scoped_lock(lightweight_mutex & m): m_(m)
        {
            winapi::EnterCriticalSection(&m_.cs_);
        }

        ~scoped_lock()
        {
            winapi::LeaveCriticalSection(&m_.cs_);
        }
    };
};

} // namespace detail

} // namespace boost

#endif // #ifndef BOOST_DETAIL_LWM_WIN32_CS_HPP_INCLUDED
