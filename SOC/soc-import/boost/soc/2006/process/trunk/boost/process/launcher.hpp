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
 
#if !defined(BOOST_PROCESS_LAUNCHER_HPP)
#define BOOST_PROCESS_LAUNCHER_HPP

#include <set>
#include <utility>

#include <boost/process/basic_child.hpp>
#include <boost/process/detail/environment.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

// XXX Shouldn't be here, nor be named like this.
static const int STDIN = 0;
static const int STDOUT = 1;
static const int STDERR = 2;

class launcher
{
    typedef std::set< std::pair< int, int > > merge_set;
    typedef std::set< int > input_set;
    typedef std::set< int > output_set;

    merge_set m_merge_set;
    input_set m_input_set;
    output_set m_output_set;

    detail::environment m_environment;

public:
    launcher& input(int desc);
    launcher& output(int desc);
    launcher& merge(int from, int to);

    void set_environment(const std::string& var, const std::string& value);
    void unset_environment(const std::string& var);

    template< class Attributes >
    basic_child< Attributes > start(const Attributes& attrs);
};

// ------------------------------------------------------------------------

inline
launcher&
launcher::input(int desc)
{
    m_input_set.insert(desc);
    return *this;
}

// ------------------------------------------------------------------------

inline
launcher&
launcher::output(int desc)
{
    m_output_set.insert(desc);
    return *this;
}

// ------------------------------------------------------------------------

inline
launcher&
launcher::merge(int src, int dest)
{
    m_merge_set.insert(std::pair< int, int >(src, dest));
    return *this;
}

// ------------------------------------------------------------------------

inline
void
launcher::set_environment(const std::string& var, const std::string& value)
{
    BOOST_ASSERT(!var.empty());
    m_environment.set(var, value);
}

// ------------------------------------------------------------------------

inline
void
launcher::unset_environment(const std::string& var)
{
    BOOST_ASSERT(!var.empty());
    m_environment.unset(var);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#if defined(BOOST_PROCESS_WIN32_API)
#   include <boost/process/launcher_win32.hpp>
#else
#   include <boost/process/launcher_posix.hpp>
#endif

#endif // !defined(BOOST_PROCESS_LAUNCHER_HPP)
