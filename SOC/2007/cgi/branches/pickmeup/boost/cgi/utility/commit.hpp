
#ifndef BOOST_CGI_COMMIT_HPP_INCLUDED_
#define BOOST_CGI_COMMIT_HPP_INCLUDED_

#include <map>
///////////////////////////////////////////////////////////
#include "boost/cgi/common/return.hpp"

namespace cgi {
 namespace common {

    /// Send a response to a reqest and close off the request.
    template<typename Request, typename Response>
    int commit(Request& req, Response& resp, int program_status)
    {
      resp.send(req.client());
      return req.close(resp.status(), program_status);
    }

 } // namespace common
} // namespace cgi

#endif // BOOST_CGI_COMMIT_HPP_INCLUDED_

