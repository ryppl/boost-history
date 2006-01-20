// ----------------------------------------------------------------------------
// Copyright (C) 2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROFILER_DETAIL_SCOPED_SEMAPHORE_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_SCOPED_SEMAPHORE_HPP_INCLUDED

#include <boost/profiler/detail/semaphore.hpp>

namespace boost { namespace profiler { namespace detail 
{

    class scoped_semaphore
    {
    public:
        scoped_semaphore(semaphore_t *counter):
            m_counter(counter)
        {
            acquire_semaphore(m_counter);
        }
        ~scoped_semaphore()
        {
            release_semaphore(m_counter);
        }
    private:
        semaphore_t *m_counter;
        scoped_semaphore(const scoped_semaphore &);
        scoped_semaphore &operator =(const scoped_semaphore &);
    };

} } }

#endif
