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
            signed short ss1,
            signed long sl1,
            signed long long sll1) :
            ss(ss1), 
            sl(sl1),
            sll(sll1)
    {
    }

    signed short ss;
    signed long sl;
    signed long long sll;
};


int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t",
            "ss smallint, "
            "sl int, "
            "sll bigint");

    typedef std::vector<record> data_vect;
    data_vect data;
    for ( int i = 0; i < 10; ++i )
    {
        data.push_back(record(
                static_cast<signed short>( i - 8192 ),
                static_cast<signed long>( i - 1048576l ),
                static_cast<signed long long>( i - 134217728ll ) ));
    }

    signed short ss;
    signed long sl;
    signed long long sll;
    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( ss, sl, sll) "
            "values ( ?, ?, ? )",
            ss,
            sl,
            sll);
    for ( data_vect::iterator dvi = data.begin(); dvi != data.end(); ++dvi )
    {
        ss = dvi->ss;
        sl = dvi->sl;
        sll = dvi->sll;
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
        BOOST_CHECK( (*rsi)["ss"].get<signed short>() == dvi->ss );
        BOOST_CHECK( (*rsi)["sl"].get<signed long>() == dvi->sl );
        BOOST_CHECK( (*rsi)["sll"].get<signed long long>() == dvi->sll );
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
