//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

//[demo
// An example of what a Boost database interface library might look like

#include <iostream>
#include <ostream>

#define BOOST_ALL_DYN_LINK 1

#include <boost/optional.hpp>
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, <= 0x564)
#   pragma hdrstop
#endif

#include <boost/none.hpp>

#include <boost/sql_cli.hpp>

int main()
{
    try
    {

// Connect to a database, specifying the underlying technology. Unless
// specific SQL syntax is used, this should be the only place where the
// actual DBMS product / technology is evident

        boost::sql_cli::connection conn("ODBC");
        conn.connect("SQL_CLI", "dba", "sql");

// Execute a couple of simple queries

        boost::sql_cli::statement st(conn);
        try
        {
            st.exec("drop table t");
            conn.commit();
        }
        catch ( boost::sql_cli::error & e )
        {
            std::cerr << e.what() << '\n';
        }
        st.exec("create table t ( a int, b varchar(10), c real, "
                "d_is_an_incredibly_long_column_name int )");

// Execute a parameterized query

        long a;
        std::string b;
        boost::optional<float> c;      // a value that may be null
        boost::sql_cli::null_value<long> d; // a value that is always null

        // prepare the statement and bind input parameters, so get to be able
        // to execute it in a loop with different values

        st.prepare("insert into t ( a, b, c, "
                "d_is_an_incredibly_long_column_name ) values ( ?, ?, ?, ? )",
                a, b, c, d);

        char * nums[] = { "zero", "one", "two", "three", "four", "five", "six",
                "seven", "eight", "nine" }; 
        for ( a = 0; a < 10; ++a )
        {
            b = nums[a]; 
            if ( a % 2 )
                c = a * 1.1f;
            else
                c = boost::none;
            st.exec();
        }
        conn.commit();

// Execute a query that returns a result set

        st.exec("select * from t");
        boost::sql_cli::result_set rs(st);

        // Explore the structure of the result set

        std::cout << "No. of columns: " << rs.column_count() << '\n';
        for ( int i = 0; i < rs.column_count(); ++i )
        {
            std::cout << rs[i].name() << "\t " << rs[i].type().name() << '(' << 
                    rs[i].size() << ", " << rs[i].precision() << ')';
            if ( rs[i].nullable() )
                std::cout << "\t null";
            std::cout << '\n';
        }

        // Print the actual results

        std::cout << "\nRows:\n";
        for ( boost::sql_cli::result_set::iterator i = rs.begin(); 
                i != rs.end(); ++i )
        {
            boost::sql_cli::row const & r = *i;

            // Columns may be accessed by position...

            std::cout << r[0].get<long>() << "\t ";
            
            // ...or by name

            std::cout << r["b"].get<std::string>() << "\t ";
            boost::optional<float> c = r[2].get<boost::optional<float> >();
            if ( ! c )
                std::cout << "(null)\t ";
            else
                std::cout << *c << "\t ";
            boost::optional<long> d = r[3].get<boost::optional<long> >();
            if ( ! d )
                std::cout << "(null)\n";
            else
                std::cout << *d << '\n';
        }
        st.exec("drop table t");
        conn.commit();
        conn.disconnect();
    }
    catch ( boost::sql_cli::error & e )
    {
        std::cerr << e.what() << '\n';
        std::cout << "Failure!\n";
        return 1;
    }
    std::cout << "Success!\n";
    return 0;
}

//]

