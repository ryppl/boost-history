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

#include "../add_edge_assigner.hpp"
#include <boost/graph/adjacency_matrix.hpp>

namespace boost
{
    namespace assignment
    {
        template< typename D, typename VP, 
                  typename EP, typename GP, typename A >
        inline detail::add_edge_assigner< adjacency_matrix<D,VP,EP,GP,A> >
        assign( adjacency_matrix<D,VP,EP,GP,A>& graph )
        {
            return 
                detail::add_edge_assigner< adjacency_matrix<D,VP,EP,GP,A> >( graph );
        }
    }
}

#endif
