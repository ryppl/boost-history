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

#ifndef BOOST_ASSIGN_GRAPH_ADJACENCY_LIST_HPP
#define BOOST_ASSIGN_GRAPH_ADJACENCY_LIST_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

#include <boost/graph/adjacency_list.hpp>

namespace boost
{
namespace assignment
{
    template< typename EL, typename VL, typename D, 
    typename VP, typename EP, typename GP, typename Vertex >
    inline void make_tuple_insertion( adjacency_list<EL,VL,D,VP,EP,GP>& g, 
                                      const Vertex& v, const Vertex& v2 ) 
    { 
        add_edge( v, v2, g );
    }   

    template< typename EL, typename VL, typename D, 
    typename VP, typename EP, typename GP, typename Vertex, typename EdgeProp >
    inline void make_tuple_insertion( adjacency_list<EL,VL,D,VP,EP,GP>& g, 
                                      const Vertex& v, const Vertex& v2, const EdgeProp& ep ) 
    { 
        add_edge( v, v2, ep, g );
    }   

}
}   

#include <boost/assign/tuple_insert_assigner.hpp>        

namespace boost
{
namespace assignment
{

    template< typename EL, typename VL, typename D,
    typename VP, typename EP, typename GP >
    inline tuple_insert_assigner< adjacency_list<EL,VL,D,VP,EP,GP> >
    insert( adjacency_list<EL,VL,D,VP,EP,GP>& graph )
    {
        return tuple_insert_assigner< adjacency_list<EL,VL,D,VP,EP,GP> >( graph );
    }


    template< typename EL, typename VL, typename D,
    typename VP, typename EP, typename GP >
    inline tuple_insert_assigner< adjacency_list<EL,VL,D,VP,EP,GP> >
    assign( adjacency_list<EL,VL,D,VP,EP,GP>& graph )
    {
        graph.clear();
        return tuple_insert_assigner< adjacency_list<EL,VL,D,VP,EP,GP> >( graph );
    }

} // namespave 'assignment'
} // namespace 'boost'

#endif
