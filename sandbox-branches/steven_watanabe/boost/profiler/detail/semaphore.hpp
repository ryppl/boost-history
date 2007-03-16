#ifndef BOOST_PROFILER_DETAIL_SEMAPHORE_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_SEMAPHORE_HPP_INCLUDED

#include <boost/config.hpp>

#if !defined(BOOST_PROFILER_MULTITHREADED)
    #include <boost/profiler/detail/dummy_semaphore.hpp>
#elif !defined(BOOST_HAS_THREADS)
    #include <boost/profiler/detail/dummy_semaphore.hpp>
#elif defined(BOOST_WINDOWS)
    #include <boost/profiler/detail/windows/semaphore.hpp>
#else
    #error Your platform does not currently support multithreaded profiling
#endif

namespace boost { namespace profiler { namespace detail
{

    class scoped_semaphore
    {
    public:
        scoped_semaphore(semaphore_t &counter):
            m_counter(counter)
        {
            acquire_semaphore(m_counter);
        }
        ~scoped_semaphore()
        {
            release_semaphore(m_counter);
        }
    private:
        semaphore_t &m_counter;
        scoped_semaphore(const scoped_semaphore &);
        scoped_semaphore &operator =(const scoped_semaphore &);
    };

    inline semaphore_t &index_semaphore()
    {
        static semaphore_t semaphore(1);
        return semaphore;
    }

    inline semaphore_t &point_semaphore()
    {
        static semaphore_t semaphore(1);
        return semaphore;
    }
    
    inline semaphore_t &context_semaphore()
    {
        static semaphore_t semaphore(1);
        return semaphore;
    }

} } }

#endif

