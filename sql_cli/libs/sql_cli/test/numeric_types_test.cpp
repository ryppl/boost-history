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
            float f1,
            double d1
            ) :
            f(f1),
            d(d1)
    {
    }

    float f;
    double d;
};


int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t",
            "f real, "
            "d double precision"
            );

    typedef std::vector<record> data_vect;
    data_vect data;
    for ( int i = 0; i < 10; ++i )
    {
        data.push_back(record(
                static_cast<float>( i + 1.1e-20 ),
                static_cast<double>( i - 1.1e200 )
                ));
    }

    float f;
    double d;
    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( f, d ) "
            "values ( ?, ? )",
            f,
            d
            );
    for ( data_vect::iterator dvi = data.begin(); dvi != data.end(); ++dvi )
    {
        f = dvi->f;
        d = dvi->d;
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
        BOOST_CHECK(compare( (*rsi)["f"].get<float>(), dvi->f ));
        BOOST_CHECK(compare( (*rsi)["d"].get<double>(), dvi->d ));
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
