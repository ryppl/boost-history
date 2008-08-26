// mpl_graph - defines a metadata implementation of the BGL immutable graph concepts

// (c) 2008 Gordon Woodhull 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSEmpl::_1_0.txt or copy at
// http://www.boost.org/LICENSEmpl::_1_0.txt)

#ifndef BOOST_MPLGRAPH_H
#define BOOST_MPLGRAPH_H

#include <boost/mpl/map.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/type_traits.hpp>


namespace boost {
namespace metagraph {
namespace mpl_graph {

// idea is to lazily produce maps and other metadata structures
// for looking up the various things U want to look up thru bgl-like interfaces

// currently Edge types must be unique but it might make sense to make them
// unique per Source and/or Target (which would require a different edge_descriptor)

// also this doesn't provide for vertices with no edges 

// edgeseq_graph takes an mpl sequence of sequences <Edge,Source,Target> and wraps it
// so that the rest of the code knows what it is
// edgeseq_graph doesn't do anything but label the data as usable by the metafunctions below
// and provide indirection for other possible input formats (?)
template<typename EdgeSequence>
struct edgeseq_graph {
    typedef EdgeSequence est_sequence;
};

namespace detail {
    // clarifiers
    template<typename EST> struct fetch_edge : 
        mpl::front<EST> {};
    template<typename EST> struct fetch_source : 
        mpl::deref<typename mpl::next<typename mpl::begin<EST>::type>::type> {};
    template<typename EST> struct fetch_target : 
        mpl::back<EST> {};

//#define MPL_GRAPH_PRODUCE_OUTMAP_AS_MAP
#ifdef MPL_GRAPH_PRODUCE_OUTMAP_AS_MAP
        // this implementation didn't work on msvc - anyway not sure if it's more efficient
        // to build all maps at once at the expense of lots of map updates, or to use
        // the many pass, easier-to-read filter-and-build algs below.
    // S->E->T map for out_*, adjacent_vertices
    template<typename EdgeSeqGraph>
    struct produce_outs_map :
        mpl::fold<typename EdgeSeqGraph::est_sequence,
                  mpl::map<>,
                  mpl::insert<mpl::_1,
                              mpl::pair<fetch_source<mpl::_2>,
                                        mpl::insert<mpl::if_<mpl::has_key<mpl::_1,fetch_source<mpl::_2> >,
                                                             mpl::at<mpl::_1,fetch_source<mpl::_2> >,
                                                             mpl::map<> >,
                                                    mpl::pair<fetch_edge<mpl::_2>, fetch_target<mpl::_2> > > > > >
    {};
    template<typename S, typename EdgeSeqGraph>
    struct produce_out_map :
        mpl::at<typename produce_outs_map<EdgeSeqGraph>::type, S>
    {};
#else // produce out map via a filtered  
    // E->T map for an S for out_*, adjacent_vertices
    template<typename S, typename EdgeSeqGraph>
    struct produce_out_map :
        mpl::fold<typename mpl::filter_view<typename EdgeSeqGraph::est_sequence, boost::is_same<fetch_source<mpl::_1>,S> >::type,
             mpl::map<>,
             mpl::insert<mpl::_1,mpl::pair<fetch_edge<mpl::_2>,fetch_target<mpl::_2> > > >
    {};
#endif

    // E->S map for a T for in_*, degree
    template<typename T, typename EdgeSeqGraph>
    struct produce_in_map :
        mpl::fold<typename mpl::filter_view<typename EdgeSeqGraph::est_sequence, 
                                            boost::is_same<fetch_target<mpl::_1>,T> >::type,
             mpl::map<>,
             mpl::insert<mpl::_1,mpl::pair<fetch_edge<mpl::_2>,fetch_source<mpl::_2> > > >

    {};
    // E->pair<S,T> map for source, target
    template<typename EdgeSeqGraph>
    struct produce_edge_st_map :
        mpl::fold<typename EdgeSeqGraph::est_sequence,
             mpl::map<>,
             mpl::insert<mpl::_1,mpl::pair<fetch_edge<mpl::_2>,
                            mpl::pair<fetch_source<mpl::_2>, 
                                 fetch_target<mpl::_2> > > > >
    {};
    // Vertex set for VertexListGraph
    template<typename EdgeSeqGraph>
    struct produce_vertex_set :
        mpl::fold<typename EdgeSeqGraph::est_sequence,
             typename mpl::fold<typename EdgeSeqGraph::est_sequence,
                           mpl::set<>,
                           mpl::insert<mpl::_1,fetch_target<mpl::_2> >
                           >::type,
             mpl::insert<mpl::_1, fetch_source<mpl::_2> > >
    {};
    // Edge set for EdgeListGraph
    template<typename EdgeSeqGraph>
    struct produce_edge_set :
        mpl::fold<typename EdgeSeqGraph::est_sequence,
            mpl::set<>,
            mpl::insert<mpl::_1,fetch_edge<mpl::_2> > >
    {};
}

// Boost Graph concepts, MPL style

// IncidenceGraph
template<typename E, typename G>
struct source : 
    mpl::first<typename mpl::at<typename detail::produce_edge_st_map<G>::type,E>::type> 
{};
template<typename E, typename G>
struct target : 
    mpl::second<typename mpl::at<typename detail::produce_edge_st_map<G>::type,E>::type> 
{};
template<typename U, typename G>
struct out_edges :
    mpl::fold<typename detail::produce_out_map<U,G>::type,
         mpl::vector<>,
         mpl::push_back<mpl::_1, mpl::first<mpl::_2> > >
{};
template<typename U, typename G>
struct out_degree : 
    mpl::size<typename out_edges<U,G>::type>
{};

// BidirectionalGraph
template<typename V, typename G>
struct in_edges :
    mpl::fold<typename detail::produce_in_map<V,G>::type,
         mpl::vector<>,
         mpl::push_back<mpl::_1, mpl::first<mpl::_2> > >
{};
template<typename V, typename G>
struct in_degree :
    mpl::size<typename in_edges<V,G>::type>
{};
template<typename V, typename G>
struct degree :
    mpl::plus<typename out_degree<V,G>::type,typename in_degree<V,G>::type>
{};

// AdjacencyGraph 
template<typename V, typename G>
struct adjacent_vertices :
    mpl::transform<typename detail::produce_out_map<V,G>::type,
              mpl::second<mpl::_1>,
              mpl::back_inserter<mpl::vector<> > >
{};

// VertexListGraph
template<typename G>
struct vertices :
    detail::produce_vertex_set<G>
{};
template<typename G>
struct num_vertices :
    mpl::size<typename vertices<G>::type>
{};

// EdgeListGraph
template<typename G>
struct edges :
    detail::produce_edge_set<G>
{};
template<typename G>
struct num_edges :
    mpl::size<typename edges<G>::type>
{};
// source and target are defined in IncidenceGraph

} // mplgraph
} // metagraph
} // boost

#endif // BOOST_MPLGRAPH_H
