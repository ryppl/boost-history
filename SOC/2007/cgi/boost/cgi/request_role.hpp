//                   -- role_type.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_ROLE_TYPE_HPP_INCLUDED__
#define CGI_ROLE_TYPE_HPP_INCLUDED__

namespace cgi {

  /// The 'role' of the request
  /**
   * See the section on 'role types' in the Design notes for more information.
   * @code responder @endcode is by far the most common type.
   */
  enum role_type { responder
                 , authorizer
                 , filter
                 , undefined = responder // CGI programs default to responder
  };

} // namespace cgi

#endif // CGI_ROLE_TYPE_HPP_INCLUDED__
