//
// Boost.Process
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/process.hpp
//!
//! Includes the declaration of the process class.
//!

#if !defined(BOOST_PROCESS_PROCESS_HPP)
/** \cond */
#define BOOST_PROCESS_PROCESS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <sys/types.h>
}
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

namespace boost {
namespace process {

//template< class Executable, class Arguments > class basic_pipeline;
//class launcher;

// ------------------------------------------------------------------------

//!
//! \brief Generic implementation of the Process concept.
//!
//! The process class implements the Process concept in an operating system
//! agnostic way.
//!
class process
{
public:
#if defined(BOOST_PROCESS_DOXYGEN)
    //!
    //! \brief Opaque name for the native process' identifier type.
    //!
    //! Each operating system identifies processes using a specific type.
    //! The \a id_type type is used to transparently refer to a process
    //! regarless of the operating system in which this class is used.
    //!
    //! This type is guaranteed to be an integral type on all supported
    //! platforms.
    //!
    typedef NativeProcessId id_type;
#elif defined(BOOST_PROCESS_WIN32_API)
    typedef DWORD id_type;
#elif defined(BOOST_PROCESS_POSIX_API)
    typedef pid_t id_type;
#endif

    //!
    //! \brief Returns the process' identifier.
    //!
    //! Returns the process' identifier.
    //!
    id_type get_id(void) const;

private:
    //!
    //! \brief The process' identifier.
    //!
    id_type m_id;

public: // XXX?
    //!
    //! \brief Constructs a new process object.
    //!
    //! Creates a new process object that represents a running process
    //! within the system.
    //!
    //! This constructor is protected because the library user has no
    //! business in creating representations of live processes himself;
    //! the library takes care of that in all cases.
    //!
    process(id_type id);
    //template< class Executable, class Arguments >
    //    friend class basic_pipeline;
    //friend class launcher;
};

// ------------------------------------------------------------------------

inline
process::process(id_type id) :
    m_id(id)
{
}

// ------------------------------------------------------------------------

inline
process::id_type
process::get_id(void)
    const
{
    return m_id;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_PROCESS_HPP)
