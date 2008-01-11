//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <vector>

#include <boost/detail/workaround.hpp>

#include <boost/sql_cli/odbc/odbc_result_set.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, <= 0x564)
#   pragma hdrstop
#endif

#include <boost/sql_cli/odbc/odbc_row.hpp>
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

result_set::result_set(statement const & s) : stmt(s)
{
    SQLNumResultCols(s.get_handle(), &num_col);
    if ( num_col == 0 )
        throw error("No active query");
    for ( SQLSMALLINT i = 1; i <= num_col; ++i )
        cols.push_back(create_column(i));
}

boost::shared_ptr<column> result_set::create_column(int n)
{
    typedef std::vector<SQLCHAR> name_type;
    name_type name(32);
    SQLSMALLINT nam_len;
    SQLSMALLINT type;
    SQLUINTEGER size;
    SQLSMALLINT prec;
    SQLSMALLINT nullable;
    SQLRETURN ret = SQLDescribeCol(stmt.get_handle(), 
            static_cast<SQLSMALLINT>(n), &name[0],
            static_cast<SQLSMALLINT>(name.size()), &nam_len, &type, &size,
            &prec, &nullable);
    if ( ret == SQL_SUCCESS_WITH_INFO && 
            name.size() < static_cast<name_type::size_type>(nam_len + 1) )
    {
        name.resize(nam_len + 1);
        ret = SQLDescribeCol(stmt.get_handle(), static_cast<SQLSMALLINT>(n),
                &name[0], static_cast<SQLSMALLINT>(name.size()), &nam_len,
                &type, &size, &prec, &nullable);
    }
    switch(ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        {
            boost::shared_ptr<column> c(new column((const char *)&name[0],
                    type, size, prec, nullable == SQL_NULLABLE, n));
            return c;
        }
    default:
        throw error(stmt, ret);
    }
}

std::auto_ptr<detail::row_base> result_set::create_row()
{
    SQLRETURN ret = SQLFetch(stmt.get_handle()); 

    switch(ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
    case SQL_NO_DATA:
        {
            std::auto_ptr<detail::row_base> r;
            if ( ret == SQL_NO_DATA )
            {
                ret = SQLCloseCursor(stmt.get_handle());
                if ( ret == SQL_SUCCESS_WITH_INFO )
                {
                    // TODO: Handle warning
                }
                else if ( ret != SQL_SUCCESS )
                {
                    throw error(stmt, ret);
                }
           }
            else
                r.reset(new row(*this));
            return r;
        }
    default:
        throw error(stmt, ret);
    }
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost

