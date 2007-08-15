//  boost/system/error_code.hpp  ---------------------------------------------//

//  Copyright Beman Dawes 2006, 2007

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

#ifndef BOOST_ERROR_CODE_HPP
#define BOOST_ERROR_CODE_HPP

#include <boost/system/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <boost/operators.hpp>
#include <boost/noncopyable.hpp>
#include <ostream>
#include <string>
#include <stdexcept>

// TODO: undef these macros if not already defined
#include <boost/cerrno.hpp> 

# ifdef BOOST_WINDOWS_API
#   include <winerror.h>
# endif

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace system
  {

    class error_code;

    namespace posix
    {

      enum posix_errno
      {
        success = 0,
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

    } // namespace posix

    //  class error_category  ------------------------------------------------//

    class error_category : public noncopyable
    {
    public:
      virtual ~error_category(){}
      virtual const std::string & name() const;  // see implementation note below
      virtual std::string message( int ev ) const;   // see implementation note below
      virtual error_code portable_error_code( int ev) const;

      bool operator==(const error_category & rhs) const { return this == &rhs; }
      bool operator!=(const error_category & rhs) const { return !(*this == rhs); }
      bool operator<( const error_category & rhs ) const{ return name() < rhs.name(); }
    };

    //  predefined error categories  -----------------------------------------//

    BOOST_SYSTEM_DECL extern const error_category & posix_category;
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
      error_code() : m_val(0), m_cat(&posix_category) {}
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
        m_cat = &posix_category;
      }

      // observers:
      int                     value() const    { return m_val; }
      const error_category &  category() const { return *m_cat; }
      error_code              portable_error_code() const  { return m_cat->portable_error_code(value()); }
      std::string             message() const  { return m_cat->message(value()); }

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
      inline friend bool same( const error_code & lhs,
                               const error_code & rhs )
        //  the more symmetrical non-member syntax is preferred
      {
        return lhs.m_cat == rhs.m_cat && lhs.m_val == rhs.m_val;
      }
                  
      inline friend bool operator==( const error_code & lhs,
                                     const error_code & rhs )
      {
        if ( lhs.m_cat == rhs.m_cat ) return lhs.m_val == rhs.m_val;
        return same( lhs, rhs.portable_error_code() )
            || same( lhs.portable_error_code(), rhs );
        // Rationale: As in the language proper, a single automatic conversion
        // is acceptable, but two automatic conversions are not. Thus
        // return same (lhs.portable_error_code(), rhs.portable_error_code()) is
        // not an acceptable implementation as it performs two conversions.
      }

      inline friend bool operator!=( const error_code & lhs,
                                     const error_code & rhs )
      {
        return !(lhs == rhs);
      }

    private:
      int                     m_val;
      const error_category *  m_cat;

    };


    //  non-member functions  ------------------------------------------------//


    //  posix::posix_errno make_error_code:
    inline error_code make_error_code( posix::posix_errno e )
      { return error_code( e, posix_category ); }


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


    //  error_category implementation  ---------------------------------------//

    //   error_code is a complete type at this point
    inline error_code error_category::portable_error_code( int ev) const
    { 
      return error_code( ev, *this );
    }

    //  error_category implementation note: VC++ 8.0 objects to name() and
    //  message() being pure virtual functions. Thus these implementations.
    inline const std::string & error_category::name() const
    { 
      static std::string s("error: should never be called");
      return s;
    }
    inline std::string error_category::message( int ev ) const
    { 
      static std::string s("error: should never be called");
      return s;
    }

    //  ----------------------------------------------------------------------//

    //  Operating system specific interfaces  --------------------------------//


    //  The interface is divided into general and system-specific portions to
    //  meet these requirements:
    //
    //  * Code calling an operating system API can create an error_code with
    //    a single category (system_category), even for POSIX-like operating
    //    systems that return some POSIX errno values and some native errno
    //    values. This code should not have to pay the cost of distinguishing
    //    between categories, since it is not yet know if that is needed.
    //
    //  * Users wishing to write system-specific code should have enums for
    //    at least the common error cases.
    //
    //  * System specific code should fail at compile time if moved to another
    //    operating system.

#ifdef BOOST_POSIX_API

    //  POSIX-based systems  -------------------------------------------------//

    //  To construct an error_code after a API error:
    //
    //      error_code( errno, system_category )

    //  User code should use the portable "posix" enums for POSIX errors; this
    //  allows such code to be portable to non-POSIX systems. For the non-POSIX
    //  errno values that POSIX-based systems typically provide in addition to 
    //  POSIX values, use the system specific enums below.

# ifdef __CYGWIN__

    namespace cygwin
    {
      enum cygwin_error
      {
        no_net = ENONET,
        no_package = ENOPKG,
        no_share = ENOSHARE,
      };
    }  // namespace cygwin

    inline error_code make_error_code(cygwin::cygwin_error e)
      { return error_code( e, system_category ); }

# elif defined(linux) || defined(__linux) || defined(__linux__)

    namespace lnx  // linux obvious name prempted by its use as predefined macro
    {
      enum linux_error
      {
        advertise_error = EADV,
        bad_exchange = EBADE,
        bad_file_number = EBADFD,
        bad_font_format = EBFONT,
        bad_request_code = EBADRQC,
        bad_request_descriptor = EBADR,
        bad_slot = EBADSLT,
        channel_range = ECHRNG,
        communication_error = ECOMM,
        dot_dot_error = EDOTDOT,
        exchange_full = EXFULL,
        host_down = EHOSTDOWN,
        is_named_file_type= EISNAM,
        key_expired = EKEYEXPIRED,
        key_rejected = EKEYREJECTED,
        key_revoked = EKEYREVOKED,
        level2_halt= EL2HLT,
        level2_no_syncronized= EL2NSYNC,
        level3_halt = EL3HLT,
        level3_reset = EL3RST,
        link_range = ELNRNG,
        medium_type = EMEDIUMTYPE,
        no_anode= ENOANO,
        no_block_device = ENOTBLK,
        no_csi = ENOCSI,
        no_key = ENOKEY,
        no_medium = ENOMEDIUM,
        no_network = ENONET,
        no_package = ENOPKG,
        not_avail = ENAVAIL,
        not_named_file_type= ENOTNAM,
        not_recoverable = ENOTRECOVERABLE,
        not_unique = ENOTUNIQ,
        owner_dead = EOWNERDEAD,
        protocol_no_supported = EPFNOSUPPORT,
        remote_address_changed = EREMCHG,
        remote_io_error = EREMOTEIO,
        remote_object = EREMOTE,
        restart_needed = ERESTART,
        shared_library_access = ELIBACC,
        shared_library_bad = ELIBBAD,
        shared_library_execute = ELIBEXEC,
        shared_library_max_ = ELIBMAX,
        shared_library_section= ELIBSCN,
        shutdown = ESHUTDOWN,
        socket_type_not_supported = ESOCKTNOSUPPORT,
        srmount_error = ESRMNT,
        stream_pipe_error = ESTRPIPE,
        too_many_references = ETOOMANYREFS,
        too_many_users = EUSERS,
        unattached = EUNATCH,
        unclean = EUCLEAN,
      };
    }  // namespace lnx

    inline error_code make_error_code(lnx::linux_error e)
      { return error_code( e, system_category ); }

# endif

    //  TODO: Add more POSIX-based operating systems here


#elif defined(BOOST_WINDOWS_API)

    //  Microsoft Windows  ---------------------------------------------------//

    //  To construct an error_code after a API error:
    //
    //      error_code( ::GetLastError(), system_category )

    namespace windows
    {
      enum windows_error
      {
        success = 0,
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
    }  // namespace windows

    inline error_code make_error_code(windows::windows_error e)
      { return error_code( e, system_category ); }

#else
#  error BOOST_POSIX_API or BOOST_WINDOWS_API must be defined
#endif

  } // namespace system
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_ERROR_CODE_HPP


