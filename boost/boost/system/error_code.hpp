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

// TODO: undef these macros if no already defined
#include <boost/cerrno.hpp> 

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace system
  {

    class error_code;
    bool equivalent( const error_code & lhs, const error_code & rhs );

    namespace posix
    {

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

    } // namespace posix

    //  class error_category  ------------------------------------------------//

    class error_category : public noncopyable
    {
    public:
      virtual ~error_category(){}
      virtual const std::string & name() const = 0;
      virtual posix::posix_errno posix( int ev) const = 0;
      virtual error_code generic_error_code( int ev) const = 0;
      virtual std::string message( int ev ) const = 0;

      bool operator==(const error_category & rhs) const { return this == &rhs; }
      bool operator!=(const error_category & rhs) const { return !(*this == rhs); }
      bool operator<( const error_category & rhs ) const
      {
        return name() < rhs.name(); 
      }
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
      error_code()                                      : m_val(0), m_cat(&posix_category) {}
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
      posix::posix_errno      posix() const    { return m_cat->posix(value()); }
      error_code              generic_error_code() const  { return m_cat->generic_error_code(value()); }
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
      bool operator==( const error_code & rhs ) const
      {
        if ( category() == rhs.category() ) return value() == rhs.value();
        return equivalent( *this, rhs );
      }

      bool operator!=( const error_code & rhs ) const
      {
        return !(*this == rhs);
      }

    private:
      int                     m_val;
      const error_category *  m_cat;
    };


    //  non-member functions  ------------------------------------------------//

    //  the more symmetrical non-member syntax is preferred
    inline bool equal( const error_code & lhs, const error_code & rhs )
    {
      return lhs.category() == rhs.category() && lhs.value() == rhs.value();
    }
                
    inline bool equivalent( const error_code & lhs, const error_code & rhs )
    {
      return equal( lhs.generic_error_code(), rhs.generic_error_code() );
    }

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

  } // namespace system
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_ERROR_CODE_HPP


