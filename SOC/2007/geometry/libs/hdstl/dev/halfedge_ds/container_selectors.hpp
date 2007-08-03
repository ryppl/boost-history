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
//      : userId(uId), familyName(name), securityLevel(level) {}
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
//      typedef container_gen<ContainerS, Element>       container_generator;
//      typedef typename container_generator::type       container_type;
//      typedef typename container_generator::descriptor descriptor;
//      typedef typename container_generator::iterator   iterator;
//
//    private:
//      // DATA
//      container_type  m_collection;
//
//    public:
//      // CREATORS
//      ElementCollection() {}
//          // Create an empty collection of 'Element' objects.
//
//      template <typename Iterator>
//      ElementCollection(Iterator elementBegin, Iterator elementEnd)
//          // Create a collection of 'Element' objects initially containing
//          // the objects in the specified range '[elementBegin, elementEnd)'.
//      : m_collection(elementBegin, elementEnd) {}
//
//      // MANIPULATORS
//      iterator begin() { return container_generator::container_begin(m_collection); }
//          // Return an iterator pointing to the beginning of this collection.
//
//      iterator end() { return container_generator::container_end(m_collection); }
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
//  int main() {
//      Element george(632,  "Harrison",  +78);
//      Element john  (834,  "Lennon",    +255);
//      Element paul  (932,  "McCartney", +126);
//      Element ringo (1432, "Starr",     +123);
//      Element theBeatles[] = { george, john, paul, ringo };
//..
// We can use a collection as a searchable set:
//..
//      ElementCollection<setS> setCollection(theBeatles, theBeatles + 4);
//      Element unknown(843, "Unkown",   +0);
//      BOOST_CHECK(( setCollection.theContainer().find(unknown) == setCollection.end() ));
//      BOOST_CHECK(( setCollection.theContainer().find(john)->familyName == "Lennon" ));
//..
// and access the iterators of the collection (here identical to the
// iterators of the container):
//..
//      BOOST_CHECK(( setCollection.begin()->familyName == "Harrison" ));
//      BOOST_CHECK(( (--setCollection.end())->familyName == "Starr" ));
//..
// or we can use the collection as an indexed array:
//..
//      ElementCollection<vecS> vectorCollection(theBeatles, theBeatles + 4);
//      BOOST_CHECK(( vectorCollection.theContainer()[1].familyName == "Lennon" ));
//      BOOST_CHECK(( vectorCollection.theContainer()[3].familyName == "Starr" ));
//..
// and access the iterators of the collection (whose value type here is the
// descriptor, not the same as the iterators of the container):
//..
//      BOOST_CHECK( *vectorCollection.begin() == 0 );
//      BOOST_CHECK( *vectorCollection.end() == 4 );
//
//      return true;
//  }
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

                   // ===================
                   // container selectors
                   // ===================

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


                   // ===================
                   // class container_gen
                   // ===================

template <typename Selector, typename ValueType>
struct container_gen {
    // This 'struct', properly specialized for the specified 'Selector', gives
    // various traits of the container of the specified 'ValueType' suitable by
    // the 'halfedge_ds' implementation.
};

// SPECIALIZATIONS
#if 0
template <typename ValueType>
struct container_gen<hashS, ValueType> {
    // This specialization of 'container_gen' selects the 'std::tr1::hash_set'
    // container.

    // TYPES
    typedef hashS selector;
        // The container selector, here a 'hashS'.

    typedef std::tr1::hash_set<ValueType> type;
        // The container type, here a 'std::tr1::hash_set'.
    
    typedef typename std::tr1::hash_set<ValueType>::size_type size_type;
        // The container size type.

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
        // TYPES
        typedef ValueType*                            value_type;
        typedef value_type&                           reference;
        typedef value_type*                           pointer;
        typedef typename base_type::difference_type   difference_type;
        typedef typename base_type::iterator_category iterator_category;
        // CREATORS
        iterator() {}
            // Create an unitialized iterator.
        iterator(base_type it) : base_type(it) {}
            // Create an iterator pointing to the same element as the specified
            // 'it' iterator in the specified 'container'.
        // MANIPULATORS
        value_type operator*()
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type());
        }
        // ACCESSORS
        value_type operator*() const
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

    static ValueType& value(descriptor x, type& container)
            // This utility returns the value associated with the specified
            // descriptor 'x' of the specified 'container'.
    {
        (void)container;  // avoid unused variable warning
        return *x;
    }
};
#endif

template <typename ValueType>
struct container_gen<listS, ValueType> {
    // This specialization of 'container_gen' selects the 'std::list'
    // container.  It offers a descriptor type that is simply a pointer to a
    // value, and an iterator type that is a list iterator but whose value type
    // is instead a descriptor.

    // TYPES
    typedef listS selector;
        // The container selector, here a 'listS'.

    typedef std::list<ValueType> type;
        // The container type, here a 'std::list'.
    
    typedef typename std::list<ValueType>::size_type size_type;
        // The container size type.

    typedef ValueType* descriptor;
        // Type used to describe an element in the collection of elements
        // present in a given container.

    struct iterator : public std::list<ValueType>::iterator {
        // Iterator type over a given container.  Note: the value type must be
        // a descriptor, so we cannot use a 'std::list:iterator'.  Instead,
        // we create a very similar type, but override the 'operator*'.

        typedef typename std::list<ValueType>::iterator base_type;
      public:
        // TYPES
        typedef ValueType*                            value_type;
        typedef value_type                            reference;
        typedef value_type*                           pointer;
        typedef typename base_type::difference_type   difference_type;
        typedef typename base_type::iterator_category iterator_category;
        // CREATORS
        iterator() {}
            // Create an unitialized iterator.
        iterator(base_type it) : base_type(it) {}
            // Create an iterator pointing to the same element as the specified
            // 'it' iterator in the specified 'container'.
        // MANIPULATORS
        iterator& operator++() { ++static_cast<base_type&>(*this); return *this; }
        iterator& operator--() { --static_cast<base_type&>(*this); return *this; }
        reference operator*()
            // Return the modifiable descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type(*this));
        }
        // ACCESSORS
        bool operator==(iterator rhs)
        {
            return base_type(*this) == base_type(rhs);
        }
        value_type operator*() const
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type(*this));
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

    static iterator container_add(ValueType& x, type& container)
        // This utility adds an element at the end of the 
        // specified 'container', and returns an iterator 
        // pointing to that element.
    {
        container.push_back(x);
        return --container_end(container);
    }

    static void container_remove(iterator it, type& container)
        // This utility removes the element with specified iterator
        // 'it' from the specified 'container'.
    {
        container.erase(it);
    }
    
    static void container_remove(descriptor x, type& container)
        // This utility removes the element with specified descriptor
        // 'x' from the specified 'container'.
    {
        for (iterator it = container.begin(); it!=container.end(); ++it){
            if (*it == x){
                container.erase(it);
                break;
            }
        }
    }

    static ValueType& value(descriptor x, type& container)
            // This utility returns the value associated with the specified
            // descriptor 'x' of the specified 'container'.
    {
        (void)container;  // avoid unused variable warning
        return *x;
    }
};

template <typename ValueType>
struct container_gen<setS, ValueType> {
    // This specialization of 'container_gen' selects the 'std::set'
    // container.

    // TYPES
    typedef setS selector;
        // The container selector, here a 'setS'.

    typedef std::set<ValueType> type;
        // The container type, here a 'std::set'.

    typedef typename std::set<ValueType>::size_type size_type;
        // The container size type.

    typedef const ValueType* descriptor;
        // Type used to describe an element in the collection of elements
        // present in a given container.

    struct iterator : public std::set<ValueType>::iterator {
        // Iterator type over a given container.  Note: the value type must be
        // a descriptor, so we cannot use a 'std::set:iterator'.  Instead,
        // we create a very similar type, but override the 'operator*'.

        typedef typename std::set<ValueType>::iterator base_type;
      public:
        // TYPES
        typedef const ValueType*                      value_type;
        typedef value_type                            reference;
        typedef value_type*                           pointer;
        typedef typename base_type::difference_type   difference_type;
        typedef typename base_type::iterator_category iterator_category;
        // CREATORS
        iterator() {}
            // Create an unitialized iterator.
        iterator(base_type it) : base_type(it) {}
            // Create an iterator pointing to the same element as the specified
            // 'it' iterator in the specified 'container'.
        // MANIPULATORS
        iterator& operator++() { ++static_cast<base_type&>(*this); return *this; }
        iterator& operator--() { --static_cast<base_type&>(*this); return *this; }
        reference operator*()
            // Return the non-modifiable descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type(*this));
        }
        // ACCESSORS
        bool operator==(iterator rhs)
        {
            return base_type(*this) == base_type(rhs);
        }
        value_type operator*() const
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return &(*base_type(*this));
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

    static iterator container_add(ValueType& x, type& container)
        // This utility adds an element at the  
        // specified 'container'.
    {
        return (container.insert(x)).first;
    }
    
    static void container_remove(iterator it, type& container)
        // This utility removes the element with specified iterator
        // 'it' from the specified 'container'.
    {
        container.erase(it);
    }

    static void container_remove(descriptor x, type& container)
        // This utility removes the element with specified descriptor
        // 'x' from the specified 'container'.
    {
        container.erase(container.find(x));
    }

    static const ValueType& value(descriptor x, type& container)
            // This utility returns the value associated with the specified
            // descriptor 'x' of the specified 'container'.
    {
        (void)container;  // avoid unused variable warning
        return *x;
    }
};

template <typename ValueType>
struct container_gen<vecS, ValueType> {

    // TYPES
    typedef vecS selector;
        // The container selector, here a 'vecS'.

    typedef std::vector<ValueType> type;
        // The container type, here a 'std::vector'.

    typedef typename std::vector<ValueType>::size_type size_type;
        // The container size type.

    typedef typename std::vector<ValueType>::size_type  descriptor;
        // Type used to describe an element in the collection of elements
        // present in a given container.

    struct descriptor_proxy {
        // This type is a proxy for a descriptor reference.

        // TYPES
        typedef typename std::vector<ValueType>::iterator  base_type;
        typedef typename std::vector<ValueType>::size_type value_type;
        // DATA 
        base_type m_begin;
        value_type m_offset;
        // CREATORS
        descriptor_proxy(base_type begin, value_type offset)
        : m_begin(begin), m_offset(offset) {}
        // MANIPULATORS
        descriptor_proxy& operator=(value_type x) {
            *(m_begin+m_offset) = m_begin+x;
            return *this;
        }
        // ACCESSORS
        operator value_type() const {
            return m_offset;
        }
    };

    struct iterator : public std::vector<ValueType>::iterator {
        // Iterator type over a given container.  Note: the value type must be
        // a descriptor, so we cannot use a 'std::vector:iterator'.  Instead,
        // we create a very similar type, but override the 'operator*'.

        typedef typename std::vector<ValueType>::iterator base_type;
        // DATA
        std::vector<ValueType> *m_container;
      public:
        // TYPES
        typedef typename std::vector<ValueType>::size_type value_type;
        typedef descriptor_proxy                           reference;
        typedef value_type*                                pointer;
        typedef typename base_type::difference_type        difference_type;
        typedef typename base_type::iterator_category      iterator_category;
        // CREATORS
        iterator()
            // Create an unitialized iterator.
        : m_container(0) {}
        iterator(std::vector<ValueType> *container, base_type it)
            // Create an iterator pointing to the same element as the specified
            // 'it' iterator in the specified 'container'.
        : base_type(it), m_container(container) {}
        // MANIPULATORS
        iterator& operator++() { ++static_cast<base_type&>(*this); return *this; }
        iterator& operator--() { --static_cast<base_type&>(*this); return *this; }
        reference operator*()
            // Return a modifiable descriptor of the element pointed to by this
            // iterator.
        {
            return descriptor_proxy(m_container->begin(),
                                    base_type(*this) - m_container->begin());
        }
        // ACCESSORS
        bool operator==(iterator rhs)
        {
            return base_type(*this) == base_type(rhs);
        }
        reference operator*()  const
            // Return the descriptor of the element pointed to by this
            // iterator.
        {
            return descriptor_proxy(m_container->begin(),
                                    base_type(*this) - m_container->begin());
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
        // This utility returns an iterator to the end of the 
        // specified 'container'.
    {
        return iterator(&container, container.end());
    }

    static iterator container_add(ValueType x, type& container)
        // This utility adds an element at the end of the 
        // specified 'container'.
    {
        container.push_back(x);
        return --container_end(container);
    }
    
    static void container_remove(descriptor x, type& container)
        // This utility removes the element with specified descriptor
        // 'x' from the specified 'container'.
    {
        container.erase(container.begin()+x);
    }

    static void container_remove(iterator it, type& container)
        // This utility removes the element with specified iterator
        // 'it' from the specified 'container'.
    {
        container.erase(it);
    }

    static ValueType& value(descriptor x, type& container)
            // This utility returns the value associated with the specified
            // descriptor 'x' of the specified 'container'.
    {
        return container[x];
    }
};

} // namespace hdstl
} // namespace boost

#endif
