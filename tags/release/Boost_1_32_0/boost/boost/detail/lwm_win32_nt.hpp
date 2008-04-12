#ifndef BOOST_DETAIL_LWM_WIN32_NT_HPP_INCLUDED
#define BOOST_DETAIL_LWM_WIN32_NT_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  boost/detail/lwm_win32_nt.hpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  "No threads" version of lwm_win32.hpp; binary compatible but no-op.
//

namespace boost
{

namespace detail
{

class lightweight_mutex
{
private:

    long l_;

    lightweight_mutex(lightweight_mutex const &);
    lightweight_mutex & operator=(lightweight_mutex const &);

public:

    lightweight_mutex(): l_(0)
    {
    }

    class scoped_lock;
    friend class scoped_lock;

    class scoped_lock
    {
    private:

        scoped_lock(scoped_lock const &);
        scoped_lock & operator=(scoped_lock const &);

    public:

        explicit scoped_lock(lightweight_mutex &)
        {
        }
    };
};

} // namespace detail

} // namespace boost

#endif // #ifndef BOOST_DETAIL_LWM_WIN32_NT_HPP_INCLUDED
