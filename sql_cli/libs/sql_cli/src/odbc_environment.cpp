//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#define BOOST_SQL_CLI_SOURCE

#include <boost/sql_cli/odbc/odbc_environment.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

environment::environment()
{
    std::string msg = "Environment handle allocation error";
    check_return(SQLAllocHandle(kind, SQL_NULL_HANDLE, &get_handle()), &msg);
    check_return(SQLSetEnvAttr(get_handle(), SQL_ATTR_ODBC_VERSION, 
            reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0));
}

environment::~environment()
{
    try
    {
        check_return(SQLFreeHandle(kind, get_handle()));
    }
    catch ( error & )
    {
        // TODO: Decide what to do...
    }
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost
