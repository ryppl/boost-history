//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_BACKEND_TRAITS_HPP)
#define BOOST_SQL_CLI_DETAIL_BACKEND_TRAITS_HPP

#include <string>

namespace boost
{
namespace sql_cli
{

// Backend How-To: Declare your connection class

namespace odbc
{
class connection;
} // namespace odbc

namespace detail
{

// Backend How-To: Add a tag type for your backend

struct odbc_tag {};

template <typename ServerT>
struct backend_traits
{
    typedef void connection_type;
};

template <typename ConnectionT>
struct backend_traits_base
{
    typedef ConnectionT connection_type;
};

// Backend How-To: Define a backend_traits specialization for your backend

template <>
struct backend_traits<odbc_tag> : backend_traits_base<odbc::connection>
{
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_BACKEND_TRAITS_HPP)
