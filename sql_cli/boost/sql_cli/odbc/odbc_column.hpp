//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_COLUMN_HPP)
#define BOOST_SQL_CLI_ODBC_COLUMN_HPP

#include <string>
#include <typeinfo>

#include <boost/sql_cli/detail/column_base.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

class column : public sql_cli::detail::column_base
{
public:
    column(std::string const & nm, int t, int s, int p, bool nl, int no) : 
            nam(nm), typ(t), siz(s), prec(p), null(nl), num(no)
    {
    }

    std::string name() const { return nam; }
    std::type_info const & type() const;
    int size() const { return siz; }
    int precision() const { return prec; }
    bool nullable() const { return null; }
    int number() const { return num; }

private:
    std::string nam;
    int typ;
    int siz; 
    int prec;
    bool null;
    int num;
};

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_COLUMN_HPP)
