//container_selectors.hpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a selector class.  We
// must make sure that all selectors are suitably defined and that the
// selection is done properly.
//
//@TEST_PLAN: First create an instance of all the selectors (to make sure they
// exist, and verify that there are no more than tested).  Then instantiate the
// 'container_gen<ContainerS,ValueType>' for a given value type and all
// selectors, and verify that its members have the expected types and
// signatures.  Finally, verify that the usage example compiles and executes
// without errors, when assert is replaced by BOOST_CHECK.

#include <boost/hdstl/halfedge_ds/container_selectors.hpp>
#include <boost/hdstl/halfedge_ds/meta_functions.hpp>

#include <boost/test/minimal.hpp>

#include <set>
#include <string>
#include <vector>

using namespace boost::hdstl;
using namespace std;

// ===========================================================================
//                              SELECTOR CLASSES
// ===========================================================================

template <typename ContainerS>
bool selector_requirements(ContainerS const&) {
    return false;
}

#if 0
bool selector_requirements(boost::hdstl::hashS const&) {
    BOOST_CHECK(( meta_is_same<container_gen<hashS>::selector,
                               listS>::value ));
    // TODO Other checks
    return true;
}
#endif

bool selector_requirements(listS const&) {
    BOOST_CHECK(( meta_is_same<container_gen<listS, int>::selector,
                               listS>::value ));
    // TODO Other checks
    return true;
}

bool selector_requirements(setS const&) {
    BOOST_CHECK(( meta_is_same<container_gen<setS, int>::selector,
                               setS>::value ));
    // TODO Other checks
    return true;
}

bool selector_requirements(vecS const&) {
    BOOST_CHECK(( meta_is_same<container_gen<vecS, int>::selector,
                               vecS>::value ));
    // TODO Other checks
    return true;
}

// ===========================================================================
//                              CLASS CONTAINER_GEN
// ===========================================================================

template <class ContainerGen, typename ContainerS>
bool selection_requirements(ContainerS const&, ContainerGen&) {
    return false;
}

#if 0
template <typename ValueType>
bool selection_requirements(hashS, std::tr1::hash_set<ValueType>&) {
    return true;
}
#endif
    
template <typename ValueType>
bool selection_requirements(listS, std::list<ValueType>&) {
    return true;
}
    
template <typename ValueType>
bool selection_requirements(setS, std::set<ValueType>&) {
    return true;
}
    
template <typename ValueType>
bool selection_requirements(vecS, std::vector<ValueType>&) {
    return true;
}
    
template <class ContainerGen, typename ValueType, typename ContainerS>
bool container_gen_requirements() {
    ValueType array[] = { 0, 1, 2, 3 };
    typename ContainerGen::type container(array, array + 4);
    
    BOOST_CHECK(( selection_requirements(ContainerS(), container) ));

    // Types must exist.
    typedef typename ContainerGen::descriptor descriptor;
    typedef typename ContainerGen::iterator   iterator;

    // Value type of iterator must be a descriptor.
    iterator begin = ContainerGen::container_begin(container);
    descriptor theBegin = *begin;

    // Descriptor must hold correct value:
    BOOST_CHECK(( ContainerGen::value(theBegin, container) == 0 ));
    BOOST_CHECK(( ContainerGen::value(*++begin, container) == 1 ));
    BOOST_CHECK(( ContainerGen::value(*++begin, container) == 2 ));
    BOOST_CHECK(( ContainerGen::value(*++begin, container) == 3 ));

    // Value type of iterator must be a descriptor.
    iterator end = ContainerGen::container_end(container);
    descriptor theEnd = *--end;

    // Descriptor must hold correct value:
    BOOST_CHECK(( ContainerGen::value(theEnd, container) == 3 ));
    BOOST_CHECK(( ContainerGen::value(*--end, container) == 2 ));
    BOOST_CHECK(( ContainerGen::value(*--end, container) == 1 ));
    BOOST_CHECK(( ContainerGen::value(*--end, container) == 0 ));
    return true;
}

template <class ContainerGen, typename ValueType, typename ContainerS>
bool container_gen_mutable_requirements() {
    // Types must exist.
    typedef typename ContainerGen::descriptor descriptor;
    typedef typename ContainerGen::iterator   iterator;
    
    typename ContainerGen::type container;
    ValueType array[] = { 0, 1, 2, 3 };
    descriptor h0 = *ContainerGen::container_add(array[0], container);   
    descriptor h1 = *ContainerGen::container_add(array[1], container);   
    descriptor h2 = *ContainerGen::container_add(array[2], container);   
    descriptor h3 = *ContainerGen::container_add(array[3], container);   
    (void) h0; (void) h1; (void) h2; (void) h3;
    
    // Value type of iterator must be a descriptor.
    iterator begin = ContainerGen::container_begin(container);
    descriptor theBegin = *begin;

    // Descriptor must hold correct value:
    BOOST_CHECK(( ContainerGen::value(theBegin, container) == 0 ));
    BOOST_CHECK(( ContainerGen::value(*++begin, container) == 1 ));
    BOOST_CHECK(( ContainerGen::value(*++begin, container) == 2 ));
    BOOST_CHECK(( ContainerGen::value(*++begin, container) == 3 ));

    // Value type of iterator must be a descriptor.
    iterator end = ContainerGen::container_end(container);
    descriptor theEnd = *--end;

    // Descriptor must hold correct value:
    BOOST_CHECK(( ContainerGen::value(theEnd, container) == 3 ));
    BOOST_CHECK(( ContainerGen::value(*--end, container) == 2 ));
    BOOST_CHECK(( ContainerGen::value(*--end, container) == 1 ));
    BOOST_CHECK(( ContainerGen::value(*--end, container) == 0 ));

    //remove the elements from container
    begin = ContainerGen::container_begin(container);
    theBegin = *begin;
    BOOST_CHECK(( container.size() == 4 ));
    //ContainerGen::container_remove(static_cast<iterator>(*&h0), container);
    ContainerGen::container_remove(begin, container);
    BOOST_CHECK(( container.size() == 3 ));
    begin = ContainerGen::container_begin(container);
    ContainerGen::container_remove(begin, container);
    BOOST_CHECK(( container.size() == 2 ));
    begin = ContainerGen::container_begin(container);
    ContainerGen::container_remove(begin, container);
    BOOST_CHECK(( container.size() == 1 ));
    begin = ContainerGen::container_begin(container);
    ContainerGen::container_remove(begin, container);
    BOOST_CHECK(( container.empty() ));

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
    template <typename ContainerS>
    class ElementCollection {
        // This class stores and gives access to a collection of 'Element'
        // objects, using the container selected by the specified 'ContainerS'
        // selector.
  
      public:
        // TYPES
        typedef container_gen<ContainerS, Element>       container_generator;
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

    BOOST_CHECK(( container_gen_mutable_requirements<container_gen<listS, int>, int, listS>() ));
    BOOST_CHECK(( container_gen_mutable_requirements<container_gen<setS, int>, int, setS>() ));
    BOOST_CHECK(( container_gen_mutable_requirements<container_gen<vecS, int>, int, vecS>() ));

    BOOST_CHECK(( usageExample() ));
    
    return 0;
}

