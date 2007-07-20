//                    -- reply.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_REPLY_HPP_INCLUDED__
#define CGI_REPLY_HPP_INCLUDED__

#include <ostream>

#include "request_ostream.hpp"

namespace cgi {

  /// The reply class: a helper for replying to requests
  /**
   * This simply initialises a cgi::ostream to write replies to a request.
   *
   * The logger class is analogous to this except this works on the request's
   * standard output rather than the error output.
   */
  class reply
    : public request_ostream
  {
  public:
    /// Default constructor
    explicit reply()
    {
    }

    /// Construct with a particular buffer
    /**
     * Takes a buffer and uses it internally, does nothing with it on
     * destruction.
     */
    reply(std::streambuf* buf)
      : request_ostream(buf)
    {
    }

    /// Construct, taking a buffer from an external source
    /**
     * Gets a buffer from the request/protocol service held by the request.
     *
     * <strike>
     * Takes a buffer from T (can be a model of ProtocolService or
     * CommonGatewayRequest) to use internally.
     * </strike>
     */
    template<typename CommonGatewayRequest>
    reply(CommonGatewayRequest& req)
      : request_ostream(req)
    {
    }

    ~reply()
    {
    }
  };

} // namespace cgi

#endif // CGI_REPLY_HPP_INCLUDED__
