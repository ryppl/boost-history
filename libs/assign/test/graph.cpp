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
 
#include <boost/assign/graph.hpp>
#include <boost/graph/properties.hpp>
#include <string>

struct first_vertex_name_t { typedef vertex_property_tag kind; };
using namespace boost;
using namespace boost::assignment;

int main()
{

    typedef property<first_vertex_name_t, std::string> FirstNameProperty;
    typedef adjacency_list<vecS, vecS, directedS, 
                           FirstNameProperty> MyGraphType;

  typedef pair<int,int> Pair;
  Pair edge_array[11] = { Pair(0,1), Pair(0,2), Pair(0,3), 
                          Pair(0,4), Pair(2,0), Pair(3,0), 
                          Pair(2,4), Pair(3,1), Pair(3,4), 
                          Pair(4,0), Pair(4,1) };
    
  MyGraphType G(5);
  for (int i = 0; i < 11; ++i)
    add_edge(edge_array[i].first, edge_array[i].second, G);

  property_map<MyGraphType, first_vertex_name_t>::type
    name = get(first_vertex_name_t(), G);

}
