//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_ENVIRONMENT_HPP)
#define BOOST_SQL_CLI_ODBC_ENVIRONMENT_HPP

#include <boost/sql_cli/odbc/odbc_config.hpp>
#include <boost/sql_cli/odbc/odbc_handle.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

class environment : public handle<SQLHENV, SQL_HANDLE_ENV>
{
public:
    environment();
    ~environment();
};

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_ENVIRONMENT_HPP)
