// Header file container_proxy.hpp
//
// Copyright (c) 2003 Raoul M. Gough
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)
//
// Class invariant:
//   size() == m_proxies.size()
//   for 0 <= i < size()
//     m_proxies[i].get() != 0
//     m_proxies[i]->owner() == this
//     m_proxies[i]->index() == i
//     m_proxies[i]->m_element_ptr.get() == 0
//
// History
// =======
// 2003/ 8/26   rmg     File creation
// 2003/10/23   rmg     Change pointer container from map to sequence
//
// $Id$
//

#ifndef BOOST_PYTHON_INDEXING_CONTAINER_PROXY_HPP
#define BOOST_PYTHON_INDEXING_CONTAINER_PROXY_HPP

#include <boost/python/suite/indexing/proxy_iterator.hpp>
#include <boost/python/suite/indexing/shared_proxy_impl.hpp>
#include <boost/python/suite/indexing/element_proxy.hpp>
#include <boost/python/suite/indexing/element_proxy_traits.hpp>
#include <boost/python/suite/indexing/workaround.hpp>

#include <vector>    // Default pointer container
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/python/suite/indexing/container_traits.hpp>
#include <boost/python/suite/indexing/algorithms.hpp>
#include <boost/python/suite/indexing/algo_selector.hpp>

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

  struct vector_generator {
    // Generates vector type for any element type with default allocator
    template<typename Element> struct apply {
      typedef std::vector<Element> type;
    };
  };

  template<class Container
           , class Holder = identity<Container>
           , class Generator = vector_generator>
  class container_proxy
  {
    typedef container_proxy<Container, Holder, Generator> self_type;
    typedef typename Container::iterator raw_iterator;
    typedef ::boost::detail::iterator_traits<raw_iterator> raw_iterator_traits;

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
    typedef proxy_iterator<self_type> const_iterator; // ??

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

    size_type size ()     const { return raw_container().size(); }
    size_type capacity () const { return raw_container().capacity(); }
    void reserve (size_type s);

  public:
    iterator begin() { return iterator (this, static_cast<size_type>(0)); }
    iterator end() { return iterator (this, raw_container().size()); }

    iterator erase (iterator);
    iterator erase (iterator, iterator);
    iterator insert (iterator, raw_value_type const &);
    template<typename Iter> void insert (iterator, Iter, Iter);

    void push_back (raw_value_type const &copy) { insert (end(), copy); }

    value_type pop_back () {
      value_type result = at (size() - 1);
      erase (end() - 1);
      return result;
    }

  public:
    // These functions are useful only when client code has direct
    // non-const acccess to the raw container (e.g. via an indirect
    // holder supplied to our constructor). Any code that directly
    // modifies the contents of the raw container (by replacing,
    // inserting or erasing elements) must notify the container_proxy.

    void detach_proxy (size_type index);
    void detach_proxies (size_type from, size_type to);
    // Call before overwriting element(s) in the raw container

    void prepare_erase (size_type from, size_type to);
    // Call before erasing elements directly from the raw container

    void notify_insertion (size_type from, size_type to);
    // Call after inserting elements directly into the raw container

  public:
    // Convenient replacement of elements (automatic proxy detachment)
    void replace (size_type index, raw_value_type const &);
    template<typename Iter> void replace (size_type index, Iter, Iter);

    void swap_elements (size_type index1, size_type index2);

    bool is_valid () const; // Check the class invariant (for testing purposes)

  private:
    // Overloads for insertions with/without useful std::distance
    template<typename Iter>
    void insert (iterator, Iter, Iter, std::forward_iterator_tag);

    template<typename Iter>
    void insert (iterator, Iter, Iter, std::input_iterator_tag);

  private:
    typedef boost::shared_ptr<shared_proxy> pointer_impl;
    typedef typename Generator::template apply<pointer_impl>::type
        pointer_container;
    typedef typename pointer_container::iterator pointer_iterator;

  private:
    Container &raw_container();

    void adjust_proxies (pointer_iterator, pointer_iterator, difference_type);
    void write_proxies (size_type, size_type);
    bool clear_proxy (pointer_impl &);         // detach and do not reset
    void clear_proxies (size_type, size_type); // detach and do not reset

  private:
    held_type m_held_obj;
    pointer_container m_proxies;
  };

  template<class Container, class Holder, class Generator>
  container_proxy<Container, Holder, Generator>
  ::container_proxy ()
    : m_held_obj (Holder::create())
    , m_proxies ()
  {
    // Container is empty - no further processing
  }

  template<class Container, class Holder, class Generator>
  container_proxy<Container, Holder, Generator>
  ::container_proxy (held_type const &held)
    : m_held_obj (Holder::copy (held))
    , m_proxies (size())
  {
    write_proxies (0, size());
  }

  template<class Container, class Holder, class Generator>
  template<typename Iter>
  container_proxy<Container, Holder, Generator>
  ::container_proxy (Iter start, Iter finish)
    : m_held_obj (Holder::create())
    , m_proxies ()
  {
    insert (begin(), start, finish);
  }

  template<class Container, class Holder, class Generator>
  container_proxy<Container, Holder, Generator>
  ::container_proxy (container_proxy const &copy)
    : m_held_obj (Holder::copy (copy.m_held_obj))
    , m_proxies (size())
  {
    write_proxies (0, size()); // Create our own proxies for the copied values
  }

  template<class Container, class Holder, class Generator>
  container_proxy<Container, Holder, Generator> &
  container_proxy<Container, Holder, Generator>
  ::operator= (container_proxy const &copy)
  {
    // Copy original values into any proxies being shared by external pointers
    clear_proxies (0, size());

    Holder::assign (m_held_obj, copy.m_held_obj);

    m_proxies.resize (size());
    write_proxies (0, m_proxies.size());

    return *this;
  }

  template<class Container, class Holder, class Generator>
  container_proxy<Container, Holder, Generator>
  ::~container_proxy ()
  {
    // Copy original values into any proxies being shared by external pointers
    clear_proxies (0, size());
    Holder::pre_destruction (m_held_obj);
  }

  template<class Container, class Holder, class Generator>
  Container &
  container_proxy<Container, Holder, Generator>
  ::raw_container ()
  {
    return Holder::get (m_held_obj);
  }

  template<class Container, class Holder, class Generator>
  Container const &
  container_proxy<Container, Holder, Generator>
  ::raw_container () const
  {
    return Holder::get (m_held_obj);
  }

  template<class Container, class Holder, class Generator>
  void container_proxy<Container, Holder, Generator>::reserve (size_type size)
  {
    raw_container().reserve (size);
    m_proxies.reserve (size);
  }

  template<class Container, class Holder, class Generator>
  typename container_proxy<Container, Holder, Generator>::reference
  container_proxy<Container, Holder, Generator>
  ::at (size_type index)
  {
    pointer_impl const &ptr (m_proxies.BOOST_INDEXING_AT (index));
    assert (ptr->owner() == this);
    assert (ptr->index() == index);
    return reference (ptr);
  }

  template<class Container, class Holder, class Generator>
  typename container_proxy<Container, Holder, Generator>::const_reference
  container_proxy<Container, Holder, Generator>
  ::at (size_type index) const
  {
    pointer_impl const &ptr (m_proxies.BOOST_INDEXING_AT (index));
    assert (ptr->owner() == this);
    assert (ptr->index() == index);
    return const_reference (ptr);
  }

  template<class Container, class Holder, class Generator>
  void
  container_proxy<Container, Holder, Generator>
  ::replace (size_type index, raw_value_type const &copy)
  {
    detach_proxy (index);
    raw_container().BOOST_INDEXING_AT (index) = copy;
    write_proxies (index, index + 1);
  }

  template<class Container, class Holder, class Generator>
  template<typename Iter>
  void
  container_proxy<Container, Holder, Generator>
  ::replace (size_type index, Iter from, Iter to)
  {
    while (from != to)
      {
        replace (index++, *from++);
      }
  }

  template<class Container, class Holder, class Generator>
  void
  container_proxy<Container, Holder, Generator>
  ::swap_elements (size_type index1, size_type index2)
  {
    pointer_impl &ptr1 (m_proxies[index1]);
    pointer_impl &ptr2 (m_proxies[index2]);

    assert (ptr1->owner() == this);
    assert (ptr2->owner() == this);
    assert (ptr1->index() == index1);
    assert (ptr2->index() == index2);

    // Swap produces the diagrammed transformation. Any external
    // pointers that refer to proxy1 or proxy2 will end up still
    // pointing to their original (now relocated) values.
    //
    // .. ptr1 .. ptr2 ..     .. ptr1 .. ptr2  ..  (m_proxies)
    //      |      |                 \  /
    //      |      |                  \/
    //      |      |                  /\.
    //      V      V                 /  \.
    //   proxy1  proxy2  -->    proxy1  proxy2
    //      |      |                 \  /
    //      |      |                  \/
    //      |      |                  /\.
    //      V      V                 /  \.
    //  .. v1 ... v2 ..         .. v2 .. v1 ..  (raw_container)

    std::swap (ptr1->m_index, ptr2->m_index);
    std::swap (ptr1, ptr2);
    std::swap (raw_container()[index1], raw_container()[index2]);

    assert (m_proxies[index1]->index() == index1);
    assert (m_proxies[index2]->index() == index2);
  }

  template<class Container, class Holder, class Generator>
  typename container_proxy<Container, Holder, Generator>::iterator
  container_proxy<Container, Holder, Generator>::erase (iterator iter)
  {
    return erase (iter, iter + 1);
  }

  template<class Container, class Holder, class Generator>
  typename container_proxy<Container, Holder, Generator>::iterator
  container_proxy<Container, Holder, Generator>::erase (
      iterator from, iterator to)
  {
    assert (from.ptr == this);
    assert (to.ptr == this);

    // Detach and remove the proxies for the about-to-be-erased elements
    prepare_erase (from.index, to.index);

    // Erase the elements from the real container
    raw_iterator result
      = raw_container().erase (
          raw_container().begin() + from.index
          , raw_container().begin() + to.index);

    return iterator (this, result);
  }

  template<class Container, class Holder, class Generator>
  template<typename Iter>
  void container_proxy<Container, Holder, Generator>::insert (
      iterator iter, Iter from, Iter to, std::forward_iterator_tag)
  {
    assert (iter.ptr == this);
    size_type count = std::distance (from, to);

    // Add empty proxy pointers for the new value(s) (could throw)
    m_proxies.insert (m_proxies.begin() + iter.index, count, pointer_impl());

    try
      {
        // Insert the new element(s) into the real container (could throw)
        raw_container().insert (
            raw_container().begin() + iter.index
            , from
            , to);

        try
          {
            // Create new proxies for the new elements (could throw)
            write_proxies (iter.index, iter.index + count);
          }

        catch (...)
          {
            raw_container().erase (
                raw_container().begin() + iter.index
                , raw_container().begin() + iter.index + count);

            throw;
          }
      }

    catch (...)
      {
        m_proxies.erase (
            m_proxies.begin() + iter.index
            , m_proxies.begin() + iter.index + count);

        throw;
      }

    // Adjust any proxies after the inserted elements (nothrow)
    adjust_proxies (
        m_proxies.begin() + iter.index + count
        , m_proxies.end()
        , static_cast<difference_type> (count));
  }

  template<class Container, class Holder, class Generator>
  typename container_proxy<Container, Holder, Generator>::iterator
  container_proxy<Container, Holder, Generator>::insert (
      iterator iter, raw_value_type const &copy)
  {
    // Use the iterator-based version by treating the value as an
    // array of size one (see section 5.7/4 of the C++98 standard)
    insert (iter, &copy, (&copy) + 1, std::random_access_iterator_tag());

    return iter;
  }

  template<class Container, class Holder, class Generator>
  template<typename Iter>
  void container_proxy<Container, Holder, Generator>::insert (
      iterator iter, Iter from, Iter to, std::input_iterator_tag)
  {
    // insert overload for iterators where we *can't* get distance()
    // so just insert elements one at a time
    while (from != to)
      {
        iter = insert (iter, *from++) + 1;
      }
  }

  template<class Container, class Holder, class Generator>
  template<typename Iter>
  void container_proxy<Container, Holder, Generator>::insert (
      iterator iter, Iter from, Iter to)
  {
    // Forward insertion to the right overloaded version
    typedef typename BOOST_ITERATOR_CATEGORY<Iter>::type category;
    insert (iter, from, to, category());
  }

  template<class Container, class Holder, class Generator>
  bool container_proxy<Container, Holder, Generator>::clear_proxy (
      pointer_impl &ptr)
  {
    // Warning - this can break the class invariant. Use only when the
    // pointer is about to be overwritten or removed from m_proxies

    assert (ptr->owner() == this);

    if (!ptr.unique())
      {
        ptr->detach (); // Cause proxy to copy element value
        return true;
      }

    else
      {
        // If the pointer isn't shared, don't bother causing a copy of
        // the container element, since the proxy is about to be
        // deleted or reused.
        return false;
      }
  }

  template<class Container, class Holder, class Generator>
  void container_proxy<Container, Holder, Generator>::clear_proxies (
      size_type from_index, size_type to_index)
  {
    while (from_index != to_index)
      {
        clear_proxy (m_proxies[from_index]);
        ++from_index;
      }
  }

  template<class Container, class Holder, class Generator>
  void container_proxy<Container, Holder, Generator>
  ::detach_proxy (size_type index)
  {
    pointer_impl &ptr (m_proxies[index]);

    assert (ptr->index() == index);

    if (clear_proxy (ptr))
      {
        // To maintain class invariant
        ptr.reset (new shared_proxy (this, index));
      }
  }

  template<class Container, class Holder, class Generator>
  void container_proxy<Container, Holder, Generator>::detach_proxies (
      size_type from_index, size_type to_index)
  {
    while (from_index != to_index)
      {
        detach_proxy (from_index);
        ++from_index;
      }
  }

  template<class Container, class Holder, class Generator>
  void container_proxy<Container, Holder, Generator>
  ::prepare_erase (size_type from_index, size_type to_index)
  {
    difference_type deleting = to_index - from_index;
    pointer_iterator erase_begin = m_proxies.begin() + from_index;
    pointer_iterator erase_end = m_proxies.begin() + to_index;

    // Adjust the indexes of any trailing proxies
    adjust_proxies (erase_end, m_proxies.end(), -deleting);

    // Detach any proxies without updating our pointers to them
    clear_proxies (from_index, to_index);

    // Remove the pointers
    m_proxies.erase (erase_begin, erase_end);
  }

  template<class Container, class Holder, class Generator>
  void container_proxy<Container, Holder, Generator>::notify_insertion (
      size_type from_index, size_type to_index)
  {
    size_type count = to_index - from_index;

    m_proxies.insert (
        m_proxies.begin() + from_index, count, pointer_impl());

    try
      {
        write_proxies (from_index, to_index); // Could throw
      }

    catch (...)
      {
        m_proxies.erase (
            m_proxies.begin() + from_index
            , m_proxies.begin() + to_index);

        throw;
      }

    // Adjust any proxies after the inserted elements (nothrow)
    adjust_proxies (
        m_proxies.begin() + to_index
        , m_proxies.end()
        , static_cast<difference_type> (count));
  }

  template<class Container, class Holder, class Generator>
  void container_proxy<Container, Holder, Generator>::adjust_proxies (
      pointer_iterator from
      , pointer_iterator to
      , difference_type offset)
  {
    while (from != to)
      {
        (*from)->m_index += offset;
        ++from;
      }
  }

  template<class Container, class Holder, class Generator>
  void container_proxy<Container, Holder, Generator>::write_proxies (
      size_type from, size_type to)
  {
    // (over)write proxy pointers in the given range. Re-uses existing
    // shared_proxy objects where possible. Does not call detach_proxy
    // since it is assumed that the original values could have already
    // been modified and copying them now would be wrong.

    while (from != to)
      {
        pointer_impl &ptr (m_proxies[from]);

        if ((ptr.get() == 0) || (!ptr.unique()))
          {
            // Either no proxy yet allocated here, or there is one
            // but it is being shared by an external pointer.
            ptr.reset (new shared_proxy (this, from));
          }

        else
          {
            // Re-use the existing object since we have the only pointer to it
            assert (ptr->owner() == this);
            ptr->m_index = from;
          }

        ++from;
      }
  }

  template<class Container, class Holder, class Generator>
  bool container_proxy<Container, Holder, Generator>::is_valid () const
  {
    bool ok = size() == m_proxies.size(); // Sizes must match

    for (size_type count = 0; ok && (count < size()); ++count)
      {
        pointer_impl const &ptr (m_proxies[count]);

        ok = ptr.get() && (ptr->owner() == this) && (ptr->index() == count)
          && !ptr->m_element_ptr.get();
      }

    return ok;
  }

  /////////////////////////////////////////////////////////////////////////
  // ContainerTraits implementation for container_proxy instances
  /////////////////////////////////////////////////////////////////////////

  template<typename Container>
  struct container_proxy_traits : public default_sequence_traits<Container>
  {
    typedef Container container;
    typedef typename container::raw_value_type value_type; // insert, ...
    typedef typename container::raw_value_type key_type;   // find, count, ...
    typedef typename container::reference reference;       // return values

    typedef typename boost::call_traits<value_type>::param_type value_param;
    typedef typename boost::call_traits<key_type>::param_type   key_param;

    typedef value_traits<reference> value_traits_;
    // Get value_traits for the reference type (i.e. element_proxy)
    // to get the custom visitor_helper
  };

  namespace detail {
    ///////////////////////////////////////////////////////////////////////
    // algo_selector support for std::list instances
    ///////////////////////////////////////////////////////////////////////

    template <typename RawContainer, typename Holder, typename Generator>
    class selector_impl<container_proxy<RawContainer, Holder, Generator> >
    {
      typedef container_proxy<RawContainer, Holder, Generator> Container;

      typedef container_proxy_traits<Container>       mutable_traits;
      typedef container_proxy_traits<Container const> const_traits;

    public:
      typedef default_algorithms<mutable_traits> mutable_algorithms;
      typedef default_algorithms<const_traits>   const_algorithms;
    };
  }
} } }

#endif // BOOST_PYTHON_INDEXING_CONTAINER_PROXY_HPP
