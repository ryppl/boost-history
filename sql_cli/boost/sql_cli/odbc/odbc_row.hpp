//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_ROW_HPP)
#define BOOST_SQL_CLI_ODBC_ROW_HPP

#include <boost/shared_ptr.hpp>

#include <boost/sql_cli/odbc/odbc_field.hpp>
#include <boost/sql_cli/odbc/odbc_result_set.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>
#include <boost/sql_cli/detail/row_base.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

class row : public sql_cli::detail::row_base
{
public:
    row(result_set & rs) : res(rs) 
    {
        for ( int i = 0; i < res.column_count(); ++i )
        {
            boost::shared_ptr<field> f(new field(res.get_statement(), 
                    res.get_odbc_column(i)));
            fields.push_back(f);
        }
    }

    boost::shared_ptr<sql_cli::detail::field_base> get_field(int i)
    {
        if ( i < 0 || i >= res.column_count() )
            throw error("field index out of range");
        return fields[i];
    }

    boost::shared_ptr<sql_cli::detail::field_base> get_field(std::string const & n)
    {
        for ( int i = 0; i < res.column_count(); ++i )
        {
            if ( fields[i]->name() == n )
                return fields[i];
        }
        throw error(n + ": inexistent field");
    }

private:
    row & operator= (row const &);

    result_set & res;
    std::vector<boost::shared_ptr<field> > fields;
};

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_ROW_HPP)
