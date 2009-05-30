
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_BIND_PROCESSOR_H
#define BOOST_TASK_DETAIL_BIND_PROCESSOR_H

#include <boost/task/detail/config.hpp>

# if defined(BOOST_TASK_WIN32)
#   define BOOST_HAS_PROCESSOR_BINDINGS 1
#   include <boost/task/detail/bind_processor_windows.hpp>
# elif defined(BOOST_TASK_LINUX)
#   define BOOST_HAS_PROCESSOR_BINDINGS 1
#   include <boost/task/detail/bind_processor_linux.hpp>
# elif defined(BOOST_TASK_AIX)
#   define BOOST_HAS_PROCESSOR_BINDINGS 1
#   include <boost/task/detail/bind_processor_aix.hpp>
# elif defined(BOOST_TASK_HPUX)
#   define BOOST_HAS_PROCESSOR_BINDINGS 1
#   include <boost/task/detail/bind_processor_hpux.hpp>
# elif defined(BOOST_TASK_SOLARIS)
#   define BOOST_HAS_PROCESSOR_BINDINGS 1
#   include <boost/task/detail/bind_processor_solaris.hpp>
# else
#   undef BOOST_HAS_PROCESSOR_BINDINGS
# endif

#endif // BOOST_TASK_DETAIL_BIND_PROCESSOR_H
