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
 
#if !defined(BOOST_PROCESS_LAUNCHER_POSIX_HPP)
#define BOOST_PROCESS_LAUNCHER_POSIX_HPP

#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <boost/process/basic_child.hpp>
#include <boost/process/detail/systembuf.hpp>
#include <boost/process/exceptions.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Attributes >
inline
basic_child< Attributes >
launcher::start(const Attributes& a)
{
    typedef typename basic_child< Attributes >::pipe_map pipe_map;

    pipe_map inpipes;
    for (input_set::const_iterator iter = m_input_set.begin();
         iter != m_input_set.end(); iter++)
        inpipes.insert
            (typename pipe_map::value_type(*iter, detail::shared_pipe()));

    pipe_map outpipes;
    for (output_set::const_iterator iter = m_output_set.begin();
         iter != m_output_set.end(); iter++)
        outpipes.insert
            (typename pipe_map::value_type(*iter, detail::shared_pipe()));

    pid_t pid = ::fork();
    if (pid == -1) {
        boost::throw_exception
            (system_error("boost::process::launcher::start",
                          "fork(2) failed", errno));
    } else if (pid == 0) {
        for (typename pipe_map::iterator iter = inpipes.begin();
             iter != inpipes.end(); iter++) {
            int d = (*iter).first;
            detail::shared_pipe p = (*iter).second;

            p->close_write_end();
            if (d != p->get_read_end())
                p->remap_read_end(d);
        }

        for (typename pipe_map::iterator iter = outpipes.begin();
             iter != outpipes.end(); iter++) {
            int d = (*iter).first;
            detail::shared_pipe p = (*iter).second;

            p->close_read_end();
            if (d != p->get_write_end())
                p->remap_write_end(d);
        }

        for (merge_set::const_iterator iter = m_merge_set.begin();
             iter != m_merge_set.end(); iter++) {
            const std::pair< int, int >& p = (*iter);
            ::close(p.first);
            ::dup2(p.second, p.first);
        }

        try {
            m_environment.setup();
            a.setup();
        } catch (const system_error& e) {
            ::write(STDERR, e.what(), std::strlen(e.what()));
            ::write(STDERR, "\n", 1);
            ::exit(EXIT_FAILURE);
        }

        size_t nargs = a.get_command_line().get_arguments().size();
        char* args[nargs + 2];
        const std::string& executable = a.get_command_line().get_executable();

        {
            std::string::size_type pos = executable.rfind('/');
            if (pos == executable.size())
                pos = 0;
            args[0] = ::strdup(executable.substr(pos).c_str());
        }

        for (size_t i = 0; i < nargs; i++)
            args[i + 1] = ::strdup
                (a.get_command_line().get_arguments()[i].c_str());
        args[nargs + 1] = NULL;

        ::execvp(executable.c_str(), args);
        system_error e("boost::process::launcher::start",
                       "execvp(2) failed", errno);

        for (size_t i = 0; i <= nargs; i++)
            delete [] args[i];

        ::write(STDERR, e.what(), std::strlen(e.what()));
        ::write(STDERR, "\n", 1);
        ::exit(EXIT_FAILURE);
    } else {
        for (typename pipe_map::iterator iter = inpipes.begin();
             iter != inpipes.end(); iter++)
            (*iter).second->close_read_end();

        for (typename pipe_map::iterator iter = outpipes.begin();
             iter != outpipes.end(); iter++)
            (*iter).second->close_write_end();
    }

    basic_child< Attributes > c(pid, a, inpipes, outpipes);
    return c;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_LAUNCHER_POSIX_HPP)
