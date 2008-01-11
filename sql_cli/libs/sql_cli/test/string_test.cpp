//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2007.
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <string>

#include "test.hpp"

using namespace boost::sql_cli::test;

char * nums[] = { "zero", "one", "two", "three", "four", "five", "six",
        "seven", "eight", "nine" }; 

struct record
{
    record(char const * s1) : s(s1)
    {
    }

    std::string s;
};


int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t", "s varchar(6)");

    typedef std::vector<record> data_vect;
    data_vect data;
    for ( int i = 0; i < 10; ++i )
    {
        data.push_back(record(nums[i]));
    }

    std::string s;
    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( s ) values ( ? )", s);
    for ( data_vect::iterator dvi = data.begin(); dvi != data.end(); ++dvi )
    {
        s = dvi->s;
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
        BOOST_CHECK( (*rsi)["s"].get<std::string>() == dvi->s );
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
