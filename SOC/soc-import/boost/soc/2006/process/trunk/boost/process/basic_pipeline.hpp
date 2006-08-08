//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/basic_pipeline.hpp
//!
//! Includes the declaration of the basic_pipeline template.
//!

#if !defined(BOOST_PROCESS_BASIC_PIPELINE_HPP)
/** \cond */
#define BOOST_PROCESS_BASIC_PIPELINE_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <boost/process/detail/posix_ops.hpp>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <boost/process/detail/win32_ops.hpp>
#else
#   error "Unsupported platform."
#endif

#include <boost/process/basic_children.hpp>
#include <boost/process/detail/factories.hpp>
#include <boost/process/detail/launcher_base.hpp>

#include <vector>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Pipelined implementation of the Launcher concept.
//!
//! The basic_pipeline template implements the Launcher concept allowing
//! for the construction of process pipelines.  This class is operating
//! system independent.
//!
template< class Command_Line >
class basic_pipeline :
    public detail::launcher_base
{
    //!
    //! \brief Data describing an entry in the pipeline.
    //!
    //! This helper structure is used to describe a process in the
    //! pipeline.
    //!
    struct entry
    {
        Command_Line m_cl;
        bool m_merge_out_err;

        entry(Command_Line cl, bool merge_out_err) :
            m_cl(cl),
            m_merge_out_err(merge_out_err)
        {
        }
    };

    //!
    //! \brief Information about the processes that compose the pipeline.
    //!
    std::vector< entry > m_entries;

public:
    //!
    //! \brief Adds a new stage to the pipeline.
    //!
    //! Prepares a new process that will be part of the pipeline.
    //! This new process is described by its command line and a flag that
    //! indicates whether its stderr should be merged on its stdout.
    //!
    basic_pipeline< Command_Line >& add(const Command_Line& cl,
                                        bool merge_out_err = false);

    //!
    //! \brief Starts the pipeline.
    //!
    //! Spawns all processes that form the pipeline, connecting them
    //! appropriately to share the data flows.
    //!
    //! \return An object that holds the status of all the processes
    //!         related to the pipeline.
    //!
    basic_children< Command_Line > start(void);
};

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_pipeline< Command_Line >&
basic_pipeline< Command_Line >::add(const Command_Line& cl,
                                    bool merge_out_err)
{
    m_entries.push_back(entry(cl, merge_out_err));
    return *this;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_children< Command_Line >
basic_pipeline< Command_Line >::start(void)
{
    BOOST_ASSERT(m_entries.size() >= 2);

    // Method's result value.
    basic_children< Command_Line > children;

    // Convenience variables to avoid clutter below.
    const stream_behavior bin =
        basic_pipeline< Command_Line >::get_stdin_behavior();
    const stream_behavior bout =
        basic_pipeline< Command_Line >::get_stdout_behavior();
    const stream_behavior berr =
        basic_pipeline< Command_Line >::get_stderr_behavior();
    detail::file_handle fhinvalid;

#if defined(BOOST_PROCESS_POSIX_API)
    // The pipes used to connect the pipeline's internal process.
    detail::pipe pipes[m_entries.size() - 1];

    // Process context configuration.
    detail::posix_setup s;
    s.m_work_directory = get_work_directory();

    // Configure and spawn the pipeline's first process.
    {
        typename std::vector< Command_Line >::size_type i = 0;

        detail::info_map infoin, infoout;
        detail::merge_set merges;

        posix_behavior_to_info(bin, STDIN_FILENO, false, infoin);

        detail::stream_info si2;
        si2.m_type = detail::stream_info::usehandle;
        si2.m_handle = pipes[i].wend().disown();
        infoout.insert(detail::info_map::value_type(STDOUT_FILENO, si2));

        if (m_entries[i].m_merge_out_err)
            merges.insert(std::pair< int, int >(STDERR_FILENO,
                                                STDOUT_FILENO));

        pid_t ph = detail::posix_start(m_entries[i].m_cl, get_environment(),
                                       infoin, infoout, merges, s);

        detail::file_handle fhstdin;

        if (bin == redirect_stream)
            fhstdin = posix_info_locate_pipe(infoin, STDIN_FILENO, false);

        children.push_back
            (detail::factories::create_child(ph, m_entries[i].m_cl, fhstdin,
                                             fhinvalid, fhinvalid));
    }

    // Configure and spawn the pipeline's internal processes.
    for (typename std::vector< Command_Line >::size_type i = 1;
         i < m_entries.size() - 1; i++) {
        detail::info_map infoin, infoout;
        detail::merge_set merges;

        detail::stream_info si1;
        si1.m_type = detail::stream_info::usehandle;
        si1.m_handle = pipes[i - 1].rend().disown();
        infoin.insert(detail::info_map::value_type(STDIN_FILENO, si1));

        detail::stream_info si2;
        si2.m_type = detail::stream_info::usehandle;
        si2.m_handle = pipes[i].wend().disown();
        infoout.insert(detail::info_map::value_type(STDOUT_FILENO, si2));

        if (m_entries[i].m_merge_out_err)
            merges.insert(std::pair< int, int >(STDERR_FILENO,
                                                STDOUT_FILENO));

        pid_t ph = detail::posix_start(m_entries[i].m_cl, get_environment(),
                                       infoin, infoout, merges, s);

        children.push_back
            (detail::factories::create_child(ph, m_entries[i].m_cl, fhinvalid,
                                             fhinvalid, fhinvalid));
    }

    // Configure and spawn the pipeline's last process.
    {
        typename std::vector< Command_Line >::size_type i =
            m_entries.size() - 1;

        detail::info_map infoin, infoout;
        detail::merge_set merges;

        detail::stream_info si1;
        si1.m_type = detail::stream_info::usehandle;
        si1.m_handle = pipes[i - 1].rend().disown();
        infoin.insert(detail::info_map::value_type(STDIN_FILENO, si1));

        posix_behavior_to_info(bout, STDOUT_FILENO, true, infoout);
        posix_behavior_to_info(berr, STDERR_FILENO, true, infoout);

        if (m_entries[i].m_merge_out_err || get_merge_out_err())
            merges.insert(std::pair< int, int >(STDERR_FILENO,
                                                STDOUT_FILENO));

        pid_t ph = detail::posix_start(m_entries[i].m_cl, get_environment(),
                                       infoin, infoout, merges, s);

        detail::file_handle fhstdout, fhstderr;

        if (bout == redirect_stream)
            fhstdout = posix_info_locate_pipe(infoout, STDOUT_FILENO, true);

        if (berr == redirect_stream)
            fhstderr = posix_info_locate_pipe(infoout, STDERR_FILENO, true);

        children.push_back
            (detail::factories::create_child(ph, m_entries[i].m_cl,
                                             fhinvalid, fhstdout, fhstderr));
    }
#elif defined(BOOST_PROCESS_WIN32_API)
#   error "Unimplemented."
#endif

    return children;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_PIPELINE_HPP)
