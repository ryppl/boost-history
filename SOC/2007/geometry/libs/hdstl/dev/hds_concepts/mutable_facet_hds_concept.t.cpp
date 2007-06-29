// mutable_facet_hds_concept.t.cpp   -*- C++ -*-
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

#include <boost/hdstl/hds_concepts/mutable_facet_hds_concept.hpp>
#include <boost/test/minimal.hpp>
#include <boost/concept_check.hpp>

namespace hdstl1 {

    struct mutable_facet_hds_archetype {
        // This 'struct', intentionally defined in a namespace different from
        // 'hdstl', the 'hds_traits' specialization defined in the namespace
        // 'hdstl', and the supporting function 'opposite', defined in the same
        // namespace as this 'struct' to take advantage of ADL
        // (argument-dependent lookup) provide an archetype for the 'HDS'
        // concept.

        typedef int halfedge_descriptor;
        typedef int facet_descriptor;
    };

    mutable_facet_hds_archetype::halfedge_descriptor
    opposite(mutable_facet_hds_archetype::halfedge_descriptor h,  
                              const mutable_facet_hds_archetype&)
    {
        return h;
    }
    
    mutable_facet_hds_archetype::facet_descriptor
    facet(mutable_facet_hds_archetype::halfedge_descriptor,  
                           const mutable_facet_hds_archetype&)
    {
        return mutable_facet_hds_archetype::facet_descriptor();
    }

    mutable_facet_hds_archetype::halfedge_descriptor
    new_edge(mutable_facet_hds_archetype&)
    {
        return mutable_facet_hds_archetype::halfedge_descriptor();
    }

    void
    delete_edge(mutable_facet_hds_archetype::halfedge_descriptor,
                                     mutable_facet_hds_archetype&)
    { 
    }
    
    void
    set_facet(mutable_facet_hds_archetype::halfedge_descriptor,
              mutable_facet_hds_archetype::facet_descriptor,
             mutable_facet_hds_archetype&)
    { 
    }
    
    mutable_facet_hds_archetype::facet_descriptor
    new_facet(mutable_facet_hds_archetype&)
    { 
        return mutable_facet_hds_archetype::facet_descriptor();
    }

    void
    delete_facet(mutable_facet_hds_archetype::facet_descriptor,
                                   mutable_facet_hds_archetype&)
    { 
    }

}  // namespace hdstl1

namespace boost {
namespace hdstl {
    
    template <>
    struct hds_traits<hdstl1::mutable_facet_hds_archetype>
    {
        typedef hdstl1::mutable_facet_hds_archetype::halfedge_descriptor
                                             halfedge_descriptor;
        typedef hdstl1::mutable_facet_hds_archetype::facet_descriptor
                                             facet_descriptor;
        enum { supports_vertices = false};
        static const int supports_facets = true;
    };
    
}  // namespace hdstl
}  // namespace boost

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

template <class HDS>
struct class_concept_requirements
{
    BOOST_CLASS_REQUIRE(HDS, boost::hdstl::concepts, MutableFacetHDSConcept);
};

template <class HDS>
bool concept_requirements()
{
    boost::function_requires<boost::hdstl::concepts::MutableFacetHDSConcept<HDS> >();
    class_concept_requirements<HDS>(); // force instantiation
    return true;
}

int test_main(int, char **)
{
    BOOST_CHECK(( concept_requirements<hdstl1::mutable_facet_hds_archetype>() ));
    return 0;
}
