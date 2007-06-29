//mutable_backward_hds_concept.t.cpp   -*- C++ -*-
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

#include <boost/hdstl/hds_concepts/mutable_backward_hds_concept.hpp>
#include <boost/test/minimal.hpp>
#include <boost/concept_check.hpp>

namespace hdstl1 {

    struct TT : public boost::hdstl::backward_traversal_tag {};

    template<typename BackwardCategory>
    struct mutable_backward_hds_archetype {
        // This 'struct', intentionally defined in a namespace different from
        // 'hdstl', the 'hds_traits' specialization defined in the namespace
        // 'hdstl', and the supporting functions 'opposite', 'prev_in_facet',
        // 'prev_at_source', and 'prev_at_target' defined in the same namespace
        // as this 'struct' to take advantage of ADL (argument-dependent
        // lookup) provide an archetype for the 'MutableBackwardHDS' concept.

        typedef int halfedge_descriptor;
        typedef TT  traversal_category;
        typedef BackwardCategory backward_category;
    };

    template<typename BackwardCategory>
    typename mutable_backward_hds_archetype<BackwardCategory>::halfedge_descriptor
    opposite(typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor h,
             const mutable_backward_hds_archetype<BackwardCategory>&)
    {
        return h;
    }

    template<typename BackwardCategory>
    typename mutable_backward_hds_archetype<BackwardCategory>::halfedge_descriptor
    prev_in_facet(typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor h,
                  const mutable_backward_hds_archetype<BackwardCategory>&)
    {
        return h;
    }

    template<typename BackwardCategory>
    typename mutable_backward_hds_archetype<BackwardCategory>::halfedge_descriptor
    prev_at_source(typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor h,
                  const mutable_backward_hds_archetype<BackwardCategory>&)
    {
        return h;
    }

    template<typename BackwardCategory>
    typename mutable_backward_hds_archetype<BackwardCategory>::halfedge_descriptor
    prev_at_target(typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor h,
                  const mutable_backward_hds_archetype<BackwardCategory>&)
    {
        return h;
    }

    template<typename BackwardCategory>
    void
    set_prev_in_facet(typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor,
                      typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor,
                      const mutable_backward_hds_archetype<BackwardCategory>&)
    {
    }

    template<typename BackwardCategory>
    void
    set_prev_at_source(typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor,
                      typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor,
                      const mutable_backward_hds_archetype<BackwardCategory>&)
    {
    }
    
    template<typename BackwardCategory>
    void
    set_prev_at_target(typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor,
                      typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor,
                      const mutable_backward_hds_archetype<BackwardCategory>&)
    {
    }

    template<typename BackwardCategory>
    typename mutable_backward_hds_archetype<BackwardCategory>::halfedge_descriptor
    new_edge(mutable_backward_hds_archetype<BackwardCategory>&)
    {
        return typename mutable_backward_hds_archetype<BackwardCategory>
                                                        ::halfedge_descriptor();
    }

    template<typename BackwardCategory>
    void
    delete_edge(typename mutable_backward_hds_archetype<BackwardCategory>
                                                          ::halfedge_descriptor,
                mutable_backward_hds_archetype<BackwardCategory>&)
    {
    }

}  // namespace hdstl

namespace boost {
namespace hdstl {
    
    template <typename BackwardCategory>
    struct hds_traits<hdstl1::mutable_backward_hds_archetype<BackwardCategory> >
    {
        typedef typename hdstl1::mutable_backward_hds_archetype<BackwardCategory>
                                                          ::halfedge_descriptor
                                                           halfedge_descriptor;
        typedef typename hdstl1::mutable_backward_hds_archetype<BackwardCategory>
                                                           ::traversal_category
                                                           traversal_category;
        typedef typename hdstl1::mutable_backward_hds_archetype<BackwardCategory>
                                                           ::backward_category
                                                           backward_category;
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
    BOOST_CLASS_REQUIRE(HDS, boost::hdstl::concepts, MutableBackwardHDSConcept);
};

template <class HDS>
bool concept_requirements()
{
    boost::function_requires<
                     boost::hdstl::concepts::MutableBackwardHDSConcept<HDS> >();
    class_concept_requirements<HDS>(); // force instantiation
    return true;
}

int test_main(int, char **)
{
    BOOST_CHECK(( concept_requirements<
         hdstl1::mutable_backward_hds_archetype<
                                        boost::hdstl::prev_in_facet_tag> >() ));
    BOOST_CHECK(( concept_requirements<
         hdstl1::mutable_backward_hds_archetype<
                                       boost::hdstl::prev_at_source_tag> >() ));
    BOOST_CHECK(( concept_requirements<
         hdstl1::mutable_backward_hds_archetype<
                                       boost::hdstl::prev_at_target_tag> >() ));
    return 0;
}
