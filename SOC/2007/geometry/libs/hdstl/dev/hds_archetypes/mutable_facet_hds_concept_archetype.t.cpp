//mutable_facet_hds_concept_archetype.t.cpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a concept-archetype class.  We
// proceed with the standard test plan for such a class.
//
//@TEST_PLAN:  Verify (using the Boost.ConceptCheck library macros) that the
// class 'mutable_facet_hds_concept_archetype' satisfies all the concept requirements of the
// 'HDS' concept.

#include <boost/hdstl/hds_archetypes/mutable_facet_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_concepts/mutable_facet_hds_concept.hpp>
#include <boost/test/minimal.hpp>
#include <boost/concept_check.hpp>

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
    using namespace boost;
    function_requires<hdstl::concepts::MutableFacetHDSConcept<HDS> >();
    class_concept_requirements<HDS>(); // force instantiation
    BOOST_CHECK(false == hdstl::hds_traits<HDS>::supports_vertices);
    BOOST_CHECK(true == hdstl::hds_traits<HDS>::supports_facets);
    return true;
}

int test_main(int, char **)
{
    BOOST_CHECK(( concept_requirements<
        boost::hdstl::MutableFacetHDSConcept_archetype>() ));
    return 0;
}
