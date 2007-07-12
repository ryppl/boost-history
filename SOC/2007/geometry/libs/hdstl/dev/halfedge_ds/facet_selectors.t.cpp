//facet_selectors.hpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a selector class.  We
// must make sure that all selectors are suitably defined and that the
// selection is done properly.
//
//@TEST_PLAN: First create an instance of all the selectors (to make sure they
// exist, and verify that there are no more than tested).  Then instantiate the
// 'container_gen<FacetS,ValueType>' for a given value type and all
// selectors, and verify that its members have the expected types and
// signatures.  Finally, verify that the usage example compiles and executes
// without errors, when assert is replaced by BOOST_CHECK.

#include <boost/hdstl/halfedge_ds/container_selectors.hpp>

#include <boost/test/minimal.hpp>

#include <set>
#include <string>
#include <vector>

using namespace boost::hdstl;
using namespace std;

// ===========================================================================
//                              SELECTOR CLASSES
// ===========================================================================

template <typename FacetS>
bool selector_requirements(FacetS const&) {
    return false;
}

bool selector_requirements(noFacetS const&) {
    return true;
}

template <typename Base, typename HalfedgeDescriptor>
bool selector_requirements(facetS<Base, false, HalfedgeDescriptor> const&) {
    return true;
}

template <typename Base, typename HalfedgeDescriptor>
bool selector_requirements(facetS<Base, true, HalfedgeDescriptor> const&) {
    return true;
}

// ===========================================================================
//                              CLASS CONTAINER_GEN
// ===========================================================================

template <typename FacetS>
bool selection_requirements(FacetS const&) {
    return false;
}

template <typename Base, typename HalfedgeDescriptor>
bool selector_requirements(facetS<Base, false, HalfedgeDescriptor> const&f) {
    return true;
}

template <typename Base, typename HalfedgeDescriptor>
bool selector_requirements(facetS<Base, true, HalfedgeDescriptor> const&) {
    return true;
}

template <class ContainerGen, typename ValueType, typename FacetS>
bool container_gen_requirements() {
    ValueType array[] = { 0, 1, 2, 3 };
    typename ContainerGen::type container(array, array + 4);
    
    BOOST_CHECK(( selection_requirements(FacetS(), container) ));

    // Types must exist.
    typedef typename ContainerGen::descriptor descriptor;
    typedef typename ContainerGen::iterator   iterator;

    // Value type of iterator must be a descriptor.
    iterator begin = ContainerGen::container_begin(container);
    descriptor theBegin = *begin;

    return true;
}

// ===========================================================================
//                              USAGE EXAMPLE
// ===========================================================================

///Usage
///-----
// Suppose we want a data structure 'ElementCollection' containing instances of
// a custom 'Element' type, stored in a container chosen by a selector given as
// template parameter.  To make this picture more concrete, let us pick an
// implementation:
//..
    struct Element {
        int           userId;
        std::string   familyName;
        int           securityLevel;
        Element(int uId, std::string name, int level)
        : userId(uId), familyName(name), securityLevel(level) {}
    };
    bool operator<(const Element& lhs, const Element& rhs) {
        return lhs.userId < rhs.userId;
    }
//..
//  We can implement 'ElementCollection' using the 'container_gen' facility as
//  follows.  We purposely keep the de
//..
    template <typename FacetS>
    class ElementCollection {
        // This class stores and gives access to a collection of 'Element'
        // objects, using the container selected by the specified 'FacetS'
        // selector.
  
      public:
        // TYPES
        typedef container_gen<FacetS, Element>       container_generator;
        typedef typename container_generator::type       container_type;
        typedef typename container_generator::descriptor descriptor;
        typedef typename container_generator::iterator   iterator;
  
      private:
        // DATA
        container_type  m_collection;
  
      public:
        // CREATORS
        ElementCollection() {}
            // Create an empty collection of 'Element' objects.
  
        template <typename Iterator>
        ElementCollection(Iterator elementBegin, Iterator elementEnd)
            // Create a collection of 'Element' objects initially containing
            // the objects in the specified range '[elementBegin, elementEnd)'.
        : m_collection(elementBegin, elementEnd) {}
  
        // MANIPULATORS
        iterator begin() { return container_generator::container_begin(m_collection); }
            // Return an iterator pointing to the beginning of this collection.
  
        iterator end() { return container_generator::container_end(m_collection); }
            // Return an iterator pointing past the end of this collection.
  
        container_type& theContainer() { return m_collection; }
            // Return the modifiable container underlying this collection.
  
        // ACCESSORS
        const container_type& theContainer() const { return m_collection; }
            // Return the non-modifiable container underlying this collection.
    };
//..
// We can now use the collection as follows.  First let us create the
// individual elements:
//..
    bool usageExample() {
        Element george(632,  "Harrison",  +78);
        Element john  (834,  "Lennon",    +255);
        Element paul  (932,  "McCartney", +126);
        Element ringo (1432, "Starr",     +123);
        Element theBeatles[] = { george, john, paul, ringo };
//..
// We can use a collection as a searchable set:
//..
        ElementCollection<setS> setCollection(theBeatles, theBeatles + 4);
        Element unknown(843, "Unkown",   +0);
        BOOST_CHECK(( setCollection.theContainer().find(unknown) == setCollection.end() ));
        BOOST_CHECK(( setCollection.theContainer().find(john)->familyName == "Lennon" ));
//..
// and access the iterators of the collection (here identical to the
// iterators of the container):
//..
        BOOST_CHECK(( setCollection.begin()->familyName == "Harrison" ));
        BOOST_CHECK(( (--setCollection.end())->familyName == "Starr" ));
//..
// or we can use the collection as an indexed array:
//..
        ElementCollection<vecS> vectorCollection(theBeatles, theBeatles + 4);
        BOOST_CHECK(( vectorCollection.theContainer()[1].familyName == "Lennon" ));
        BOOST_CHECK(( vectorCollection.theContainer()[3].familyName == "Starr" ));
//..
// and access the iterators of the collection (whose value type here is the
// descriptor, not the same as the iterators of the container):
//..
        BOOST_CHECK( *vectorCollection.begin() == 0 );
        BOOST_CHECK( *vectorCollection.end() == 4 );

        return true;
    }
//..

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

int test_main(int, char **)
{
    #if 0
    BOOST_CHECK(( selector_requirements(hashS()) ));
    #endif
    BOOST_CHECK(( selector_requirements(listS()) ));
    BOOST_CHECK(( selector_requirements(setS()) ));
    BOOST_CHECK(( selector_requirements(vecS()) ));

    #if 0
    BOOST_CHECK(( selection_requirements<container_gen<hashS, int>, hashS>() ));
    #endif
    BOOST_CHECK(( container_gen_requirements<container_gen<listS, int>, int, listS>() ));
    BOOST_CHECK(( container_gen_requirements<container_gen<setS, int>, int, setS>() ));
    BOOST_CHECK(( container_gen_requirements<container_gen<vecS, int>, int, vecS>() ));

    BOOST_CHECK(( usageExample() ));
    
    return 0;
}

