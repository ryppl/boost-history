//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2007.
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <string>

#include "test.hpp"

using namespace boost::sql_cli::test;

wchar_t * wnums[] = { L"zero", L"one", L"two", L"three", L"four", L"five",
        L"six", L"seven", L"eight", L"nine" }; 

struct record
{
    record(wchar_t const * w1) : w(w1)
    {
    }

    std::wstring w;
};


int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t", "w varwchar(6)");

    typedef std::vector<record> data_vect;
    data_vect data;
    for ( int i = 0; i < 10; ++i )
    {
        data.push_back(record(wnums[i]));
    }

    std::wstring w;
    boost::sql_cli::statement st(conn);
    st.prepare("insert into t ( w ) values ( ? )", w);
    for ( data_vect::iterator dvi = data.begin(); dvi != data.end(); ++dvi )
    {
        w = dvi->w;
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
        BOOST_CHECK( (*rsi)["w"].get<std::wstring>() == dvi->w );
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
