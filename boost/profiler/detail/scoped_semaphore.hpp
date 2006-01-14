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
