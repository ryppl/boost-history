//
// Boost.Process
// Implementation of the Launcher concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_STANDARD_LAUNCHER_HPP)
#define BOOST_PROCESS_STANDARD_LAUNCHER_HPP

#include <boost/process/launcher.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class standard_launcher :
    public launcher
{
public:
    standard_launcher(void);
};

// ------------------------------------------------------------------------

inline
standard_launcher::standard_launcher(void)
{
    input(STDIN);
    output(STDOUT);
    merge(STDERR, STDOUT);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_STANDARD_LAUNCHER_HPP)
