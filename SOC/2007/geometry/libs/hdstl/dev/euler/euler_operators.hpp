//euler_operators.hpp   -*- C++ -*-
//
//@PURPOSE: Provide euler operator functions.
//
//@CLASSES:
//   'create_lens':  
//   'delete_lens':
//  'create_torus':  
//   'delete_torus'
//   'split_facet':
//   'join_facets':
//   'split_vertex':
//   'join_vertices':
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION:
//
//  - 'create_lens': Create_lens is an Euler operator whose purpose is to create a topological ball. It makes a new lens
//                   (i.e. a vertex, two halfedges, and two faces) and connects them in the HDS.
//  - 'delete_lens': Delete_lens takes a halfedge which is adjacent to a lens, and reclaims the vertex, the halfedge and
//                   its opposite, and the two adjacent facets. The behaviour is undefined if the halfedge is not 
//                   adjacent to a lens.
//  - 'create_torus': Create_torus is an Euler operator whose purpose is to create a topological torus. The smallest
//                    torus representable in our HDS is a torus split along a major and a minor circle (one vertex, two
//                    edges, one facet with one loop, one shell). This torus can be obtained by gluing
//                    a square along opposite sides (h2 and h4 in figure, top, then the two cycles h1 and h3). Note that
//                    the torus is completely symmetric in h1, h2, h3 or h4. This functions makes such a new toric HDS,
//                    and returns the handle to h1.
//  - 'delete_torus': Delete_torus takes a halfedge which is adjacent to a torus, and reclaims the vertex, the four
//                    halfedges, and the adjacent facet. The behaviour is undefined if the halfedge is not adjacent to a
//                    torus.
//  - 'split_facet':  Split_facet is an Euler operator whose purpose is to insert an edge inside an existing facet,
//                    attach both its endpoints at two (not necessarily distinct) vertices in the boundary, and split
//                    the existing facet into two (one which is a copy of the previous one, and a new one which is
//                    created). If holes are supported, the attach vertices must be both on the outer boundary or on the
//                    same hole boundary; care must be taken to assign the holes of the split facet to either of the new
//                    two facets. In this case, our default behavior is to not assign any hole to the new facet that is
//                    created.  Specifically, h1 and h2 must be consecutive in the facet boundary, same with g1 and g2,
//                    and all four must be adjacent to the same facet f, and on the same outer or inner boundary if
//                    holes are supported. Two halfedges k1 and k2 are created, k1 is stitched as the successor of h1
//                    and predecessor of g2, and k2 becomes the successor of g1 and predecessor of h2; k1 and k2 are
//                    also stitched in the cycles around the two anchor vertices. A new facet f2 is created and made
//                    adjacent to k2 and to all its successors and predecessors.  If holes are supported, all the holes
//                    of f remain assigned to f and none to f2. The halfedge k2 whose incident facet is the new facet f2
//                    is returned. 
//
//  - 'join_facet':   Join_facets is an Euler operator whose purpose is to remove an edge incident to two existing
//                    facet, and delete one of the two facets while extending the boundary of the other to include both
//                    boundary cycles (minus the removed edge). If holes are supported, the two facets must be distinct.
//                    In that case, the new facet contains the holes of both.  Specifically, let f1 be the facet
//                    incident to k1, k2 its opposite and f2 be the facet incident to k2. The boundary cycle of f2 is
//                    traversed starting from k2, and the incident facet is set to f1 for all halfedges in this cycle.
//                    Then, the predecessor h1 of k1 is stitched (in cycle and in facet) with the successor h2 of k2,
//                    and so are the successor g2 of k1, and the predecessor g1 of k2. Finally k1 and k2 are deleted.
//                    This operation is strictly the reverse of split_facet and takes the same running time.

///Usage Example
//--------------
// Suppose we want to build a square shaped halfedge data structure from scratch. First 
// we have to create the initial vertex with the necessary halfedges within data structure
// 'hds', which is correspondent to creating a lens in our data structure:
//
// halfedge_descriptor h1 = create_lens(hds);
//
// create_lens function has two overloads, one requires two facet, two vertex, two halfedge 
// descriptors and the hds. The one we use above creates these necessary vertex and halfedges.
// The create_lens function above creates a vertex 'v1', two halfedges 'h1' and 'h2', which
// are opposites of each other and two facets 'f1' and 'f2'.
//
// In order to complete the square we create 3 more lenses in the hds and link these to 
// each other as the corners of the square as follows:
//
// halfedge_descriptor g1 = create_lens(hds);
// halfedge_descriptor a1 = split_vertex(h1, h2, g1, g2, hds);
//
// halfedge_descriptor i1 = create_lens(hds);
// halfedge_descriptor b1 = split_vertex(g1, a2, i1, i2, hds);
//
// halfedge_descriptor j1 = create_lens(hds);
// halfedge_descriptor c1 = split_vertex(i1, b2, b1, b2, hds);
//
// Now we have four vertices which form an upside down 'U' shape. In order to form the final
// square, we have to split the facet inside this 'U' shape and outside of it. 
//
// halfedge_descriptor d1 = split_facet(h2, a2, j1, c2, hds);
//
// The final data structure looks like this:
//
//..
//   /\\g1             i1//\
//   g2\\/     b1->    _//i2
//      [2]===========[3]
//      /||    <-b2   /||           
//     a1||a2        c2||c1
//       ||/   d2 ->   ||/        
//     _[1]===========[4] _
//     //      <-d1     \\ j1 
//  h1//_ h2          j2_\\     
//..
namespace boost {
namespace hdstl {

template <typename HalfedgeGen>
typename HalfedgeGen::halfedge_descriptor
create_lens(typename HalfedgeGen::facet_descriptor f1, typename HalfedgeGen::facet_descriptor f2
          , typename HalfedgeGen::vertex_descriptor v1, typename HalfedgeGen::vertex_descriptor v2
          , typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds)
{
    typedef typename HalfedgeGen::halfedge_selector::next_tag next_tag;
    typedef typename HalfedgeGen::halfedge_selector::prev_tag prev_tag; 
    typedef typename HalfedgeGen::facet_selector facet_tag;
    typedef typename HalfedgeGen::vertex_selector vertex_tag;

    stitch_next_in_facet(h, h, hds, next_tag());
    stitch_prev_in_facet(h, h, hds, prev_tag());
    
    stitch_next_in_facet(opposite(h, hds), opposite(h, hds), hds, next_tag());
    stitch_prev_in_facet(opposite(h, hds), opposite(h, hds), hds, prev_tag());

    stitch_next_at_source(h, opposite(h,hds), hds, next_tag());
    stitch_prev_at_source(opposite(h,hds), h, hds, prev_tag());
    
    stitch_next_at_source(opposite(h,hds), h, hds, next_tag());
    stitch_prev_at_source(h, opposite(h,hds), hds, prev_tag());

    stitch_next_at_target(h, opposite(h,hds), hds, next_tag());
    stitch_prev_at_target(opposite(h,hds), h, hds, prev_tag());
    
    stitch_next_at_target(opposite(h,hds), h, hds, next_tag());
    stitch_prev_at_target(h, opposite(h,hds), hds, prev_tag());

    stitch_set_facet(h, f1, hds, facet_tag());
    stitch_set_facet(opposite(h), f2, hds, facet_tag());
    stitch_set_vertex(h, v1, hds, vertex_tag());
    stitch_set_vertex(opposite(h), v2, hds, vertex_tag());
}

template <typename HalfedgeGen>
typename HalfedgeGen::halfedge_descriptor
create_lens(typename HalfedgeGen::vertex_descriptor v1, typename HalfedgeGen::vertex_descriptor v2
          , typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds)
{
    typedef typename HalfedgeGen::halfedge_selector::next_tag next_tag;
    typedef typename HalfedgeGen::halfedge_selector::prev_tag prev_tag; 
    typedef typename HalfedgeGen::vertex_selector vertex_tag;

    stitch_next_in_facet(h, h, hds, next_tag());
    stitch_prev_in_facet(h, h, hds, prev_tag());
    
    stitch_next_in_facet(opposite(h, hds), opposite(h, hds), hds, next_tag());
    stitch_prev_in_facet(opposite(h, hds), opposite(h, hds), hds, prev_tag());

    stitch_next_at_source(h, opposite(h,hds), hds, next_tag());
    stitch_prev_at_source(opposite(h,hds), h, hds, prev_tag());
    
    stitch_next_at_source(opposite(h,hds), h, hds, next_tag());
    stitch_prev_at_source(h, opposite(h,hds), hds, prev_tag());

    stitch_next_at_target(h, opposite(h,hds), hds, next_tag());
    stitch_prev_at_target(opposite(h,hds), h, hds, prev_tag());
    
    stitch_next_at_target(opposite(h,hds), h, hds, next_tag());
    stitch_prev_at_target(h, opposite(h,hds), hds, prev_tag());
    
    stitch_set_vertex(h, v1, hds, vertex_tag());
    stitch_set_vertex(opposite(h), v2, hds, vertex_tag());
}

template <typename HalfedgeGen>
typename HalfedgeGen::halfedge_descriptor
create_lens(typename HalfedgeGen::facet_descriptor f1, typename HalfedgeGen::facet_descriptor f2
          , typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds)
{
    typedef typename HalfedgeGen::halfedge_selector::next_tag next_tag;
    typedef typename HalfedgeGen::halfedge_selector::prev_tag prev_tag; 
    typedef typename HalfedgeGen::facet_selector facet_tag;

    stitch_next_in_facet(h, h, hds, next_tag());
    stitch_prev_in_facet(h, h, hds, prev_tag());
    
    stitch_next_in_facet(opposite(h, hds), opposite(h, hds), hds, next_tag());
    stitch_prev_in_facet(opposite(h, hds), opposite(h, hds), hds, prev_tag());

    stitch_next_at_source(h, opposite(h,hds), hds, next_tag());
    stitch_prev_at_source(opposite(h,hds), h, hds, prev_tag());
    
    stitch_next_at_source(opposite(h,hds), h, hds, next_tag());
    stitch_prev_at_source(h, opposite(h,hds), hds, prev_tag());

    stitch_next_at_target(h, opposite(h,hds), hds, next_tag());
    stitch_prev_at_target(opposite(h,hds), h, hds, prev_tag());
    
    stitch_next_at_target(opposite(h,hds), h, hds, next_tag());
    stitch_prev_at_target(h, opposite(h,hds), hds, prev_tag());
    
    stitch_set_facet(h, f1, hds, facet_tag());
    stitch_set_facet(opposite(h), f2, hds, facet_tag());
}

template <typename HalfedgeGen>
typename HalfedgeGen::halfedge_descriptor
create_lens(HalfedgeGen& hds)
{
    // in order to use this method with no parameters except hds, we have to use specialized helpers
    // to create the lens based on the many configuration options such as has vertex, has facet etc...
}

template <typename HalfedgeGen>
void
delete_lens(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds)
{
    delete_vertex(source(h, hds), hds);
    delete_facet(facet(h, hds), hds);
    delete_edge(h, hds);
}

template <typename HalfedgeGen>
typename HalfedgeGen::halfedge_descriptor
create_torus(typename HalfedgeGen::facet_descriptor f, typename HalfedgeGen::vertex_descriptor v 
           , typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g
           , HalfedgeGen& hds)
{
    typedef typename HalfedgeGen::halfedge_selector::next_tag next_tag;
    typedef typename HalfedgeGen::halfedge_selector::prev_tag prev_tag; 
    typedef typename HalfedgeGen::facet_selector facet_tag;
    typedef typename HalfedgeGen::vertex_selector vertex_tag;

    stitch_next_in_facet(h, g, hds, next_tag());
    stitch_prev_in_facet(g, h, hds, prev_tag());

    stitch_next_in_facet(g, opposite(h, hds) , hds, next_tag());
    stitch_prev_in_facet(opposite(h, hds), g , hds, prev_tag());

    stitch_next_in_facet(opposite(h, hds), opposite(g, hds), hds, next_tag());
    stitch_prev_in_facet(opposite(g, hds), opposite(h, hds), hds, prev_tag());

    stitch_next_in_facet(opposite(g, hds), h, hds, next_tag());
    stitch_prev_in_facet(h, opposite(g, hds), hds, prev_tag());

    stitch_next_at_source(h, opposite(g,hds), hds, next_tag());
    stitch_prev_at_source(opposite(g,hds), h, hds, prev_tag());

    stitch_next_at_source(opposite(g,hds), opposite(h,hds), hds, next_tag());
    stitch_prev_at_source(opposite(h,hds), opposite(g,hds), hds, prev_tag());

    stitch_next_at_source(opposite(h,hds), g, hds, next_tag());
    stitch_prev_at_source(opposite(g,hds), h, hds, prev_tag());

    stitch_next_at_source(g, h, hds, next_tag());
    stitch_prev_at_source(h, g, hds, prev_tag());

    stitch_next_at_target(h, opposite(g,hds), hds, next_tag());
    stitch_prev_at_target(opposite(g,hds), h, hds, prev_tag());

    stitch_next_at_target(opposite(g,hds), opposite(h,hds), hds, next_tag());
    stitch_prev_at_target(opposite(h,hds), opposite(g,hds), hds, prev_tag());

    stitch_next_at_target(opposite(h,hds), g, hds, next_tag());
    stitch_prev_at_target(g, opposite(h,hds), hds, prev_tag());

    stitch_next_at_target(g, h, hds, next_tag());
    stitch_prev_at_target(h, g, hds, prev_tag());

    stitch_set_vertex(h, v, hds, vertex_tag());
    stitch_set_vertex(opposite(h, hds), v, hds, vertex_tag());
    stitch_set_vertex(g, v, hds, vertex_tag());
    stitch_set_vertex(opposite(g, hds), v, hds, vertex_tag());
    stitch_set_facet(h, f, hds, facet_tag());
    stitch_set_facet(opposite(h, hds), f, hds, facet_tag());
    stitch_set_facet(g, f, hds, facet_tag());
    stitch_set_facet(opposite(g, hds), f, hds, facet_tag());
}

template <typename HalfedgeGen>
void
delete_torus(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds)
{
    typedef typename HalfedgeGen::halfedge_selector halfedge_selector;
    typename HalfedgeGen::halfedge_descriptor op_h = opposite(h, hds);
    typename HalfedgeGen::halfedge_descriptor g = get_next_in_facet(h, hds, halfedge_selector::is_forward);
    g = get_prev_in_facet(h, hds, halfedge_selector::is_backward);
    typename HalfedgeGen::halfedge_descriptor op_g = opposite(g, hds);

    delete_vertex(source(h, hds), hds);
    delete_facet(facet(h, hds), hds);
    delete_edge(h, hds);
}

} // end namespace hdstl
} // end namespace boost
