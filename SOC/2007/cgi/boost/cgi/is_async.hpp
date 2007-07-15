//                  -- is_async.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_IS_ASYNC_HPP_INCLUDED__
#define CGI_IS_ASYNC_HPP_INCLUDED__

namespace cgi {

  template<typename Protocol>
  struct is_aync
  {
  };

  template<>
  struct is_aync<tags::cgi>
  {
    typedef boost::mpl::bool_<false>   type;
    typedef type::value                value;
  };

  template<>
  struct is_aync<tags::async_cgi>
  {
    typedef boost::mpl::bool_<true>   type;
    typedef type::value               value;
  };

  template<>
  struct is_aync<tags::fcgi>
  {
    typedef boost::mpl::bool_<true>   type;
    typedef type::value               value;
  };

  template<>
  struct is_aync<tags::scgi>
  {
    typedef boost::mpl::bool_<true>   type;
    typedef type::value               value;
  };

} // namespace cgi

#endif // CGI_IS_ASYNC_HPP_INCLUDED__
