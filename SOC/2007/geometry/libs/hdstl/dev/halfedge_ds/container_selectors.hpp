//container_selectors.hpp   -*- C++ -*-
//
//@PURPOSE: Provide selectors for container
//
//@CLASSES:
//  'container_gen':  class for selecting the storage type of a collection
//          'hashS':  selector for the 'std::tr1::hash_set' container
//          'listS':  selector for the 'std::list' container
//           'setS':  selector for the 'std::set' container
//         'slistS':  selector for the 'std::ext::slist' container (if provided)
//           'vecS':  selector for the 'std::vector' container
//  
//@SEE_ALSO: {halfedge_ds.hpp}
//
//@DESCRIPTION: This component provides 'container_gen', a class which,
// properly specialized for the specified 'Selector', gives various traits of
// the container of the specified 'ValueType' suitable by the 'halfedge_ds'
// implementation.  The various selectors defined also in this component are:
//..
//  Selector    Container
//  --------    -------------------------------
//   'hashS'    'std::tr1::hash_set'
//   'listS'    'std::list'
//    'setS'    'std::set'
//  'slistS'    'std::ext::slist' (if provided)
//    'vecS'    'std::vector'
//..
//
///Usage
///-----
// Suppose we want a data structure 'ElementCollection' containing instances of
// a custom 'Element' type, stored in a container chosen by a selector given as
// template parameter.  To make this picture more concrete, let us pick an
// implementation:
//..
//  struct Element {
//      int           userId;
//      std::string   familyName;
//      int           securityLevel;
//      Element(int uId, std::string name, int level)
//      : userId(uId), familyName(name), selected(level) {}
//  };
//  bool operator<(const Element& lhs, const Element& rhs) {
//      return lhs.userId < rhs.userId;
//  }
//..
//  We can implement 'ElementCollection' using the 'container_gen' facility as
//  follows.  We purposely keep the de
//..
//  template <typename ContainerS>
//  class ElementCollection {
//      // This class stores and gives access to a collection of 'Element'
//      // objects, using the container selected by the specified 'ContainerS'
//      // selector.
//
//    public:
//      // TYPES
//      typedef typename container_gen<ContainerS>::type       container_type;
//      typedef typename container_gen<ContainerS>::descriptor container_type;
//
//    private:
//      // DATA
//      container_type  m_collection;
//
//    public:
//      // CREATORS
//      ElementCollection();
//          // Create an empty collection of 'Element' objects.
//
//      template <Iterator>
//      ElementCollection(Iterator elementBegin, Iterator elementEnd);
//          // Create a collection of 'Element' objects initially containing
//          // the objects in the specified range '[elementBegin, elementEnd)'.
//
//      // MANIPULATORS
//      iterator appendElement(Element const& object);
//          // Append the specified 'object' to this collection.
//
//      iterator begin() { return m_collection.begin(); }
//          // Return an iterator pointing to the beginning of this collection.
//
//      iterator end() { return m_collection.end(); }
//          // Return an iterator pointing past the end of this collection.
//
//      container_type& theContainer() { return m_collection; }
//          // Return the modifiable container underlying this collection.
//
//      // ACCESSORS
//      const container_type& theContainer() const { return m_collection; }
//          // Return the non-modifiable container underlying this collection.
//  };
//..
// We can now use the collection as follows.  First let us create the
// individual elements:
//..
//  Element george(632, "Harrison",  +78);
//  Element john  (834, "Lennon",    +255);
//  Element paul  (432, "McCartney", +126);
//  Element ringo (432, "Starr",     +123);
//  Element theBeatles[] = { george, john, paul, ringo };
//..
// We can use a collection as a searchable set:
//..
//  ElementCollection<setS> setCollection(theBeatles, theBeatles + 4);
//  assert(setCollection.container().find(843) == setCollection.end());
//  assert(setCollection.container()[834]->familyName == "Lennon");
//..
// or as an indexed array:
//..
//  ElementCollection<vecS> vectorCollection(theBeatles, theBeatles + 4);
//  assert(setCollection.container()[1]->familyName == "Lennon");
//  assert(setCollection.container()[3]->familyName == "Starr");
//..

#ifndef BOOST_HDSTL_CONTAINER_SELECTORS_HPP
#define BOOST_HDSTL_CONTAINER_SELECTORS_HPP 1

#include <list>
#include <set>
#if 0
#include <slist>
#include <tr1/hash_set>
#endif
#include <vector>

namespace boost {
namespace hdstl {

template <typename Selector, typename ValueType>
struct container_gen {
    // This 'struct', properly specialized for the specified 'Selector', gives
    // various traits of the container of the specified 'ValueType' suitable by
    // the 'halfedge_ds' implementation.
};

/*
struct hashS {
    // This tag 'struct' is used by 'container_gen' to select the
    // specialization that uses the 'std::tr1::hash_set' container.
};
*/

struct listS {
    // This tag 'struct' is used by 'container_gen' to select the
    // specialization that uses the 'std::list' container.
};

struct setS {
    // This tag 'struct' is used by 'container_gen' to select the
    // specialization that uses the 'std::set' container.
};

/*
struct slistS {
    // This tag 'struct' is used by 'container_gen' to select the
    // specialization that uses the 'slist' container (an SGI STL extension to
    // the C++ Standard Library).
};
*/

struct vecS {
    // This tag 'struct' is used by 'container_gen' to select the
    // specialization that uses the 'std::vector' container.
};

// SPECIALIZATIONS
#if 0
template <typename ValueType>
struct container_gen<hashS, ValueType> {
    // This specialization of 'container_gen' selects the 'std::tr1::hash_set'
    // container.

    // TYPES
    typedef std::tr1::hash_set<ValueType> type;
        // The container type, here a 'std::tr1::hash_set'.

    typedef ValueType* descriptor;
        // Type used to describe an element in the collection of elements
        // present in a given container.
        //
        // TODO  This is only a suitable choice if hash_set uses chaining, but
        // not if it uses double hashing.  TR1 allows iterators and references
        // to be invalidated upon (implicit or explicit) rehashing.  Possible
        // solution: rely on Boost.multiIndex hashed indices, which have
        // stronger guarantees.

    struct iterator : public std::list<ValueType>::iterator {
        // Iterator type over a given container.  Note: the value type must be
        // a descriptor, so we cannot use a 'tr1::hash_set:iterator'.  Instead,
        // we create a very similar type, but override the 'operator*'.

        typedef typename std::tr1::hash_set<ValueType>::iterator base_type;
      public:
        // CREATORS
        iterator() {}
            // Create an unitialized iterator.
        iterator(base_type it) : base_type(it) {}
            // Create an iterator pointing to the same element as the specified
            // 'it' iterator in the specified 'container'.
        // MANIPULATORS
        ValueType* operator*()
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type());
        }
        // ACCESSORS
        ValueType* operator*() const
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type());
        }
    };

    // CLASS METHODS
    static iterator container_begin(type& container)
        // This utility returns an iterator to the beginning of the 
        // specified 'container'.
    {
        return container.begin();
    }

    static iterator container_end(type& container)
        // This utility returns an iterator to the beginning of the 
        // specified 'container'.
    {
        return container.end();
    }

    static descriptor to_descriptor(iterator it, type& container)
        // This utility translates the specified 'it' iterator to a descriptor
        // referring to the same element in the specified 'container'.
    {
        return &(*it);
    }

    static iterator to_iterator(descriptor desc, type& container)
        // This utility translates the specified 'desc' descriptor to an
        // iterator referring to the same element in the specified 'container'.
    {
        return /* not known */;
    }
};
#endif

template <typename ValueType>
struct container_gen<listS, ValueType> {
    // This specialization of 'container_gen' selects the 'std::list'
    // container.

    // TYPES
    typedef std::list<ValueType> type;
        // The container type, here a 'std::list'.

    typedef ValueType* descriptor;
        // Type used to describe an element in the collection of elements
        // present in a given container.

    struct iterator : public std::list<ValueType>::iterator {
        // Iterator type over a given container.  Note: the value type must be
        // a descriptor, so we cannot use a 'std::list:iterator'.  Instead,
        // we create a very similar type, but override the 'operator*'.

        typedef typename std::list<ValueType>::iterator base_type;
      public:
        // CREATORS
        iterator() {}
            // Create an unitialized iterator.
        iterator(base_type it) : base_type(it) {}
            // Create an iterator pointing to the same element as the specified
            // 'it' iterator in the specified 'container'.
        // MANIPULATORS
        ValueType* operator*()
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type());
        }
        // ACCESSORS
        ValueType* operator*() const
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type());
        }
    };

    // CLASS METHODS
    static iterator container_begin(type& container)
        // This utility returns an iterator to the beginning of the 
        // specified 'container'.
    {
        return container.begin();
    }

    static iterator container_end(type& container)
        // This utility returns an iterator to the beginning of the 
        // specified 'container'.
    {
        return container.end();
    }
};

template <typename ValueType>
struct container_gen<setS, ValueType> {
    // This specialization of 'container_gen' selects the 'std::set'
    // container.

    // TYPES
    typedef std::set<ValueType> type;
        // The container type, here a 'std::set'.

    typedef const ValueType* descriptor;
        // Type used to describe an element in the collection of elements
        // present in a given container.

    struct iterator : public std::set<ValueType>::iterator {
        // Iterator type over a given container.  Note: the value type must be
        // a descriptor, so we cannot use a 'std::set:iterator'.  Instead,
        // we create a very similar type, but override the 'operator*'.

        typedef typename std::set<ValueType>::iterator base_type;
      public:
        // CREATORS
        iterator() {}
            // Create an unitialized iterator.
        iterator(base_type it) : base_type(it) {}
            // Create an iterator pointing to the same element as the specified
            // 'it' iterator in the specified 'container'.
        // MANIPULATORS
        const ValueType* operator*()
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type());
        }
        // ACCESSORS
        const ValueType* operator*() const
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type());
        }
    };

    // CLASS METHODS
    static iterator container_begin(type& container)
        // This utility returns an iterator to the beginning of the 
        // specified 'container'.
    {
        return container.begin();
    }

    static iterator container_end(type& container)
        // This utility returns an iterator to the beginning of the 
        // specified 'container'.
    {
        return container.end();
    }
};

template <typename ValueType>
struct container_gen<vecS, ValueType> {

    // TYPES
    typedef std::vector<ValueType> type;
        // The container type, here a 'std::list'.

    typedef typename std::vector<ValueType>::size_type  descriptor;
        // Type used to describe an element in the collection of elements
        // present in a given container.

    struct iterator : public std::vector<ValueType>::iterator {
        // Iterator type over a given container.  Note: the value type must be
        // a descriptor, so we cannot use a 'std::vector:iterator'.  Instead,
        // we create a very similar type, but override the 'operator*'.

        typedef typename std::vector<ValueType>::iterator base_type;
        // DATA
        std::vector<ValueType> *m_container;
      public:
        // CREATORS
        iterator()
            // Create an unitialized iterator.
        : m_container(0) {}
        iterator(std::vector<ValueType> *container, base_type it)
            // Create an iterator pointing to the same element as the specified
            // 'it' iterator in the specified 'container'.
        : base_type(it), m_container(container) {}
        // MANIPULATORS
        typename std::vector<ValueType>::size_type operator*()
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return base_type(*this) - m_container->begin();
        }
        // ACCESSORS
        typename std::vector<ValueType>::size_type operator*() const
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return base_type(*this) - m_container->begin();
        }
    };

    // CLASS METHODS
    static iterator container_begin(type& container)
        // This utility returns an iterator to the beginning of the 
        // specified 'container'.
    {
        return iterator(&container, container.begin());
    }

    static iterator container_end(type& container)
        // This utility returns an iterator to the beginning of the 
        // specified 'container'.
    {
        return iterator(&container, container.end());
    }
};

} // namespace hdstl
} // namespace boost

#endif
