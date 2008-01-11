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
    record(
            unsigned short us1,
            unsigned long ul1,
            unsigned long long ull1) :
            us(us1),
            ul(ul1),
            ull(ull1)
    {
    }

    unsigned short us;
    unsigned long ul;
    unsigned long long ull;
};


int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t",
            "us unsigned smallint, "
            "ul unsigned int, "
            "ull unsigned bigint");

    typedef std::vector<record> data_vect;
    data_vect data;
    for ( int i = 0; i < 10; ++i )
    {
        data.push_back(record(
                static_cast<unsigned short>( i + 8192 ),
                static_cast<unsigned long>( i + 1048576ul ),
                static_cast<unsigned long long>( i + 134217728ull ) ));
    }

    unsigned short us;
    unsigned long ul;
    unsigned long long ull;
    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( us, ul, ull) "
            "values ( ?, ?, ? )",
            us,
            ul,
            ull);
    for ( data_vect::iterator dvi = data.begin(); dvi != data.end(); ++dvi )
    {
        us = dvi->us;
        ul = dvi->ul;
        ull = dvi->ull;
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
        BOOST_CHECK( (*rsi)["us"].get<unsigned short>() == dvi->us );
        BOOST_CHECK( (*rsi)["ul"].get<unsigned long>() == dvi->ul );
        BOOST_CHECK( (*rsi)["ull"].get<unsigned long long>() == dvi->ull );
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
