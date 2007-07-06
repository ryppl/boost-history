//                   -- logger.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_LOGGER_HPP_INCLUDED__
#define CGI_LOGGER_HPP_INCLUDED__

namespace cgi {

  /// The logger class: a helper class for writing to requests log streams
  /**
   * This simply initialises a cgi::ostream to write logging info to a request.
   *
   * The reply class is analogous to this except this works on the request's
   * error output rather than the standard output.
   */
  class logger
    : public ostream
  {
  public:
    /// Default constructor
    explicit logger()
    {
    }

    /// Construct with a particular buffer
    /**
     * Takes a buffer and uses it internally, does nothing with it on
     * destruction.
     */
    logger(std::streambuf* buf)
      : ostream(buf)
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
    logger(CommonGatewayRequest& req)
      : ostream(req)
    {
    }

    ~logger()
    {
    }
  };

} // namespace cgi

#endif // CGI_LOGGER_HPP_INCLUDED__
