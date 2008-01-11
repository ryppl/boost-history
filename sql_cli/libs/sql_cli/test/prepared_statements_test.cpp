//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2007.
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <cmath>
#include <iostream>
#include <ostream>

#include "test.hpp"

using namespace boost::sql_cli::test;

struct record
{
    record(long aa, char const * bb, double cc) : a(aa), b(bb), c(cc) {}

    long a;
    std::string b;
    double c;
};

char * nums[] = { "zero", "one", "two", "three", "four", "five", "six",
        "seven", "eight", "nine" }; 

int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t", "a int, b varchar(10), c double precision");

    typedef std::vector<record> data_vect;
    data_vect data;
    for ( long l = 0; l < 10; ++l )
    {
        data.push_back(record(l, nums[l], l * 1.1));
    }

    long a;
    std::string b;
    double c;
    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( a, b, c ) values ( ?, ?, ? )", a, b, c);
    for ( data_vect::iterator dvi = data.begin(); dvi != data.end(); ++dvi )
    {
        a = dvi->a;
        b = dvi->b; 
        c = dvi->c;
        st.exec();
    }
    conn.commit();

    st.exec("select * from t");
    boost::sql_cli::result_set rs(st);
    data_vect::iterator dvi = data.begin();
    for ( boost::sql_cli::result_set::iterator rsi = rs.begin(); 
            rsi != rs.end(); ++rsi )
    {
        if ( dvi == data.end() )
            throw std::runtime_error("Too many rows retrieved");
        BOOST_CHECK( (*rsi)[0].get<long>() == dvi->a );
        BOOST_CHECK( (*rsi)["b"].get<std::string>() == dvi->b );
        BOOST_CHECK(compare( (*rsi)[2].get<double>(), dvi->c ));
        ++dvi;
    }

    return 0;
}

int test_main( int, char *[] )
{
    test_suite ts;
    ts.subscribe(test1);
    return ts.run();
}
