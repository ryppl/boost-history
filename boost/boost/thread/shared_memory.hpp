// Copyright (C) 2002-2003
// William E. Kempf, David Moore
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  William E. Kempf makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifndef BOOST_MUTEX_JDM062402_HPP
#define BOOST_MUTEX_JDM062402_HPP

#include <boost/config.hpp>
#ifndef BOOST_HAS_THREADS
#   error   Thread support is unavailable!
#endif

#include <boost/utility.hpp>
#include <boost/function.hpp>

#include <boost/thread/exceptions.hpp>

#include <string>

namespace boost {

class shared_memory
{
public:
    enum {
        write=0x1,
        create=0x2,
        exclusive=0x4,
    };

    shared_memory(const char *name, std::size_t len, int flags);
    shared_memory(const char *name, std::size_t len, int flags,
        const boost::function1<void,void *>& initfunc);
    ~shared_memory();

    void* get() const { return m_ptr; }

	static const char* root();
	static void set_root(const char* root);

private:
    void init(const char *name, std::size_t len, int flags,
        const boost::function1<void, void*>* initfunc);

    void *m_ptr;        // Pointer to shared memory block
#if defined(BOOST_HAS_WINTHREADS)
    void* m_hmap;
#elif defined(BOOST_HAS_PTHREADS)
    std::string m_name;
    std::size_t m_len;
    int m_hmap;
#endif
};

}  // namespace boost

#endif
