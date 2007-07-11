//halfedge_ds.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a concrete bu parameterizable implementation of HDS.
//
//@CLASSES:
//  'halfedge_ds_gen':  defines configuration options in a central place
//  'container_gen':  class for selecting the storage type of 'halfedge_ds'
//  'opposite_helper':  class for configuring opposite access methods
//  'halfedge_ds':  swiss-army knife of HDS implementations
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides a single class template, 'halfedge_ds', which
// accepts many template parameters that can be used to govern its
// implementation.  Most of these parameters can be chosen as selectors (see
// 'hds_selectors' component).
//
///Definition
//-----------
// - 'halfedge_ds_gen' this class defines the 'config' subtype and keeps
//   the configuration parameters of the 'HDS' in a central place. The 
//   configurations are obtained based on selectors 'HalfedgeS', 'VertexS', 
//   and 'FacetS'. 
// - 'opposite_helper' this class configures the opposite access method, based
//   on the storage type. Do not use and opposite method and manually calculates
//   the opposite if the storage is 'stl::vector' type, otherwise uses the 
//   defined 'opposite' member.
// - 'halfedge_ds' concerete implementation of the 'HDS'.  

#ifndef BOOST_HDSTL_HALFEDGE_DS_HPP
#define BOOST_HDSTL_HALFEDGE_DS_HPP 1

#include <boost/pending/ct_if.hpp>

namespace boost {
namespace hdstl {
template <typename HalfedgeS, typename VertexS, typename FacetS>
struct halfedge_ds_gen {
    struct config {
        typedef  boost::ct_if_t<mpl::equal<HalfedgeS::container_type, vecS>,
                           false_type, true_type> halfedge_has_opposite_member;
        typedef HalfedgeS::is_forward is_forward;
        typedef HalfedgeS::is_backward is_backward;
        typedef HalfedgeS::is_bidir is_bidir;
        typedef HalfedgeS::tag traversal_tag;
            // halfedge config

        typedef boost::ct_if_t<mpl::equal<VertexS,noVertexS>, 
                             false_type, true_type> halfedge_supports_vertices;
        typedef VertexS::is_source is_source;
        typedef VertexS::is_target is_target;
        typedef VertexS::has_vertex_links has_vertex_links;
            // vertex config
        
        typedef boost::ct_if_t<mpl::equal<FacetS,noFacetS>, 
                               false_type, true_type> halfedge_supports_facets;
            // facet config

        
    };
};


template <typename Selector, typename ValueType>
struct container_gen { };

template <typename ValueType>
struct container_gen<listS, ValueType> {
    typedef std::list<ValueType> type;
    typedef false_type is_rand_access;
};

template <typename ValueType>
struct container_gen<vecS, ValueType> {
    typedef std::vector<ValueType> type;
    typedef true_type is_rand_access;
};

} // namespace detail


template<typename Selector, typename HalfedgeDescriptor>
struct opposite_helper {
    inline static
    HalfedgeDescriptor&
    opposite(HalfedgeDescriptor& h)
    {
        // halfedges come in pairs, point to each other as
        // opposite. If halfedges stored in a list, use the
        // opposite pointer.
        return h->opposite_;
    }
};

template<typename HalfedgeDescriptor>
struct opposite_helper<vecS, HalfedgeDescriptor> {
    inline static
    HalfedgeSescriptor&
    opposite(HalfedgeDescriptor& h)
    {
        // halfedges come in pairs, point to each other as
        // opposite. If halfedges stored in a vector:
        if (h%2==0)
            return h+1;
        else
            return h-1;
    }


template <typename HalfedgeS = halfedgeS<vecS, forwardS<next_in_facet_tag> >,
          typename VertexS = vertexS<vecS, vertexLinkS, sourceS>,
          typename FacetS = facetS<vecS> >
class halfedge_ds
{
    typedef void* halfedge_ptr; 

    typedef typename  container_gen<HalfedgeS, halfedge_ptr
                                                   >::is_random is_rand_access;

    typedef typename boost::ct_if_t<is_rand_access,
                          std::size_t, halfedge_ptr>::type halfedge_descriptor;

    typedef typename  container_gen<HalfedgeS, halfedge_ptr>::type 
                                                                  HalfedgeList;

    typedef typename HalfedgeList::iterator halfedge_iterator;

};

} // namespace hdstl
} // namespace boost

#endif
