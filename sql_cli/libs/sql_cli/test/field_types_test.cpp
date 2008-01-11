//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2007.
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <cmath>
#include <iostream>
#include <ostream>
#include <string>

#include "test.hpp"

using namespace boost::sql_cli::test;

int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t",
            "sc tinyint, "
            "uc unsigned tinyint, "
            "ss smallint, "
            "us unsigned smallint, "
            "sl int, "
            "ul unsigned int, "
            "sll bigint, "
            "ull unsigned bigint, "
            "f real, "
            "d double precision "
            );


    signed char sc = -64;
    unsigned char uc = 64;
    signed short ss = -8192;
    unsigned short us = 8192;
    signed long sl = -1048576l;
    unsigned long ul = 1048576ul;
    signed long long sll = -134217728ll;
    unsigned long long ull = 134217728ull;
    float f = 1.1e-20f;
    double d = -1.1e200;

    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( "
            "sc, "
            "uc, "
            "ss, "
            "us, "
            "sl, "
            "ul, "
            "sll, "
            "ull, "
            "f, "
            "d "
            ") values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )", 
            sc, 
            uc,
            ss,
            us,
            sl,
            ul,
            sll,
            ull,
            f,
            d
            );
    st.exec();
    conn.commit();

    st.exec("select * from t");
    boost::sql_cli::result_set rs(st);

    BOOST_CHECK(rs[0].type() == typeid(signed char));
    BOOST_CHECK(rs[1].type() == typeid(unsigned char));
    BOOST_CHECK(rs[2].type() == typeid(signed short));
    BOOST_CHECK(rs[3].type() == typeid(unsigned short));
    BOOST_CHECK(rs[4].type() == typeid(signed long));
    BOOST_CHECK(rs[5].type() == typeid(unsigned long));
    BOOST_CHECK(rs[6].type() == typeid(signed long long));
    BOOST_CHECK(rs[7].type() == typeid(unsigned long long));
    BOOST_CHECK(rs[8].type() == typeid(float));
    BOOST_CHECK(rs[9].type() == typeid(double));

    return 0;
}

int test2(boost::sql_cli::connection & conn)
{
    table t(conn, "t",
            "s varchar(6), "
            "w varwchar(6) "
            );


    std::string s("narrow");
    std::wstring w(L"wide");

    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( "
            "s, "
            "w "
            ") values ( ?, ? )", 
            s,
            w
            );
    st.exec();
    conn.commit();

    st.exec("select * from t");
    boost::sql_cli::result_set rs(st);

    BOOST_CHECK(rs[0].type() == typeid(std::string));
    BOOST_CHECK(rs[1].type() == typeid(std::wstring));

    return 0;
}

int test_main( int, char *[] )
{
    test_suite ts;
    ts.subscribe(test1);
    ts.subscribe(test2);
    return ts.run();
}
