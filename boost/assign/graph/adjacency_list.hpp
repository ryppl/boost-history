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

#include "../add_edge_assigner.hpp"
#include <boost/graph/adjacency_list.hpp>

namespace boost
{
    namespace assignment
    {
        template< typename EL, typename VL, typename D, 
                  typename VP, typename EP, typename GP >
        inline detail::add_edge_assigner< adjacency_list<EL,VL,D,VP,EP,GP> >
        assign( adjacency_list<EL,VL,D,VP,EP,GP>& graph )
        {
            return 
                detail::add_edge_assigner< adjacency_list<EL,VL,D,VP,EP,GP> >( graph ); 
        }
    }
}

#endif
