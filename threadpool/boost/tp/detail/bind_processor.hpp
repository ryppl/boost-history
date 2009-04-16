//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THREAD_BIND_PROCESSOR_H
#define BOOST_THREAD_BIND_PROCESSOR_H

#include <boost/tp/detail/config.hpp>

#define BOOST_THREAD_BIND_PROCESSOR_AFFINITY 1

# if defined(BOOST_WINDOWS_OS)
#   include <boost/tp/detail/bind_processor_windows.hpp>
# elif defined(BOOST_LINUX_OS)
#   include <boost/tp/detail/bind_processor_linux.hpp>
//# elif defined(BOOST_xBSD_OS)
//#   include <boost/tp/detail/bind_processor_bsd.hpp>
# elif defined(BOOST_AIX_OS)
#   include <boost/tp/detail/bind_processor_aix.hpp>
# elif defined(BOOST_HPUX_OS)
#   include <boost/tp/detail/bind_processor_hpux.hpp>
# elif defined(BOOST_SOLARIS_OS)
#   include <boost/tp/detail/bind_processor_solaris.hpp>
# else
#   undef BOOST_THREAD_BIND_PROCESSOR_AFFINITY
# endif

#endif // BOOST_THREAD_BIND_PROCESSOR_H
