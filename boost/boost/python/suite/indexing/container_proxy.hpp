// -*- mode:c++; switch-modules-target: "testcontprox.cpp" -*-
//
// Header file container_proxy.hpp
//
// Copyright (c) 2003 Raoul M. Gough
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)
//
// History
// =======
// 2003/ 8/26   rmg     File creation
//
// $Id$
//

#ifndef container_proxy_rmg_20030826_included
#define container_proxy_rmg_20030826_included

#include <boost/python/suite/indexing/proxy_iterator.hpp>
#include <boost/python/suite/indexing/shared_proxy_impl.hpp>
#include <boost/python/suite/indexing/element_proxy.hpp>

#include <map>
#include <memory>
#include <cassert>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace boost { namespace python { namespace indexing {
  template<typename T> struct identity {
    typedef T held_type;

    static T &       get(T &       obj) { return obj; }
    static T const & get(T const & obj) { return obj; }

    static T    create ()                     { return T(); }
    static T    copy   (T const &copy)        { return copy; }
    static void assign (T &to, T const &from) { to = from; }
    static void pre_destruction (T &)         { }
  };

  template<typename P> struct deref {
    typedef P held_type;

    typedef typename boost::iterator_value<P>::type     value;

    static value &       get (P &       ptr)  { return *ptr; }
    static value const & get (P const & ptr)  { return *ptr; }

    static P    create ()                     { return P(); }
    static P    copy   (P const &copy)        { return copy; }
    static void assign (P &to, P const &from) { to = from; }
    static void pre_destruction (P &)         { }
  };

  template<class Container
           , class Holder = identity<Container> >
  class container_proxy
  {
    typedef container_proxy<Container, Holder> self_type;
    typedef typename Container::iterator raw_iterator;
    typedef std::iterator_traits<raw_iterator> raw_iterator_traits;

    template<class C> friend class shared_proxy_impl;
    template<class C> friend class proxy_iterator;

  public:
    typedef typename Holder::held_type held_type;

    typedef typename Container::size_type size_type;
    typedef typename Container::difference_type difference_type;

    typedef shared_proxy_impl<self_type> shared_proxy;

    typedef typename Container::value_type raw_value_type;

    typedef element_proxy<self_type> value_type;
    typedef value_type               reference;  // Already has ref. semantics

    typedef const_element_proxy<self_type> const_value_type;
    typedef const_value_type               const_reference; // Ref. semantics

    typedef proxy_iterator<self_type> iterator;

  public:
    // Constructors
    container_proxy ();
    explicit container_proxy (held_type const &h);
    template<typename Iter> container_proxy (Iter, Iter);

    container_proxy (container_proxy const &);
    container_proxy &operator= (container_proxy const &);
    ~container_proxy ();

    Container const &raw_container() const;   // OK to expose const reference

    reference       at (size_type index);
    const_reference at (size_type index) const;

    reference       operator[] (size_type index)       { return at(index); }
    const_reference operator[] (size_type index) const { return at(index); }

    size_type size() const { return raw_container().size(); }
    size_type capacity() const { return raw_container().capacity(); }
    void reserve(size_type s) { raw_container().reserve(s); }

  public:
    iterator begin() { return iterator (this, static_cast<size_type>(0)); }
    iterator end() { return iterator (this, raw_container().size()); }

    iterator erase (iterator);
    iterator erase (iterator, iterator);
    iterator insert (iterator, raw_value_type const &);
    template<typename Iter> void insert (iterator, Iter, Iter);

    void push_back (raw_value_type const &copy) { insert (end(), copy); }

    value_type pop_back () {
      value_type result = at (end() - 1);
      erase (end() - 1);
      return result;
    }

  public:
    // Proxies for a given index must be detached before overwriting
    // that container element.
    void detach_proxy (size_type index);
    void detach_proxies (size_type from, size_type to);
    void detach_proxies (iterator from, iterator to);

  public:
    // Convenient replacement of elements (automatic proxy detachment)
    void replace (size_type index, raw_value_type const &);
    template<typename Iter> void replace (size_type index, Iter, Iter);

    void swap_elements (size_type index1, size_type index2);

  private:
    // Overloads for insertions with/without useful std::distance
    template<typename Iter>
    void insert (iterator, Iter, Iter, std::forward_iterator_tag);

    template<typename Iter>
    void insert (iterator, Iter, Iter, std::input_iterator_tag);

  private:
    typedef boost::shared_ptr<shared_proxy> pointer_impl;
    typedef std::map<size_type, pointer_impl> MapType;
    typedef typename MapType::iterator MapIterator;
    typedef typename MapType::value_type MapEntry;

  private:
    Container &raw_container();

    static void detach_if_shared (MapEntry const &);

    void adjustIndexes (MapIterator start, MapIterator end, long offset);
    void adjustIndexesReverse (MapIterator start, MapIterator end, long offset);
    void adjustIndex (MapIterator, long offset);

  private:
    held_type myHeldObj;
    MapType myMap;
  };

  template<class Container, class Holder>
  container_proxy<Container, Holder>
  ::container_proxy ()
    : myHeldObj ()
    , myMap ()
  {
  }

  template<class Container, class Holder>
  container_proxy<Container, Holder>
  ::container_proxy (held_type const &heldType)
    : myHeldObj (heldType)
    , myMap ()
  {
  }

  template<class Container, class Holder>
  template<typename Iter>
  container_proxy<Container, Holder>
  ::container_proxy (Iter start, Iter finish)
    : myHeldObj (Holder::create())
    , myMap ()
  {
    insert (begin(), start, finish);
  }

  template<class Container, class Holder>
  container_proxy<Container, Holder>
  ::container_proxy (container_proxy const &copy)
    : myHeldObj (Holder::copy (copy.myHeldObj))
    , myMap ()                      // Do *not* duplicate map
  {
  }

  template<class Container, class Holder>
  container_proxy<Container, Holder> &
  container_proxy<Container, Holder>
  ::operator= (container_proxy const &copy)
  {
    // All of our contained values are about to be dis-owned
    std::for_each (myMap.begin(), myMap.end(), detach_if_shared);
    myMap.clear();
    Holder::assign (myHeldObj, copy.myHeldObj);
    return *this;
  }

  template<class Container, class Holder>
  container_proxy<Container, Holder>
  ::~container_proxy ()
  {
    // All of our contained values are about to be dis-owned
    std::for_each (myMap.begin(), myMap.end(), detach_if_shared);
    Holder::pre_destruction (myHeldObj);
  }

  template<class Container, class Holder>
  Container &
  container_proxy<Container, Holder>
  ::raw_container ()
  {
    return Holder::get (myHeldObj);
  }

  template<class Container, class Holder>
  Container const &
  container_proxy<Container, Holder>
  ::raw_container () const
  {
    return Holder::get (myHeldObj);
  }

  template<class Container, class Holder>
  typename container_proxy<Container, Holder>::reference
  container_proxy<Container, Holder>
  ::at (size_type index)
  {
    pointer_impl &entry = myMap[index];

    if (!entry.get())
      {
        entry.reset (new shared_proxy (this, index));
      }

    return reference (entry);
  }

  template<class Container, class Holder>
  typename container_proxy<Container, Holder>::const_reference
  container_proxy<Container, Holder>
  ::at (size_type index) const
  {
    // const_cast allows insertion into map. Maybe this is wrong, and
    // there can be no const-version of at. Alternatively, maybe
    // the map member variable should be declared mutable.

    container_proxy *mutable_this = const_cast<container_proxy *>(this);

    return const_reference (mutable_this->at (index));
  }

  template<class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::replace (size_type index, raw_value_type const &copy)
  {
    detach_proxy (index);
    raw_container().at(index) = copy;
  }

  template<class Container, class Holder>
  template<typename Iter>
  void
  container_proxy<Container, Holder>
  ::replace (size_type index, Iter from, Iter to)
  {
    while (from != to)
      {
        replace (index++, *from++);
      }
  }

  template<class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::swap_elements (size_type index1, size_type index2)
  {
    MapIterator iter1 = myMap.find (index1);
    MapIterator iter2 = myMap.find (index2);

    long distance = static_cast<long>(index2) - static_cast<long>(index1);

    if ((iter1 == myMap.end()) && (iter2 == myMap.end()))
      {
        // No proxies exist for these indexes.
      }

    else if ((iter1 != myMap.end()) && (iter2 == myMap.end()))
      {
        // Proxy for the first index only
        MapIterator temp (iter1);
        adjustIndexes (iter1, ++temp, distance); // Exactly one element
      }

    else if ((iter1 == myMap.end()) && (iter2 != myMap.end()))
      {
        // Proxy for the second index only
        MapIterator temp (iter2);
        adjustIndexes (iter2, ++temp, -distance);
      }

    else
      {
        // Proxies for both indexes
        std::swap (iter1->second->myIndex, iter2->second->myIndex);
        std::swap (iter1->second, iter2->second);
      }

    std::swap (raw_container()[index1], raw_container()[index2]);
  }

  template<class Container, class Holder>
  typename container_proxy<Container, Holder>::iterator
  container_proxy<Container, Holder>
  ::erase (iterator iter)
  {
    return erase (iter, iter + 1);
  }

  template<class Container, class Holder>
  typename container_proxy<Container, Holder>::iterator
  container_proxy<Container, Holder>
  ::erase (iterator from, iterator to)
  {
    assert (from.ptr == this);
    assert (to.ptr == this);

    size_type deleting = to.index - from.index;
    MapIterator erase_begin = myMap.lower_bound (from.index);
    MapIterator erase_end = myMap.lower_bound (to.index);

    // Detach any proxies for the soon-to-be-erased elements
    std::for_each (erase_begin, erase_end, detach_if_shared);
    myMap.erase (erase_begin, erase_end);  // Note: erase_end remains valid

    // Adjust the indexes of any following proxies
    adjustIndexes (erase_end, myMap.end(), -deleting);

    // Erase the elements from the real container
    raw_iterator result
      = raw_container().erase (raw_container().begin() + from.index
                               , raw_container().begin() + to.index);

    return iterator (this, result);
  }

  template<class Container, class Holder>
  typename container_proxy<Container, Holder>::iterator
  container_proxy<Container, Holder>
  ::insert (iterator iter, raw_value_type const &copy)
  {
    assert (iter.ptr == this);

    // Adjust indexes from iter.index onwards, since insert goes before this element
    adjustIndexesReverse (myMap.lower_bound (iter.index)
			  , myMap.end()
			  , 1);

    // Insert the element into the real container
    raw_iterator result
      = raw_container().insert (raw_container().begin() + iter.index, copy);

    return iterator (this, result);
  }

  template<class Container, class Holder>
  template<typename Iter>
  void
  container_proxy<Container, Holder>
  ::insert (iterator iter, Iter from, Iter to)
  {
    // Forward insertion to the right overloaded version
    typedef typename std::iterator_traits<Iter>::iterator_category category;
    insert (iter, from, to, category());
  }

  template<class Container, class Holder>
  template<typename Iter>
  void
  container_proxy<Container, Holder>
  ::insert (iterator iter, Iter from, Iter to, std::forward_iterator_tag)
  {
    // insert overload for iterators where we can get distance()

    assert (iter.ptr == this);

    // Adjust indexes from iter.index onwanrds (insert goes before this element)
    adjustIndexesReverse (myMap.lower_bound (iter.index)
			  , myMap.end()
			  , std::distance (from, to));

    // Insert the element into the real container
    raw_container().insert (raw_container().begin() + iter.index, from, to);
  }

  template<class Container, class Holder>
  template<typename Iter>
  void
  container_proxy<Container, Holder>
  ::insert (iterator iter, Iter from, Iter to, std::input_iterator_tag)
  {
    // insert overload for iterators where we *can't* get distance()
    // so just insert elements one at a time
    while (from != to)
      {
        iter = insert (iter, *from++) + 1;
      }
  }

  template<class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::detach_if_shared (MapEntry const &ent)
  {
    if (!ent.second.unique())
      {
        ent.second->detach();
      }
    // If the pointer isn't shared, don't bother causing a copy of the
    // container element, since the proxy is about to be deleted.
  }

  template<class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::detach_proxy (size_type index)
  {
    MapIterator iter = myMap.find (index);

    if (iter != myMap.end())
      {
        detach_if_shared (*iter);
        myMap.erase (iter);
      }
  }

  template<class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::detach_proxies (size_type fromIndex, size_type toIndex)
  {
    MapIterator from = myMap.lower_bound (fromIndex);
    MapIterator to = myMap.lower_bound (toIndex);
    std::for_each (from, to, detach_if_shared);
    myMap.erase (from, to);
  }

  template<class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::detach_proxies (iterator from, iterator to)
  {
    assert (from.ptr == this);
    assert (to.ptr == this);
    detach_proxies (from.index, to.index);
  }

  template<class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::adjustIndex (MapIterator iter, long offset)
  {
    pointer_impl ptr (iter->second);  // Copy the shared pointer
    myMap.erase (iter);               // Remove the map copy of it

    if (!ptr.unique())
      {
	// Reinsert only if there are other pointers "out there"
	// referring to the shared proxy

	ptr->myIndex += offset;
	myMap.insert (typename MapType::value_type (ptr->myIndex, ptr));
      }
  }

  template<class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::adjustIndexes (MapIterator low_bound, MapIterator high_bound, long offset)
  {
    // Adjust indexes in the given range of proxies by the given offset.
    // The adjustment is done by erasing and re-inserting the entries
    // in the map.
    //
    // Could provide a hint iterator to the map insertion calls, except
    // in the case that "low_bound" is right at the start of the container
    // (the hint must be the element *before* the one to be inserted,
    // and there is no element before the first one). This would mean
    // additional complexity to deal with the special case somehow.

    while (low_bound != high_bound)
      {
        MapIterator next (low_bound);
	++next;  // Find next node before erasing the current target

	adjustIndex (low_bound, offset);

        low_bound = next;
      }
  }

  template <class Container, class Holder>
  void
  container_proxy<Container, Holder>
  ::adjustIndexesReverse (MapIterator low_bound
			  , MapIterator high_bound
			  , long offset)
  {
    while (low_bound != high_bound)
      {
	--high_bound;  // Adjust now because high_bound is one-past-the-end

	if (high_bound == low_bound)
	  {
	    adjustIndex (high_bound, offset);  // Last one to adjust
	  }

	else
	  {
	    MapIterator temp (high_bound);
	    --temp;   // Find previous node before doing erase

	    adjustIndex (high_bound, offset);   // Do erase

	    high_bound = temp;
	    ++high_bound;   // Make one-past-the-end again
	  }
      }
  }

} } }

#endif // container_proxy_rmg_20030826_included
