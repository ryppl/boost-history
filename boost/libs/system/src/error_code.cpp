//  error_code support implementation file  ----------------------------------//

//  Copyright Beman Dawes 2002, 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

//----------------------------------------------------------------------------//

//  VC++ 8.0 warns on usage of certain Standard Library and API functions that
//  can be cause buffer overruns or other possible security issues if misused.
//  See http://msdn.microsoft.com/msdnmag/issues/05/05/SafeCandC/default.aspx
//  But the wording of the warning is misleading and unsettling, there are no
//  portable altersystem functions, and VC++ 8.0's own libraries use the
//  functions in question. So turn off the warnings.
#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE

// define BOOST_SYSTEM_SOURCE so that <boost/system/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_SYSTEM_SOURCE 

#include <boost/system/config.hpp>
#include <boost/system/error_code.hpp>
#include <boost/cerrno.hpp>
#include <vector>
#include <cstdlib>
#include <cassert>

using namespace boost::system;

#include <cstring> // for strerror/strerror_r

# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::strerror; }
# endif

# if defined( BOOST_WINDOWS_API )
#   include "windows.h"
#   ifndef ERROR_INCORRECT_SIZE
#    define ERROR_INCORRECT_SIZE ERROR_BAD_ARGUMENTS
#   endif
# endif

//----------------------------------------------------------------------------//

namespace
{

  struct system_to_posix_t
  { 
    int system_value;
    boost::system::posix_errno posix_value;
  };

  const system_to_posix_t system_to_posix[] = 
  {

#ifdef BOOST_POSIX_API
  //  POSIX-like O/S -> posix_errno decode table  ----------------------------//

    // most common errors first to speed sequential search
    { ENOENT, no_such_file_or_directory },
    { EACCES, permission_denied },
    { EINVAL, invalid_argument },

    // rest are alphabetical for easy maintenance
    { 0, no_error }, 
    { E2BIG, argument_list_too_long },
    { EADDRINUSE, address_in_use },
    { EADDRNOTAVAIL, address_not_available },
    { EAFNOSUPPORT, address_family_not_supported },
    { EAGAIN, resource_unavailable_try_again },
    { EALREADY, connection_already_in_progress },
    { EBADF, bad_file_descriptor },
    { EBADMSG, bad_message },
    { EBUSY, device_or_resource_busy },
    { ECANCELED, operation_canceled },
    { ECHILD, no_child_process },
    { ECONNABORTED, connection_aborted },
    { ECONNREFUSED, connection_refused },
    { ECONNRESET, connection_reset },
    { EDEADLK, resource_deadlock_would_occur },
    { EDESTADDRREQ, destination_address_required },
    { EDOM, argument_out_of_domain },
    { EEXIST, file_exists },
    { EFAULT, bad_address },
    { EFBIG, file_too_large },
    { EHOSTUNREACH, host_unreachable },
    { EIDRM, identifier_removed },
    { EILSEQ, illegal_byte_sequence },
    { EINPROGRESS, operation_in_progress },
    { EINTR, interrupted },
    { EIO, io_error },
    { EISCONN, already_connected },
    { EISDIR, is_a_directory },
    { ELOOP, too_many_synbolic_link_levels },
    { EMFILE, too_many_files_open },
    { EMLINK, too_many_links },
    { EMSGSIZE, message_size },
    { ENAMETOOLONG, filename_too_long },
    { ENETDOWN, network_down },
    { ENETRESET, network_reset },
    { ENETUNREACH, network_unreachable },
    { ENFILE, too_many_files_open_in_system },
    { ENOBUFS, no_buffer_space },
    { ENODATA, no_message_available },
    { ENODEV, no_such_device },
    { ENOEXEC, executable_format_error },
    { ENOLCK, no_lock_available },
    { ENOLINK, no_link },
    { ENOMEM, not_enough_memory },
    { ENOMSG, no_message },
    { ENOPROTOOPT, no_protocol_option },
    { ENOSPC, no_space_on_device },
    { ENOSR, no_stream_resources },
    { ENOSTR, not_a_stream },
    { ENOSYS, function_not_supported },
    { ENOTCONN, not_connected },
    { ENOTDIR, not_a_directory },
    { ENOTEMPTY, directory_not_empty },
    { ENOTRECOVERABLE, state_not_recoverable },
    { ENOTSOCK, not_a_socket },
    { ENOTSUP, not_supported },
    { ENOTTY, inappropriate_io_control_operation },
    { ENXIO, no_such_device_or_address },
    { EOPNOTSUPP, operation_not_supported },
    { EOVERFLOW, value_too_large },
    { EOWNERDEAD, owner_dead },
    { EPERM, operation_not_permitted },
    { EPIPE, broken_pipe },
    { EPROTO, protocol_error },
    { EPROTONOSUPPORT, protocol_not_supported },
    { EPROTOTYPE, wrong_protocol_type },
    { ERANGE, result_out_of_range },
    { EROFS, read_only_file_system },
    { ESPIPE, invalid_seek },
    { ESRCH, no_such_process },
    { ETIME, stream_timeout },
    { ETIMEDOUT, timed_out },
    { ETXTBSY, text_file_busy },
    { EWOULDBLOCK, operation_would_block },
    { EXDEV, cross_device_link }

#else

  //  Windows system -> posix_errno decode table  ----------------------------//  

    // see WinError.h comments for descriptions of errors
    
    // most common errors first to speed sequential search
    { ERROR_FILE_NOT_FOUND, no_such_file_or_directory },
    { ERROR_PATH_NOT_FOUND, no_such_file_or_directory },

    // rest are alphabetical for easy maintenance
    { 0, no_error }, 
    { ERROR_ACCESS_DENIED, permission_denied },
    { ERROR_ALREADY_EXISTS, file_exists },
    { ERROR_BAD_UNIT, no_such_device },
    { ERROR_BUFFER_OVERFLOW, filename_too_long },
    { ERROR_BUSY, device_or_resource_busy },
    { ERROR_BUSY_DRIVE, device_or_resource_busy },
    { ERROR_CANNOT_MAKE, permission_denied },
    { ERROR_CANTOPEN, io_error },
    { ERROR_CANTREAD, io_error },
    { ERROR_CANTWRITE, io_error },
    { ERROR_CURRENT_DIRECTORY, permission_denied },
    { ERROR_DEV_NOT_EXIST, no_such_device },
    { ERROR_DEVICE_IN_USE, device_or_resource_busy },
    { ERROR_DIR_NOT_EMPTY, directory_not_empty },
    { ERROR_DIRECTORY, invalid_argument }, // WinError.h: "The directory name is invalid"
    { ERROR_DISK_FULL, no_space_on_device },
    { ERROR_FILE_EXISTS, file_exists },
    { ERROR_HANDLE_DISK_FULL, no_space_on_device },
    { ERROR_INVALID_ACCESS, permission_denied },
    { ERROR_INVALID_DRIVE, no_such_device },
    { ERROR_INVALID_FUNCTION, function_not_supported },
    { ERROR_INVALID_HANDLE, invalid_argument },
    { ERROR_INVALID_NAME, invalid_argument },
    { ERROR_LOCK_VIOLATION, no_lock_available },
    { ERROR_LOCKED, no_lock_available },
    { ERROR_NEGATIVE_SEEK, invalid_argument },
    { ERROR_NOACCESS, permission_denied },
    { ERROR_NOT_ENOUGH_MEMORY, not_enough_memory },
    { ERROR_NOT_READY, resource_unavailable_try_again },
    { ERROR_NOT_SAME_DEVICE, cross_device_link },
    { ERROR_OPEN_FAILED, io_error },
    { ERROR_OPEN_FILES, device_or_resource_busy },
    { ERROR_OUTOFMEMORY, not_enough_memory },
    { ERROR_READ_FAULT, io_error },
    { ERROR_SEEK, io_error },
    { ERROR_SHARING_VIOLATION, permission_denied },
    { ERROR_TOO_MANY_OPEN_FILES, too_many_files_open },
    { ERROR_WRITE_FAULT, io_error },
    { ERROR_WRITE_PROTECT, permission_denied }

#endif
  };

  //  standard error categories  -------------------------------------------//

  class system_error_category : public error_category
  {
  public:
    const std::string &   name() const;
    posix_errno           posix( int ev ) const;
  };

  const system_error_category system_category_const;

  //  system_error_category implementation  --------------------------------// 

  const std::string & system_error_category::name() const
  {
    static const std::string s( "system" );
    return s;
  }

  posix_errno system_error_category::posix( int ev ) const
  {
    const system_to_posix_t * cur = system_to_posix;
    do
    {
      if ( ev == cur->system_value )
        return cur->posix_value;
      ++cur;
    } while ( cur != system_to_posix
      + sizeof(system_to_posix)/sizeof(system_to_posix_t) );
    return boost::system::no_posix_equivalent;
  }


} // unnamed namespace

namespace boost
{
  namespace system
  {
    BOOST_SYSTEM_DECL const error_category & system_category = system_category_const;
  } // namespace system
} // namespace boost
