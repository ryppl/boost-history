//              -- fcgi/specification.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_FCGI_SPECIFICATION_HPP_INCLUDED__
#define CGI_FCGI_SPECIFICATION_HPP_INCLUDED__

#include <cinttypes>

// NOTE: CamelCase style mimicks the FastCGI specification
// SEE: http://www.fastcgi.com/devkit/doc/fcgi-spec.html#S8

namespace cgi {
 namespace fcgi {
  namespace detail {

    // Listening socket file number
    const short LISTENSOCK_FILENO = 0;

    /**
     * Number of bytes in a Header. Future versions of the protocol
     * will not reduce this number
     */
    const short HEADER_LEN = 8;

    // Value for version component of Header
    const short BOOST_CGI_FASTCGI_VERSION_1 = 1;

    // current version
    const unsigned char VERSION_NUM
      = (unsigned char)BOOST_CGI_FASTCGI_VERSION_1;

    // Values for the type component of Header
    enum request_t { BEGIN_REQUEST     =  1
                   , ABORT_REQUEST     =  2
                   , END_REQUEST       =  3
                   , PARAMS            =  4
                   , STDIN             =  5
                   , STDOUT            =  6
                   , STDERR            =  7
                   , DATA              =  8
                   , GET_VALUES        =  9
                   , GET_VALUES_RESULT = 10
                   , UNKNOWN_TYPE      = 11
                   , MAXTYPE           = UNKNOWN_TYPE
    };

    // a null request id is a management record
    static const unsigned char NULL_REQUEST_ID = 0;

    // Mask for flags component of BeginRequestBody
    static const unsigned char KEEP_CONN = 1;

    // The longest message possible per record
    const std::u_int16_t MAX_MSG_LEN = 65535;

    // Values for role component of BeginRequestBody
    enum role_t { RESPONDER  = 1
                , AUTHORIZER = 2
                , FILTER     = 3
                , ANY
    };

    // Values for protocolStatus component of EndRequestBody
    enum status_t { REQUEST_COMPLETE = 0
                  , CANT_MPX_CONN    = 1
                  , OVERLOADED       = 2
                  , UNKNOWN_ROLE     = 3
    };

    struct Header
    {
    private:
      /// The underlying type of a FastCGI header.
      /**
       * To guarantee the header is laid out exactly as we want, the
       * structure must be a POD-type (see http://tinyurl.com/yo9eav).
       */
      struct implementation_type
      {
        unsigned char version_;
        unsigned char type_;
        unsigned char requestIdB1_;
        unsigned char requestIdB0_;
        unsigned char contentLengthB1_;
        unsigned char contentLengthB0_;
        unsigned char paddingLength_;
        unsigned char reserved_;
      } impl;

    public:
      Header() { memset(this->impl, 0, sizeof(*this)); }

      Header(request_t t, int id, int len)
            : impl.version_        (VERSION_NUM)
            , impl.type_           ((unsigned char)t)
            , impl.requestIdB1_    ((unsigned char)(id  >> 8) & 0xff)
            , impl.requestIdB0_    ((unsigned char)(id      ) & 0xff)
            , impl.contentLengthB1_((unsigned char)(len >> 8) & 0xff)
            , impl.contentLengthB0_((unsigned char)(len     ) & 0xff)
            , impl.paddingLength_  ((unsigned char)0)
            , impl.reserved_       (0)
      { }

      std::u_int16_t version() const
      {
        return impl.version_;
      }

      std::u_int16_t type() const
      {
        return impl.type_;
      }

      std::u_int16_t request_id() const
      {
        return impl.requestIdB0_ + (impl.requestIdB1_ << 8);
      }

      std::u_int16_t content_length() const
      {
        return impl.contentLengthB0_ + (impl.contentLengthB1_ << 8);
      }

      std::u_int16_t padding_length() const
      {
        return impl.paddingLength_;
      }
      int body_length() const
      {
        return impl.content_length() + impl.padding_length();
      }
    };

    class BeginRequestBody
    {
      /// The underlying type of a BeginRequestBody sub-header.
      /**
       * To guarantee the header is laid out exactly as we want, the
       * structure must be a POD-type (see http://tinyurl.com/yo9eav).
       */
      struct implementation_type
      {
        unsigned char roleB1_;
        unsigned char roleB0_;
        unsigned char flags_;
        unsigned char reserved_[5];
      } impl;

    public:

      int role() const
      {
        return (impl.roleB1_ << 8 ) + impl.roleB0_;
      }

      unsigned char flags() const
      {
        return impl.flags_;
      }
    };

    struct BeginRequestRecord
    {
      Header header_;
      BeginRequestBody body_;
    };

    class EndRequestBody
    {
      /// The underlying type of an EndRequestBody sub-header.
      /**
       * To guarantee the header is laid out exactly as we want, the
       * structure must be a POD-type (see http://tinyurl.com/yo9eav).
       */
      struct implementation_type
      {
        unsigned char appStatusB3_;
        unsigned char appStatusB2_;
        unsigned char appStatusB1_;
        unsigned char appStatusB0_;
        unsigned char protocolStatus_;
        unsigned char reserved_[3];
      } impl;

    public:
      EndRequestBody( std::u_int64_t appStatus
                    , status_t  procStatus
                    )
        : impl.appStatusB3_( (appStatus >> 24) & 0xff )
        , impl.appStatusB2_( (appStatus >> 16) & 0xff )
        , impl.appStatusB1_( (appStatus >>  8) & 0xff )
        , impl.appStatusB0_( (appStatus >>  0) & 0xff )
        , impl.protocolStatus_((unsigned char)procStatus)
      {
        memset(impl.reserved_, 0, sizeof(impl.reserved_));
      }
    };

    class EndRequestRecord
    {
      /// The underlying type of an EndRequestRecord sub-header.
      /**
       * To guarantee the header is laid out exactly as we want, the
       * structure must be a POD-type (see http://tinyurl.com/yo9eav).
       */
      struct implementation_type
      {
        Header header_;
        EndRequestBody body_;
      } impl;

    public:
      EndRequestRecord( std::u_int16_t id
                      , std::u_int64_t appStatus
                      , status_t  procStatus
                      )
        : impl.header_( END_REQUEST, id, sizeof(EndRequestBody) )
        , impl.body_( appStatus, procStatus )
      {
      }
    };

    class UnknownTypeBody
    {
      /// The underlying type of an UnknownTypeBody sub-header.
      /**
       * To guarantee the header is laid out exactly as we want, the
       * structure must be a POD-type (see http://tinyurl.com/yo9eav).
       */
      struct implementation_type
      {
        unsigned char type_;
        unsigned char reserved_[7];
      } impl;

    public:
      UnknownTypeBody( unsigned char t )
          : impl.type_(t)
      {
        memset(impl.reserved_, 0, sizeof(impl.reserved_));
      }

      unsigned char type() const { return impl.type_; }
    };

    class UnknownTypeRecord
    {
      /// The underlying type of a UnknownTypeRecord sub-header
      /**
       * To guarantee the header is laid out exactly as we want, the
       * structure must be a POD-type (see http://tinyurl.com/yo9eav).
       */
      struct implementation_type
      {
        Header header_;
        UnknownTypeBody body_;
      } impl;

    public:
      UnknownTypeRecord( int type )
        : impl.header_( UNKNOWN_TYPE, 0, sizeof(UnknownTypeBody) )
        , impl.body_( (unsigned char)type ) // not sure why this is C-style
      {
      }
    };
    
  } // namespace detail
 } // namespace fcgi
}// namespace cgi

#endif // CGI_FCGI_SPECIFICATION_HPP_INCLUDED__
