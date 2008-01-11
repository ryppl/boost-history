//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_RESULT_SET_HPP)
#define BOOST_SQL_CLI_ODBC_RESULT_SET_HPP

#include <memory>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <boost/sql_cli/odbc/odbc_config.hpp>
#include <boost/sql_cli/odbc/odbc_column.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>
#include <boost/sql_cli/detail/result_set_base.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{
class row_base;
} // namespace detail

namespace odbc
{
class statement;

class result_set : public sql_cli::detail::result_set_base
{
public:
    result_set(statement const & s);

    statement const & get_statement() const { return stmt; }

    int column_count() const { return num_col; }

    boost::shared_ptr<column> get_odbc_column(int i)
    {
        if ( i < 0 || i >= num_col )
            throw error("column index out of range");
        return cols[i];
    }

    boost::shared_ptr<detail::column_base> get_column(int i)
    {
        return get_odbc_column(i);
    }

    boost::shared_ptr<detail::column_base> get_column(std::string const & n)
    {
        for ( int i = 0; i < num_col; ++i )
        {
            if ( cols[i]->name() == n )
                return cols[i];
        }
        throw error(n + ": inexistent column");
    }
    
    std::auto_ptr<detail::row_base> create_row();

private:
    boost::shared_ptr<column> create_column(int n);

    result_set & operator= (result_set const &);

    statement const & stmt;
    SQLSMALLINT num_col;
    std::vector<boost::shared_ptr<column> > cols;
};

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_RESULT_SET_HPP)
