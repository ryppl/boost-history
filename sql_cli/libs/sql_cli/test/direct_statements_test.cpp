//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2007. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include "test.hpp"

using namespace boost::sql_cli::test;

int test1(boost::sql_cli::connection & conn)
{
    table t(conn, "t", "a int, b varchar(10), c double precision");
    boost::sql_cli::statement st(conn);
    st.exec("insert into t ( a, b, c ) values ( 1, 'two', 3.0 )");
    conn.commit();
    return 0;
}

int test_main( int, char *[] )
{
    test_suite ts;
    ts.subscribe(test1);
    return ts.run();
}
