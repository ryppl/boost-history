#ifndef BOOST_HDSTL_HDS_TRAITS_HPP
#define BOOST_HDSTL_HDS_TRAITS_HPP 1

namespace boost{
namespace hdstl {

    template <typename HDS>
    struct hds_traits{

        typedef typename HDS::vertex_descriptor         vertex_descriptor;
        typedef typename HDS::halfedge_descriptor       halfedge_descriptor;
        typedef typename HDS::facet_descriptor          facet_descriptor;

        typedef typename HDS::vertex_iterator           vertex_iterator;
        typedef typename HDS::halfedge_iterator         halfedge_iterator;
        typedef typename HDS::facet_iterator            facet_iterator;

        typedef typename HDS::const_vertex_iterator     
            const_vertex_iterator;
        typedef typename HDS::const_halfedge_iterator   
            const_halfedge_iterator;
        typedef typename HDS::const_facet_iterator      
            const_facet_iterator;

        typedef typename HDS::size_type                 size_type;

        typedef typename HDS::traversal_category        traversal_category; 
        typedef typename HDS::forward_category          forward_category;
        typedef typename HDS::backward_category         backward_category;
    };

    struct forward_traversal_tag {};
    struct backward_traversal_tag {};

    struct next_in_facet_tag {};
    struct next_at_source_tag {};
    struct next_at_target_tag {};
    
    struct prev_in_facet_tag {};
    struct prev_at_source_tag {};
    struct prev_at_target_tag {};

} // namespace hdstl
} // namespace boost

#endif

