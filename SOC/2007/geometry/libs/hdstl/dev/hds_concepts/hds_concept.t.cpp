//hds_concept.t.cpp   -*- C++ -*-

//#include <boost/hdstl/hds_concepts/hds_concept.hpp>
#include "hds_concept.hpp"

//#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test_framework.hpp>
#include <boost/test/minimal.hpp>
#include <boost/concept_check.hpp>

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

namespace hdstl1 {

    struct hds_archetype {
        // This 'struct', intentionally defined in a namespace different from
        // 'hdstl', the 'hds_traits' specialization defined in the namespace
        // 'hdstl', and the supporting function 'opposite', defined in the same
        // namespace as this 'struct' to take advantage of ADL
        // (argument-dependent lookup) provide an archetype for the 'HDS'
        // concept.

        typedef int halfedge_descriptor;
    };

    hds_archetype::halfedge_descriptor
    opposite(hds_archetype::halfedge_descriptor h,
             const hds_archetype&)
    {
        return h;
    }

}  // namespace hdstl

namespace boost {
namespace hdstl {
    
    template <>
    struct hds_traits<hdstl1::hds_archetype>
    {
        typedef hdstl1::hds_archetype::halfedge_descriptor
                halfedge_descriptor;
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
    BOOST_CLASS_REQUIRE(HDS, boost::hdstl, HDSConcept);
};

template <class HDS>
bool concept_requirements()
{
    boost::function_requires<boost::hdstl::HDSConcept<HDS> >();
    class_concept_requirements<HDS>(); // force instantiation
    return true;
}

int test_main(int, char **)
{
    BOOST_CHECK(( concept_requirements<hdstl1::hds_archetype>() ));
    return 0;
}
