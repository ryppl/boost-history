//  boost/system/error_code.hpp  ---------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/filesystem

#ifndef BOOST_SYSTEM_ERROR_CODE_HPP
#define BOOST_SYSTEM_ERROR_CODE_HPP

#include <boost/system/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <boost/operators.hpp>
#include <boost/noncopyable.hpp>
#include <ostream>
#include <string>
#include <stdexcept>

#include <boost/cerrno.hpp> // we don't like doing this, but it appears
                            // unavoidable to implement posix_errno.
# ifdef BOOST_WINDOWS
#   include <winerror.h>
# endif

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace system
  {

    class error_code;

    enum posix_errno
    {
      no_error = 0,
      address_family_not_supported = EAFNOSUPPORT,
      address_in_use = EADDRINUSE,
      address_not_available = EADDRNOTAVAIL,
      already_connected = EISCONN,
      argument_list_too_long = E2BIG,
      argument_out_of_domain = EDOM,
      bad_address = EFAULT,
      bad_file_descriptor = EBADF,
      bad_message = EBADMSG,
      broken_pipe = EPIPE,
      connection_aborted = ECONNABORTED,
      connection_already_in_progress = EALREADY,
      connection_refused = ECONNREFUSED,
      connection_reset = ECONNRESET,
      cross_device_link = EXDEV,
      destination_address_required = EDESTADDRREQ,
      device_or_resource_busy = EBUSY,
      directory_not_empty = ENOTEMPTY,
      executable_format_error = ENOEXEC,
      file_exists = EEXIST,
      file_too_large = EFBIG,
      filename_too_long = ENAMETOOLONG,
      function_not_supported = ENOSYS,
      host_unreachable = EHOSTUNREACH,
      identifier_removed = EIDRM,
      illegal_byte_sequence = EILSEQ,
      inappropriate_io_control_operation = ENOTTY,
      interrupted = EINTR,
      invalid_argument = EINVAL,
      invalid_seek = ESPIPE,
      io_error = EIO,
      is_a_directory = EISDIR,
      message_size = EMSGSIZE,
      network_down = ENETDOWN,
      network_reset = ENETRESET,
      network_unreachable = ENETUNREACH,
      no_buffer_space = ENOBUFS,
      no_child_process = ECHILD,
      no_link = ENOLINK,
      no_lock_available = ENOLCK,
      no_message_available = ENODATA,
      no_message = ENOMSG,
      no_protocol_option = ENOPROTOOPT,
      no_space_on_device = ENOSPC,
      no_stream_resources = ENOSR,
      no_such_device_or_address = ENXIO,
      no_such_device = ENODEV,
      no_such_file_or_directory = ENOENT,
      no_such_process = ESRCH,
      not_a_directory = ENOTDIR,
      not_a_socket = ENOTSOCK,
      not_a_stream = ENOSTR,
      not_connected = ENOTCONN,
      not_enough_memory = ENOMEM,
      not_supported = ENOTSUP,
      operation_canceled = ECANCELED,
      operation_in_progress = EINPROGRESS,
      operation_not_permitted = EPERM,
      operation_not_supported = EOPNOTSUPP,
      operation_would_block = EWOULDBLOCK,
      owner_dead = EOWNERDEAD,
      permission_denied = EACCES,
      protocol_error = EPROTO,
      protocol_not_supported = EPROTONOSUPPORT,
      read_only_file_system = EROFS,
      resource_deadlock_would_occur = EDEADLK,
      resource_unavailable_try_again = EAGAIN,
      result_out_of_range = ERANGE,
      state_not_recoverable = ENOTRECOVERABLE,
      stream_timeout = ETIME,
      text_file_busy = ETXTBSY,
      timed_out = ETIMEDOUT,
      too_many_files_open_in_system = ENFILE,
      too_many_files_open = EMFILE,
      too_many_links = EMLINK,
      too_many_synbolic_link_levels = ELOOP,
      value_too_large = EOVERFLOW,
      wrong_protocol_type = EPROTOTYPE,

      no_posix_equivalent = -1 // TODO: is this a safe value?
    };

    //  class error_category  ------------------------------------------------//

    class error_category : public noncopyable
    {
    public:
      virtual ~error_category(){}
      virtual const std::string & name() const = 0;
      virtual posix_errno posix( int ev) const = 0;

      bool operator==(const error_category & rhs) const { return this == &rhs; }
      bool operator!=(const error_category & rhs) const { return !(*this == rhs); }
      bool operator<( const error_category & rhs ) const
      {
        return name() < rhs.name(); 
      }
    };

    //  predefined system_category  ------------------------------------------//

    BOOST_SYSTEM_DECL extern const error_category & system_category;

    //  class error_code  ----------------------------------------------------//

    //  We want error_code to be a value type that can be copied without slicing
    //  and without requiring heap allocation, but we also want it to have
    //  polymorphic behavior based on the error category. This is achieved by
    //  abstract base class error_category supplying the polymorphic behavior,
    //  and error_code containing a pointer to an object of a type derived
    //  from error_category.
    class BOOST_SYSTEM_DECL error_code
    {
    public:

      // constructors:
      error_code()                                      : m_val(0), m_cat(&system_category) {}
      error_code( int val, const error_category & cat ) : m_val(val), m_cat(&cat) {}

      template<typename Enum>
        error_code(Enum val) { *this = make_error_code(val); }

      // modifiers:
      void assign( int val, const error_category & cat )
      { 
        m_val = val;
        m_cat = &cat;
      }
                                             
      template<typename Enum>
        error_code & operator=( Enum val )
      { 
        *this = make_error_code(val);
        return *this;
      }

      void clear()
      {
        m_val = 0;
        m_cat = &system_category;
      }

      // observers:
      int                     value() const         { return m_val; }
      const error_category &  category() const      { return *m_cat; }
      posix_errno             posix() const         { return m_cat->posix(value()); }        

      typedef void (*unspecified_bool_type)();
      static void unspecified_bool_true() {}

      operator unspecified_bool_type() const  // true if error
      { 
        return m_val == 0 ? 0 : unspecified_bool_true;
      }

      bool operator!() const  // true if no error
      {
        return m_val == 0;
      }

      // relationals:
      bool operator==( const error_code & rhs ) const
      {
        return value() == rhs.value() && category() == rhs.category();
      }
      bool operator!=( const error_code & rhs ) const
      {
        return !(*this == rhs);
      }
      bool operator<( const error_code & rhs ) const
      {
        return category() < rhs.category() 
          || ( category() == rhs.category() && value() < rhs.value() );
      }
      bool operator<=( const error_code & rhs ) const { return *this == rhs || *this < rhs; }
      bool operator> ( const error_code & rhs ) const { return !(*this <= rhs); }
      bool operator>=( const error_code & rhs ) const { return !(*this < rhs); }

    private:
      int                     m_val;
      const error_category *  m_cat;
    };

    //  non-member functions  ------------------------------------------------//

    // TODO: both of these may move elsewhere, but the LWG hasn't spoken yet.

    template <class charT, class traits>
    inline std::basic_ostream<charT,traits>&
      operator<< (std::basic_ostream<charT,traits>& os, error_code ec)
    {
      os << ec.category().name() << ':' << ec.value();
      return os;
    }

    inline std::size_t hash_value( const error_code & ec )
    {
      return static_cast<std::size_t>(ec.value())
        + (ec.category().name().size()
            ? (static_cast<std::size_t>(ec.category().name()
              [ec.category().name().size()-1]) << 16)
            : 0);
    }

    //  implementation specific interfaces  ----------------------------------//

#ifndef BOOST_WINDOWS
  // POSIX-based Operating System:

  inline error_code make_error_code(posix_errno e)
    { return error_code(e,system_category); }

  // TODO: Add code for system specific categories here.

#else
  // Windows:
  namespace system
  {
    enum windows_error
    {
      // These names and values are based on Windows winerror.h
      invalid_function = ERROR_INVALID_FUNCTION,
      file_not_found = ERROR_FILE_NOT_FOUND,
      path_not_found = ERROR_PATH_NOT_FOUND,
      too_many_open_files = ERROR_TOO_MANY_OPEN_FILES,
      access_denied = ERROR_ACCESS_DENIED,
      invalid_handle = ERROR_INVALID_HANDLE,
      arena_trashed = ERROR_ARENA_TRASHED,
      not_enough_memory = ERROR_NOT_ENOUGH_MEMORY,
      invalid_block = ERROR_INVALID_BLOCK,
      bad_environment = ERROR_BAD_ENVIRONMENT,
      bad_format = ERROR_BAD_FORMAT,
      invalid_access = ERROR_INVALID_ACCESS,
      outofmemory = ERROR_OUTOFMEMORY,
      invalid_drive = ERROR_INVALID_DRIVE,
      current_directory = ERROR_CURRENT_DIRECTORY,
      not_same_device = ERROR_NOT_SAME_DEVICE,
      no_more_files = ERROR_NO_MORE_FILES,
      write_protect = ERROR_WRITE_PROTECT,
      bad_unit = ERROR_BAD_UNIT,
      not_ready = ERROR_NOT_READY,
      bad_command = ERROR_BAD_COMMAND,
      crc = ERROR_CRC,
      bad_length = ERROR_BAD_LENGTH,
      seek = ERROR_SEEK,
      not_dos_disk = ERROR_NOT_DOS_DISK,
      sector_not_found = ERROR_SECTOR_NOT_FOUND,
      out_of_paper = ERROR_OUT_OF_PAPER,
      write_fault = ERROR_WRITE_FAULT,
      read_fault = ERROR_READ_FAULT,
      gen_failure = ERROR_GEN_FAILURE,
      sharing_violation = ERROR_SHARING_VIOLATION,
      lock_violation = ERROR_LOCK_VIOLATION,
      wrong_disk = ERROR_WRONG_DISK,
      sharing_buffer_exceeded = ERROR_SHARING_BUFFER_EXCEEDED,
      handle_eof = ERROR_HANDLE_EOF,
      handle_disk_full= ERROR_HANDLE_DISK_FULL,
      rem_not_list = ERROR_REM_NOT_LIST,
      dup_name = ERROR_DUP_NAME,
      bad_net_path = ERROR_BAD_NETPATH,
      network_busy = ERROR_NETWORK_BUSY,
      // ...
      file_exists = ERROR_FILE_EXISTS,
      cannot_make = ERROR_CANNOT_MAKE,
      // ...
      broken_pipe = ERROR_BROKEN_PIPE,
      open_failed = ERROR_OPEN_FAILED,
      buffer_overflow = ERROR_BUFFER_OVERFLOW,
      disk_full= ERROR_DISK_FULL,
      // ...
      lock_failed = ERROR_LOCK_FAILED,
      busy = ERROR_BUSY,
      cancel_violation = ERROR_CANCEL_VIOLATION,
      already_exists = ERROR_ALREADY_EXISTS
      // ...

      // TODO: add more Windows errors
    };
  }

  inline error_code make_error_code(system::windows_error e)
    { return error_code(e,system_category); }

#endif


  } // namespace system
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_SYSTEM_ERROR_CODE_HPP


