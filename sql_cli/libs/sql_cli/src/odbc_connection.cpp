//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <boost/detail/workaround.hpp>

#include <boost/sql_cli/odbc/odbc_environment.hpp>
#include <boost/sql_cli/odbc/odbc_connection.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, <= 0x564)
#   pragma hdrstop
#endif

#include <boost/sql_cli/odbc/odbc_statement.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
#   pragma warn -8080
#endif

namespace boost
{
namespace sql_cli
{
namespace odbc
{

boost::weak_ptr<environment> connection::the_env;

connection::connection() : my_env(the_env.lock())
{
    if ( ! my_env )
    {
        my_env.reset(new environment);
        the_env = my_env;
    }
    check_return(SQLAllocHandle(kind, my_env->get_handle(), 
            &get_handle()));
}

// Note that SQLFreeHandle may fail, so freeing the handle in the destructor 
// might not be the best solution
connection::~connection()
{
    try
    {
        disconnect();
        check_return(SQLFreeHandle(kind, get_handle()));
    }
    catch ( error & )
    {
        // TODO: Decide what to do...
    }
}

void connection::connect(const std::string & srv, const std::string & user, 
        const std::string & auth)
{
    check_return(SQLConnect(get_handle(), (SQLCHAR *)srv.c_str(), SQL_NTS, 
            (SQLCHAR *)user.c_str(), SQL_NTS, (SQLCHAR *)auth.c_str(), 
            SQL_NTS));
}

void connection::disconnect()
{
    check_return(SQLDisconnect(get_handle()));
    disc_sig();
}

void connection::commit()
{
    check_return(SQLEndTran(kind, get_handle(), SQL_COMMIT));
}

void connection::rollback()
{
    check_return(SQLEndTran(kind, get_handle(), SQL_ROLLBACK));
}

std::auto_ptr<sql_cli::detail::statement_base> connection::create_statement()
{
    std::auto_ptr<sql_cli::detail::statement_base> s(new statement(*this));
    return s;
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost
