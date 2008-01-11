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
    record(signed char sc1, unsigned char uc1) : sc(sc1), uc(uc1)
    {
    }

    signed char sc;
    unsigned char uc;
};


int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t", "sc tinyint, uc unsigned tinyint");

    typedef std::vector<record> data_vect;
    data_vect data;
    for ( int i = 0; i < 10; ++i )
    {
        data.push_back(record(
                static_cast<signed char>( i - 64 ),
                static_cast<unsigned char>( i + 64 ) ));
    }

    signed char sc;
    unsigned char uc;
    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( sc, uc ) values ( ?, ? )", sc, uc);
    for ( data_vect::iterator dvi = data.begin(); dvi != data.end(); ++dvi )
    {
        sc = dvi->sc;
        uc = dvi->uc;
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
        BOOST_CHECK( (*rsi)["sc"].get<signed char>() == dvi->sc );
        BOOST_CHECK( (*rsi)["uc"].get<unsigned char>() == dvi->uc );
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
