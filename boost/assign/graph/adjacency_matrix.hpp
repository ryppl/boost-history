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

#ifndef BOOST_ASSIGN_GRAPH_ADJACENCY_MATRIX_HPP
#define BOOST_ASSIGN_GRAPH_ADJACENCY_MATRIX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/graph/adjacency_matrix.hpp>
#include <boost/assign/graph/detail/tuple_inserter.hpp>

namespace boost
{
namespace assignment
{
    template< typename D, typename VP, typename EP,
              typename GP, typename A, typename Tuple >
    inline void make_insertion( adjacency_matrix<D,VP,EP,GP,A>& g, const Tuple& t )
    { 
        detail::make_graph_insertion_impl< tuples::length<Tuple>::value >()( g, t ); 
    }   
}
}   

#include <boost/assign/tuple_insert_assigner.hpp>        

namespace boost
{

namespace assignment
{
    template< typename D, typename VP, typename EP,
              typename GP, typename A >
    inline tuple_insert_assigner< adjacency_matrix<D,VP,EP,GP,A> >
    insert( adjacency_matrix<D,VP,EP,GP,A>& graph )
    {
        return tuple_insert_assigner< adjacency_matrix<D,VP,EP,GP,A> >( graph );
    }


    
    template< typename D, typename VP, typename EP,
              typename GP, typename A >
    inline tuple_insert_assigner< adjacency_matrix<D,VP,EP,GP,A> >
    assign( adjacency_matrix<D,VP,EP,GP,A>& graph )
    {
        graph.clear();
        return tuple_insert_assigner< adjacency_matrix<D,VP,EP,GP,A> >( graph );
    }
}

}

#endif
