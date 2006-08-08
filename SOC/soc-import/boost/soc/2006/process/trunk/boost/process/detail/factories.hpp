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
//! \file boost/process/detail/factories.hpp
//!
//! Includes the declaration of several private factories to construct
//! objects on behalf of other classes in the library.  This is to prevent
//! the user from instantiating them on his own.
//!

#if !defined(BOOST_PROCESS_DETAIL_FACTORIES_HPP)
/** \cond */
#define BOOST_PROCESS_DETAIL_FACTORIES_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <boost/process/basic_posix_child.hpp>
#elif defined(BOOST_PROCESS_WIN32_API)
#else
#   error "Unsupported platform."
#endif

#include <boost/process/basic_child.hpp>
#include <boost/process/detail/file_handle.hpp>

namespace boost {
namespace process {
namespace detail {

struct factories {

// ------------------------------------------------------------------------

//!
//! \brief Creates a new Child instance.
//!
//! Creates a new instance of the basic_child template using the
//! \a Command_Line template type.  See the template's constructor for
//! more details on the parameters.
//!
template< class Command_Line >
static inline
basic_child< Command_Line >
create_child(typename basic_child< Command_Line >::handle_type h,
             const Command_Line& cl,
             file_handle fhstdin,
             file_handle fhstdout,
             file_handle fhstderr)
{
    return basic_child< Command_Line >(h, cl, fhstdin, fhstdout, fhstderr);
}

// ------------------------------------------------------------------------

//!
//! \brief Creates a new POSIX Child instance.
//!
//! Creates a new instance of the basic_posix_child template using the
//! \a Command_Line template type.  See the template's constructor for
//! more details on the parameters.
//!
template< class Command_Line >
static inline
basic_posix_child< Command_Line >
create_posix_child(typename basic_child< Command_Line >::handle_type h,
                   const Command_Line& cl,
                   detail::info_map& infoin,
                   detail::info_map& infoout)
{
    return basic_posix_child< Command_Line >(h, cl, infoin, infoout);
}

// ------------------------------------------------------------------------

}; // struct factories

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_FACTORIES_HPP)
