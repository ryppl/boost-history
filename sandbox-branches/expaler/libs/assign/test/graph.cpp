/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */
 
#include <iostream>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map.hpp>
#include <string>

using namespace std;
using namespace boost;

#include <boost/assign/graph.hpp>

using namespace boost::assignment;

enum vertex_first_name_t { vertex_first_name };
namespace boost 
{
    BOOST_INSTALL_PROPERTY(vertex, first_name);
}
    

int main()
{
    
   // Create the graph, and specify that we will use std::string to
    // store the first name's.
    typedef property < edge_weight_t, double > Weight,W;  
    typedef adjacency_list<vecS, vecS, directedS, no_property, Weight > MyGraphType;

//    typedef pair<int,int> Pair;
//    Pair edge_array[11] = { Pair(0,1), Pair(0,2), Pair(0,3), Pair(0,4), 
//                            Pair(2,0), Pair(3,0), Pair(2,4), Pair(3,1), 
//                            Pair(3,4), Pair(4,0), Pair(4,1) };
    
    MyGraphType G(5);
    insert( G )(4,6);
    insert( G )(1,2, W(1.2) )(2,3, W(3.4) );
        
    return 0;
}
