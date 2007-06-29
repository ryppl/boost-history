// mutable_vertex_hds_concept.t.cpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a concept-checking class.  We
// proceed with the standard test plan for such a class.
//
//@TEST_PLAN:  Create a (loose) archetype of the concept, and verify
// (using the Boost.ConceptCheck library macros) that it satisfies all
// the concept requirements of the concept.  Note that the purpose of
// this test driver is to verify the validity of the concept checking
// class, not of the archetype.  This is why it suffices to take the
// 'halfedge_descriptor' as an 'int', although a real archetype would make
// this into a custom-made class with the tailored minimal requirements.

#include <boost/hdstl/hds_concepts/mutable_vertex_hds_concept.hpp>
#include <boost/test/minimal.hpp>
#include <boost/concept_check.hpp>

namespace hdstl1 {

    struct mutable_vertex_hds_archetype {
        // This 'struct', intentionally defined in a namespace different from
        // 'hdstl', the 'hds_traits' specialization defined in the namespace
        // 'hdstl', and the supporting function 'opposite', defined in the same
        // namespace as this 'struct' to take advantage of ADL
        // (argument-dependent lookup) provide an archetype for the 'HDS'
        // concept.

        typedef int halfedge_descriptor;
        typedef int vertex_descriptor;
    };

    mutable_vertex_hds_archetype::halfedge_descriptor
    opposite(mutable_vertex_hds_archetype::halfedge_descriptor h,  
                              const mutable_vertex_hds_archetype&)
    {
        return h;
    }
    
    mutable_vertex_hds_archetype::vertex_descriptor
    vertex(mutable_vertex_hds_archetype::halfedge_descriptor,  
                           const mutable_vertex_hds_archetype&)
    {
        return mutable_vertex_hds_archetype::vertex_descriptor();
    }

    mutable_vertex_hds_archetype::halfedge_descriptor
    new_edge(mutable_vertex_hds_archetype&)
    {
        return mutable_vertex_hds_archetype::halfedge_descriptor();
    }

    void
    delete_edge(mutable_vertex_hds_archetype::halfedge_descriptor,
                                     mutable_vertex_hds_archetype&)
    { 
    }
    
    void
    set_vertex(mutable_vertex_hds_archetype::halfedge_descriptor,
              mutable_vertex_hds_archetype::vertex_descriptor,
             mutable_vertex_hds_archetype&)
    { 
    }
    
    mutable_vertex_hds_archetype::vertex_descriptor
    new_vertex(mutable_vertex_hds_archetype&)
    { 
        return mutable_vertex_hds_archetype::vertex_descriptor();
    }

    void
    delete_vertex(mutable_vertex_hds_archetype::vertex_descriptor,
                                   mutable_vertex_hds_archetype&)
    { 
    }
    
    mutable_vertex_hds_archetype::vertex_descriptor
    source(mutable_vertex_hds_archetype::halfedge_descriptor,
             const mutable_vertex_hds_archetype&)
    {
        return mutable_vertex_hds_archetype::vertex_descriptor();
    }

    mutable_vertex_hds_archetype::vertex_descriptor
    target(mutable_vertex_hds_archetype::halfedge_descriptor,
             const mutable_vertex_hds_archetype&)
    {
        return mutable_vertex_hds_archetype::vertex_descriptor();
    }

}  // namespace hdstl1

namespace boost {
namespace hdstl {
    
    template <>
    struct hds_traits<hdstl1::mutable_vertex_hds_archetype>
    {
        typedef hdstl1::mutable_vertex_hds_archetype::halfedge_descriptor
                                             halfedge_descriptor;
        typedef hdstl1::mutable_vertex_hds_archetype::vertex_descriptor
                                             vertex_descriptor;
        enum { supports_vertices = true};
        static const int supports_facets = false;
    };
    
}  // namespace hdstl
}  // namespace boost

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

template <class HDS>
struct class_concept_requirements
{
    BOOST_CLASS_REQUIRE(HDS, boost::hdstl::concepts, MutableVertexHDSConcept);
};

template <class HDS>
bool concept_requirements()
{
    boost::function_requires<boost::hdstl::concepts::MutableVertexHDSConcept<HDS> >();
    class_concept_requirements<HDS>(); // force instantiation
    return true;
}

int test_main(int, char **)
{
    BOOST_CHECK(( concept_requirements<hdstl1::mutable_vertex_hds_archetype>() ));
    return 0;
}
