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
// halfedge_descriptor as an int, although a real archetype would make
// this into a custom-made class with the tailored minimal requirements.

namespace boost {
namespace hdstl {
struct hds_archetype1 {
    typedef int halfedge_descriptor;
    halfedge_descriptor opposite(halfedge_descriptor h) const { return h; }
};


template<typename T>
int opposite(const T& hds, int h) { return h; }

struct hds_archetype2 {
    typedef int halfedge_descriptor;
    halfedge_descriptor opposite(halfedge_descriptor h) const 
    {
        return boost::hdstl::opposite(*this, h);
    }
};

    template <>
    class hds_traits<hds_archetype1>
    {
        public:
        typedef int halfedge_descriptor;
        static const bool supports_vertices = false;
        static const bool supports_facets   = false;
    };
    template <>
    class hds_traits<hds_archetype2>
    {
        public:
        typedef int halfedge_descriptor;
        enum { supports_vertices = true };
        enum { supports_facets   = true };
    };

template <class HDS>
struct class_concept_requirements
{
    BOOST_CLASS_REQUIRE(HDS, boost::hdstl, HDSConcept);
};

template <class HDS>
bool concept_requirements()
{
   function_requires<HDSConcept<HDS> >();
   class_concept_requirements<HDS>(); // force instantiation
}

int main()
{
    BOOST_CHECK(( concept_requirements<hds_archetype1>() ));
    BOOST_CHECK(( concept_requirements<hds_archetype2>() ));
    return 0;
}

} // end namespace hdstl
} // end namespace boost
