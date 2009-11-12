//                   -- form_parser.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef BOOST_CGI_CONFIG_HPP_INCLUDED__
#define BOOST_CGI_CONFIG_HPP_INCLUDED__

namespace boost {
 namespace cgi {}
} // namespace boost

#ifndef BOOST_CGI_NAMESPACE
# define BOOST_CGI_NAMESPACE boost::cgi
#endif // BOOST_CGI_NAMESPACE

#ifndef BOOST_CGI_NAMESPACE_BEGIN
# define BOOST_CGI_NAMESPACE_BEGIN namespace boost { namespace cgi {
#endif // BOOST_CGI_NAMESPACE_BEGIN

#ifndef BOOST_CGI_NAMESPACE_END
# define BOOST_CGI_NAMESPACE_END } } // namespace BOOST_CGI_NAMESPACE
#endif // BOOST_CGI_NAMESPACE_END

#endif // BOOST_CGI_CONFIG_HPP_INCLUDED__
