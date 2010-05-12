#ifndef BOOST_METAGRAPH_MPL_GRAPH_DEPTH_FIRST_SEARCH_HPP_INCLUDED
#define BOOST_METAGRAPH_MPL_GRAPH_DEPTH_FIRST_SEARCH_HPP_INCLUDED

#include <boost/metagraph/mpl_graph/mpl_graph.hpp>

#include <boost/mpl/has_key.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/has_key.hpp>

#include "search_colors.hpp"
#include "search_state_and_operations.hpp"

namespace boost {
namespace metagraph {
namespace mpl_graph {

// dfs takes a visitor which has all the bgl-like metafunctions encapsulated in an 
// "operations" member class, and also a state.  the operations are expected to return a new state
// in addition, the visitor operations are expected to update the colors of vertices
// and need to support a new metafunction get_color<Node, State>

struct dfs_default_visitor_operations {
    template<typename Node, typename Graph, typename State>
    struct initialize_vertex {
        typedef State type;       
    };
    
    template<typename Node, typename Graph, typename State>
    struct discover_vertex {
        typedef State type;       
    };
    
    template<typename Node, typename Graph, typename State>
    struct finish_vertex {
        typedef State type;       
    };
        
    template<typename Edge, typename Graph, typename State>
    struct tree_edge {
        typedef State type;
    };
    
    template<typename Edge, typename Graph, typename State>
    struct back_edge {
        typedef State type;
    };
    
    template<typename Edge, typename Graph, typename State>
    struct forward_or_cross_edge {
        typedef State type;
    };  
};

// visitor is operations, state as described above
// however this returns pair< visitor_state, color_state >
template<typename Graph, typename Node, typename Visitor, 
         typename ColorMap = search_state_and_operations<create_search_color_map::type, search_color_map_operations> >
struct dfs_visit {
    // enter vertex
    typedef typename Visitor::operations::template discover_vertex<Node, Graph, typename Visitor::state>::type discovered_state;
    typedef typename ColorMap::operations::template set_color<Node, search_colors::Gray, typename ColorMap::state>::type discovered_colors;
    // loop over out edges
    typedef typename mpl::template 
        fold<typename mpl_graph::out_edges<Node, Graph>::type, 
             mpl::pair<discovered_state, discovered_colors>,
             mpl::if_<boost::is_same<typename ColorMap::operations::template get_color<mpl_graph::target<mpl::_2, Graph>, mpl::second<mpl::_1 > >, 
                                     search_colors::White>,
                      // unseen target: recurse
                      dfs_visit<Graph, 
                                mpl_graph::target<mpl::_2, Graph>, 
                                search_state_and_operations<typename Visitor::operations::template tree_edge<mpl::_2, Graph, mpl::first<mpl::_1> >, 
                                                     typename Visitor::operations>,
                                search_state_and_operations<mpl::second<mpl::_1>, typename ColorMap::operations> >,
                      // seen: back or forward edge
                      mpl::pair<mpl::if_<boost::is_same<typename ColorMap::operations::template get_color<mpl_graph::target<mpl::_2, Graph>, mpl::second<mpl::_1 > >, search_colors::Gray>,
                                         typename Visitor::operations::template back_edge<mpl::_2, Graph, mpl::first<mpl::_1> >,
                                         typename Visitor::operations::template forward_or_cross_edge<mpl::_2, Graph, mpl::first<mpl::_1> > >,  // Black
                                mpl::second<mpl::_1> >
                      > 
             >::type visited_state_and_colors;
    // leave vertex, and done!
    typedef mpl::pair<typename Visitor::operations::template finish_vertex<Node, Graph, typename mpl::first<visited_state_and_colors>::type >::type,
                      typename ColorMap::operations::template set_color<Node, search_colors::Black, typename mpl::second<visited_state_and_colors>::type>::type> type;
};

template<typename Graph, typename Visitor,
         typename FirstVertex = typename mpl::front<typename mpl_graph::vertices<Graph>::type>::type,
         typename ColorMap = search_state_and_operations<create_search_color_map::type, search_color_map_operations> >
struct depth_first_search :
    mpl::fold<typename mpl_graph::vertices<Graph>::type,
              typename dfs_visit<Graph, FirstVertex, Visitor, ColorMap>::type,
              mpl::if_<boost::is_same<typename ColorMap::operations::template get_color<mpl::_2, mpl::second<mpl::_1> >,
                                      search_colors::White>,
                       dfs_visit<Graph,
                                 mpl::_2,
                                 search_state_and_operations<mpl::first<mpl::_1>, typename Visitor::operations>,
                                 search_state_and_operations<mpl::second<mpl::_1>, typename ColorMap::operations> >,
                       mpl::_1> >   
{};

} // namespace mpl_graph
} // namespace metagraph
} // namespace boost


#endif // BOOST_METAGRAPH_MPL_GRAPH_DEPTH_FIRST_SEARCH_HPP_INCLUDED
