//
// short: Test cases for container algortihms.
//
//    

// (C) Copyright Thorsten Ottosen 2002-2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.


#include "boost/sequence_algo/container_algo.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
    using namespace std;
    using namespace boost;

    ifstream in_file( "container_algo.cpp" );
    ofstream out_file( "container_algo_sorted.cpp" );   

    istream_iterator< string >    is( in_file );
    typedef vector< string >      vec_t;
    vec_t                         text;
    ostream_iterator< string >    os( out_file, " " );

    copy( is, back_inserter( text ) );
    ext::sort( text );
    copy( text, os );
    
    return 0;
}
    
