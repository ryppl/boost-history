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

#include <map>
#include <set>
#include <utility>

#include <boost/assert.hpp>
#include <boost/process/basic_child.hpp>

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

    class environment_entry
    {
        bool m_set;
        std::string m_value;

    public:
        explicit
        environment_entry(bool setit, const std::string& value = "") :
            m_set(setit),
            m_value(value)
        {
        }

        bool
        is_set(void)
            const
        {
            return m_set;
        }

        const std::string&
        get_value(void)
            const
        {
            BOOST_ASSERT(m_set);
            return m_value;
        }
    };
    typedef std::map< std::string, environment_entry > environment_map;

    environment_map m_environment;

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
launcher::set_environment(const std::string& var,
                                const std::string& value)
{
    BOOST_ASSERT(not var.empty());
    m_environment.insert
        (environment_map::value_type(var, environment_entry(true, value)));
}

// ------------------------------------------------------------------------

inline
void
launcher::unset_environment(const std::string& var)
{
    BOOST_ASSERT(not var.empty());
    m_environment.insert
        (environment_map::value_type(var, environment_entry(false)));
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
