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
# pragma once
#endif

#include <boost/graph/adjacency_list.hpp>
#include <boost/assign/graph/detail/tuple_inserter.hpp>

namespace boost
{
    namespace assignment
    {
        template< typename EL, typename VL, typename D, 
                  typename VP, typename EP, typename GP, typename Tuple >
            inline void make_insertion( adjacency_list<EL,VL,D,VP,EP,GP>& g, const Tuple& t ) 
        { 
            detail::make_insertion_impl< tuples::length<Tuple>::value >()( g, t ); 
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
