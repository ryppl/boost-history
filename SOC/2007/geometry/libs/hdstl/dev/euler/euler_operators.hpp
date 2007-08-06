//euler_operators.hpp   -*- C++ -*-
//
//@PURPOSE: Provide euler operator functions.
//
//@CLASSES:
//   'create_lens':  
//   'destroy_lens':
//  'create_torus':  
//   'destroy_torus'
//   'split_facet':
//   'join_facets':
//   'split_vertex':
//   'join_vertices':
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: //
//
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

template <typename HDS>
HDS::halfedge_descriptor
create_lens(typename HDS::facet_descriptor f1, typename HDS::facet_descriptor f2
          , typename HDS::vertex_descriptor v1, typename HDS::vertex_descriptor v2
          , typename HDS::halfedge_descriptor h, HDS& hds)
{

}

} // end namespace hdstl
} // end namespace boost
