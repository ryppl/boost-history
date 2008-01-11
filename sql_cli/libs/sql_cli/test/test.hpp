//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2007.
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_TEST_TEST_HPP)
#define BOOST_SQL_CLI_TEST_TEST_HPP

#include <cmath>
#include <vector>

#include <boost/function.hpp>
#include <boost/test/minimal.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/sql_cli.hpp>

namespace boost
{
namespace sql_cli
{
namespace test
{

class test_suite
{
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
        typedef boost::function1<int, boost::sql_cli::connection &> func_type;
#else
        typedef boost::function<int(boost::sql_cli::connection &)> func_type;
#endif
        typedef std::vector<func_type> func_vect;

    public:
        test_suite() : conn("ODBC")
        {
            conn.connect("SQL_CLI", "dba", "sql");
        }

        void subscribe(func_type f)
        {
            funcs.push_back(f);
        }

        int run()
        {
            int res = 0;
            for ( func_vect::iterator i = funcs.begin(); i != funcs.end(); ++i )
            {
                int r = (*i)(conn);
                if ( r > res )
                    res = r;
            }
            return res;
        }

    private:
        boost::sql_cli::connection conn;
        func_vect funcs;
};

class table
{
    public:
        table(boost::sql_cli::connection & c, std::string const & n,
                std::string const & desc) : 
                conn(c), stmt(c), name(n)
        {
            try
            {
                stmt.exec("drop table " + name);
                conn.commit();
            }
            catch ( boost::sql_cli::error & )
            {
            }
            stmt.exec("create table " + name + " ( " + desc + " )");
        }

        ~table()
        {
            stmt.exec("drop table " + name);
            conn.commit();
        }

    private:
        boost::sql_cli::connection & conn;
        boost::sql_cli::statement stmt;
        std::string name;
};

template <typename FloatT>
bool compare(FloatT first, FloatT second)
{
    int e1, e2;
    std::frexp(first, &e1);
    std::frexp(second, &e2);
    return std::abs(first - second) < std::ldexp(0.1, e1 + e2 / 2);
}

} // namespace test
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_TEST_TEST_HPP)

