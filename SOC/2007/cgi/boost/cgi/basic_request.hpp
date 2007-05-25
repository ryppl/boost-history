#ifndef CGI_BASIC_REQUEST_HPP_INCLUDE_
#define CGI_BASIC_REQUEST_HPP_INCLUDE_

#include <boost/noncopyable.hpp>

namespace cgi {

  /// The 'role' of the request
  /**
   * See the section on 'role types' in the Design notes for more information.
   * @code responder @endcode is by far the most common type
   */
  enum role_type
    { undefined
    , responder
    , authorizer // American spelling, yes
    , filter
    };

  enum status_type
    { ok
    , aborted
    , ended
    };

  template< typename ServiceType, typename Allocator = std::allocator() >
  class basic_request
    : public request_base
    , private boost::noncopyable
  {
  public:
    typedef ServiceType                           service_type;
    typedef typename ServiceType::protocol_type   protocol_type;

    explicit basic_request(service_type& s)
      : service_(s)
      , role_(undefined)
    {
    }

    /// Notify the server the request has finished being handled
    /**
     * In certain situations (such as a Proactor client using the async read
     * functions) it will be necessary to call end, rather than just returning
     * from the sub_main function.
     *
     * @param status_code This value is returned to the server indicating the
     * state of the request after it was finished handling. It is
     * implementation defined how the server deals with this, and it may have
     * no effect on the http status code returned to the client (eg. 200 OK).
     */
    void end(int status_code)
    {
      service_.end_request(this, status_code);
      status_ = status_type::ended;
    }
  private:
    service_type& service_;
    role_type role_;
    int status_code_; // the what to return to the server on request completion
    status_type status_;
  };

} // namespace cgi

#endif // CGI_BASIC_REQUEST_HPP_INCLUDE_

