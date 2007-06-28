//forward_hds_concept.t.cpp   -*- C++ -*-
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

#include <boost/hdstl/hds_concepts/forward_hds_concept.hpp>
#include <boost/test/minimal.hpp>
#include <boost/concept_check.hpp>

namespace hdstl1 {

    struct TT : public boost::hdstl::forward_traversal_tag {};

    template<typename ForwardCategory>
    struct forward_hds_archetype {
        // This 'struct', intentionally defined in a namespace different from
        // 'hdstl', the 'hds_traits' specialization defined in the namespace
        // 'hdstl', and the supporting functions 'opposite', 'next_in_facet',
        // 'next_at_source', and 'next_at_target' defined in the same
        // namespace as this 'struct' to take advantage of ADL
        // (argument-dependent lookup) provide an archetype for the 'ForwardHDS'
        // concept.

        typedef int halfedge_descriptor;
        typedef TT  traversal_category;
        typedef ForwardCategory forward_category;
    };

    template<typename ForwardCategory>
    typename forward_hds_archetype<ForwardCategory>::halfedge_descriptor
    opposite(typename forward_hds_archetype<ForwardCategory>
                                                        ::halfedge_descriptor h,
             const forward_hds_archetype<ForwardCategory>&)
    {
        return h;
    }

    template<typename ForwardCategory>
    typename forward_hds_archetype<ForwardCategory>::halfedge_descriptor
    next_in_facet(typename forward_hds_archetype<ForwardCategory>
                                                        ::halfedge_descriptor h,
             const forward_hds_archetype<ForwardCategory>&)
    {
        return h;
    }

    template<typename ForwardCategory>
    typename forward_hds_archetype<ForwardCategory>::halfedge_descriptor
    next_at_source(typename forward_hds_archetype<ForwardCategory>
                                                        ::halfedge_descriptor h,
             const forward_hds_archetype<ForwardCategory>&)
    {
        return h;
    }

    template<typename ForwardCategory>
    typename forward_hds_archetype<ForwardCategory>::halfedge_descriptor
    next_at_target(typename forward_hds_archetype<ForwardCategory>
                                                        ::halfedge_descriptor h,
             const forward_hds_archetype<ForwardCategory>&)
    {
        return h;
    }

}  // namespace hdstl

namespace boost {
namespace hdstl {
    
    template <typename ForwardCategory>
    struct hds_traits<hdstl1::forward_hds_archetype<ForwardCategory> >
    {
        typedef typename hdstl1::forward_hds_archetype<ForwardCategory>
                                                          ::halfedge_descriptor
                                                           halfedge_descriptor;
        typedef typename hdstl1::forward_hds_archetype<ForwardCategory>
                                                           ::traversal_category
                                                           traversal_category;
        typedef typename hdstl1::forward_hds_archetype<ForwardCategory>
                                                           ::forward_category
                                                           forward_category;
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
    BOOST_CLASS_REQUIRE(HDS, boost::hdstl::concepts, ForwardHDSConcept);
};

template <class HDS>
bool concept_requirements()
{
    boost::function_requires<boost::hdstl::concepts::ForwardHDSConcept<HDS> >();
    class_concept_requirements<HDS>(); // force instantiation
    return true;
}

int test_main(int, char **)
{
    BOOST_CHECK(( concept_requirements<
         hdstl1::forward_hds_archetype<boost::hdstl::next_in_facet_tag> >() ));
    BOOST_CHECK(( concept_requirements<
         hdstl1::forward_hds_archetype<boost::hdstl::next_at_source_tag> >() ));
    BOOST_CHECK(( concept_requirements<
         hdstl1::forward_hds_archetype<boost::hdstl::next_at_target_tag> >() ));
    return 0;
}
