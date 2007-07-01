//bidirectional_hds_concept_archetype.t.cpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a concept-archetype class.  We
// proceed with the standard test plan for such a class.
//
//@TEST_PLAN:  Verify (using the Boost.ConceptCheck library macros) that the
//class 'bidirectional_hds_concept_archetype' satisfies all the concept requirements
//of the 'HDS' concept.

#include <boost/hdstl/hds_archetypes/bidirectional_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_concepts/bidirectional_hds_concept.hpp>
#include <boost/test/minimal.hpp>
#include <boost/concept_check.hpp>

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

template <class HDS>
struct class_concept_requirements
{
    BOOST_CLASS_REQUIRE(HDS, boost::hdstl::concepts, BidirectionalHDSConcept);
};

template <class HDS>
bool concept_requirements()
{
    using namespace boost;
    function_requires<hdstl::concepts::BidirectionalHDSConcept<HDS> >();
    class_concept_requirements<HDS>(); // force instantiation
    BOOST_CHECK(false == hdstl::hds_traits<HDS>::supports_vertices);
    BOOST_CHECK(false == hdstl::hds_traits<HDS>::supports_facets);
    return true;
}

int test_main(int, char **)
{
    BOOST_CHECK(( concept_requirements<
         boost::hdstl::BidirectionalHDSConcept_archetype<
            boost::hdstl::next_in_facet_tag,
            boost::hdstl::prev_in_facet_tag> >() ));
    BOOST_CHECK(( concept_requirements<
         boost::hdstl::BidirectionalHDSConcept_archetype<
            boost::hdstl::next_at_source_tag,
            boost::hdstl::prev_at_source_tag> >() ));
    BOOST_CHECK(( concept_requirements<
         boost::hdstl::BidirectionalHDSConcept_archetype<
            boost::hdstl::next_at_target_tag,
            boost::hdstl::prev_at_target_tag> >() ));
    return 0;
}
