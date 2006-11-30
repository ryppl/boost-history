//  boost/rank_list/detail/rl_erase.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_erase.hpp
  -------------------

  Methods for erasing elements:

  it/rit erase(it/rit): (O(log N))
  it erase(it,n): vector-erase (O(min{N, n log N}))
  rit erase(rit,n): vector-erase (reverse) (O(min{N, n log N}))
  it erase(from,to): range-erase (O(min{N, n log N}))
  rit erase(rfrom,rto): range-erase (reverse) (O(min{N, n log N}))
  clear(): delete all (O(N))

  Private helper methods:

  extract_node(): remove a node (O(1)) [+ later rebalance]
  erase_it(IT): delete one element (O(log N))
  extract_nodes(): remove n nodes (O(min{N, n log N})
  erase_it(IT,n): delete n elements (O(min{N, n log N})
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_ERASE_HPP_
#define _BOOST_RANK_LIST_ERASE_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    using namespace detail;

//////////////////////////////////////////////////////////////////

// ---------------------- PUBLIC INTERFACE -----------------------

// erase(): extract one element (and delete it)
//
// Complexity: O(log(N))

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::iterator
  rank_list<T,A,W,P>::erase
  (typename rank_list<T,A,W,P>::iterator it)
{
  return erase_it (it);
}

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::reverse_iterator
  rank_list<T,A,W,P>::erase
  (typename rank_list<T,A,W,P>::reverse_iterator it)
{
  return erase_it (it);
}

// Vector erase: remove a given number of elements from the
// rlist (and delete them) starting at a given position
//
// Complexity: O(min{N, n log N})

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::iterator
  rank_list<T,A,W,P>::erase
  (typename rank_list<T,A,W,P>::iterator from,
   typename rank_list<T,A,W,P>::size_type n)
{
  return erase_it (from, n); // Just call private templ. method
}

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::reverse_iterator
  rank_list<T,A,W,P>::erase
  (typename rank_list<T,A,W,P>::reverse_iterator from,
   typename rank_list<T,A,W,P>::size_type n)
{
  return erase_it (from, n); // Just call private templ. method
}

// Range erase: remove a given range of elements from the
// rlist (and delete them)
//
// Complexity: O(min{N, n log N})

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::iterator
  rank_list<T,A,W,P>::erase
  (typename rank_list<T,A,W,P>::iterator from,
   typename rank_list<T,A,W,P>::iterator to)
{
  RL_ASSERT_HO (owner(from.ptr)==this); // from and to must point
  RL_ASSERT_HO (owner(to.ptr)==this);   // into this rlist

  return erase_it (from, to-from); // Get the difference and use
}                                  // vector erase

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::reverse_iterator
  rank_list<T,A,W,P>::erase
  (typename rank_list<T,A,W,P>::reverse_iterator from,
   typename rank_list<T,A,W,P>::reverse_iterator to)
{
  RL_ASSERT_HO (owner(from.ptr)==this); // from and to must point
  RL_ASSERT_HO (owner(to.ptr)==this);   // into this rlist

  return erase_it (from, to-from); // Get the difference and use
}                                  // vector erase

// clear(): delete all the contents of the rlist, leaving
// it empty
//
// Complexity: O(N)

template<class T,class A,class W,class P>
void rank_list<T,A,W,P>::clear ()
{
  node_t * p, * q;

  node_t::m_prev->m_next = NULL;
  p = node_t::m_next;

  init ();             // Reset

  while (p)
  {
    q = p;             // Traverse the list
    p = p->m_next;     // deleting every element
    delete_node (q);
  }
}


// ------------------- PRIVATE HELPER METHODS --------------------

// extract_node(): take a given node out of the tree, but don't
// delete it. Depending on the circumstances, we might need to call
// update_counters() or even update_counters_and_rebalance(),
// passing them the address returned by extract_node().
//
// Complexity: O(1)

template<class T,class A,class W,class P>
//not inline
  typename rank_list<T,A,W,P>::node_t *
  rank_list<T,A,W,P>::extract_node
  (typename rank_list<T,A,W,P>::node_t * p)
{
  node_t * q, * r, * w;
  size_type cl, cr;
  int side;

  RL_ASSERT (p);           // NULL pointer dereference

  RL_ASSERT_EXC (p->m_parent,
              invalid_op_with_end()); // Can't extract end()

  q = p->m_parent;

  cl = p->left_count ();
  cr = p->right_count ();

  if (!cl || !cr)      // If one subtree is empty (or both)
  {
    side = cl ? L : R;        // Take the other subtree
                              // (take R if both are empty)
    w = p->m_children[side];  // and bypass the victim node
    r = q;                    // linking the non-empty
                              // subtree directly to the
    if (w)                    // parent of the victim node
      w->m_parent = q;
                              // It must hang from the same
    if (q->m_children[L]==p)  // side the victim did
      q->m_children[L] = w;
    else                      // The potentially unbalanced
      q->m_children[R] = w;   // branch is from the victim's
  }                           // parent and upwards

                              // Ok, no subtree is empty, but
                                               // if ther's
  else if (!p->m_children[L]->m_children[R] || // a hole in
           !p->m_children[R]->m_children[L])   // the inner
  {                                            // places two
                              // levels under the victim node

    side = !p->m_children[L]->m_children[R] ? L : R; // Hole

    r = w = p->m_children[side]; // Pot. unbal.: hole's parent
    w->m_parent = q;

    if (q->m_children[L]==p)  // Put the hole's parent in the
      q->m_children[L] = w;   // place of the victim, adopting
    else                      // the victim's parent and the
      q->m_children[R] = w;   // victim's other subtree

    w->m_children[1-side] = p->m_children[1-side];
    w->m_children[1-side]->m_parent = w;
  }
  else           // Well, no subtree is empty and both inner
  {              // places two levels under the victim are
    if (cl>cr)   // occupied, so both next and previous nodes
    {            // of the victim are down there
      side = L;
      w = p->m_prev;
    }                  // Choose one of them (the one in
    else               // the most populated subtree), and
    {                  // put it in the place of the victim
      side = R;
      w = p->m_next;
    }                  // Potentially unbalanced branch: from
                       // the subsitute's parent and upwards
    r = w->m_parent;
    r->m_children[1-side] =         // The substitute has no
             w->m_children[side];   // child in one side,
                                    // buy it might have one
    if (r->m_children[1-side])      // in the other side
      r->m_children[1-side]->       // The substitute's parent
                      m_parent = r; // must adopt it

    w->m_children[L] = p->m_children[L]; // The substitute
    w->m_children[L]->m_parent = w;      // adopts the
    w->m_children[R] = p->m_children[R]; // victim's children
    w->m_children[R]->m_parent = w;

    w->m_parent = q;
                                // The victim's parent
    if (q->m_children[L]==p)    // adopts the substitute
      q->m_children[L] = w;
    else
      q->m_children[R] = w;
  }

  p->m_next->m_prev = p->m_prev;  // Bypass the victim in the
  p->m_prev->m_next = p->m_next;  // circ. doubly linked list

  return r;  // Potentially unbalanced branch
}            // (from r and upwards until the root)

// erase_it(): extract one element (and delete it). If
// this method was public, it might be called with const
// iterators (wrong). That's why it is private.
//
// Complexity: O(log(N))

template<class T,class A,class W,class P>
template<class IT>
inline
  IT rank_list<T,A,W,P>::erase_it (IT it)
{
#ifdef BOOST_CLASS_REQUIRE
#ifdef RL_USE_RANDOM_ACCESS_TAG
  function_requires<
      Mutable_RandomAccessIteratorConcept<IT> >();
#else
  function_requires<
      Mutable_BidirectionalIteratorConcept<IT> >();
#endif
#endif

  node_t * p, * r;

  RL_ASSERT_HO (owner(it.ptr)==this); // it must point into
                                      // this arr.

  p = it.ptr;
  ++ it;
  r = extract_node (p);
  update_counters_and_rebalance (r);
  delete_node (p);

  return it;
}

// Vector extract: remove a given number of elements from
// the rlist (and return them) starting at a given
// position. The sublist of extracted elements is returned
// via first and last (pointers passed by ref.). The
// parameter reverse indicates how to build the list.
//
// Complexity: (O(min{N, n log N})

template<class T,class A,class W,class P>
template <class IT>
//not inline
  bool                              // Return true iff dst belongs
  rank_list<T,A,W,P>::extract_nodes // to the extracted range

  (IT & from,                                // Source
   typename rank_list<T,A,W,P>::size_type n, // # nodes to extract

   typename rank_list<T,A,W,P>::node_t *& first, // List with
   typename rank_list<T,A,W,P>::node_t *& last,  // extracted nodes

   typename rank_list<T,A,W,P>::node_t * dst, // Dest. to check

   bool * delayed_rebuild,  // In: non-NULL means "delay tree
                            // reconstruction, cause the same tree
                            // is used for destination"
                            // Out: true means "reconstruction was
                            // delayed and the caller should do it
                            // after reinsertion"

   bool reverse) // true means: "destination has been secified with
                 // a reverse iterator, so build a reverse list"
{
#ifdef BOOST_CLASS_REQUIRE
#ifdef RL_USE_RANDOM_ACCESS_TAG
  function_requires<
      Mutable_RandomAccessIteratorConcept<IT> >();
#else
  function_requires<
      Mutable_BidirectionalIteratorConcept<IT> >();
#endif
#endif

  node_t * p, * r;
  size_type i;
  bool dst_extracted;

  first = last = NULL;
  if (n==0) return false;

  dst_extracted = false;

  if (!worth_rebuild(n,size(),true))  // If there are 'few'
  {                                   // elements to extract
    first = last = p = from.ptr;
                                         // Be careful:
    for (i=0; i<n && p->m_parent; i++)   // from can be a
    {                                    // REVERSE iterator!
                                         // (use its ++ optor.)
                                         // and don't extract end

      if (p==dst)                        // Detect src-dest
        dst_extracted = true;            // overlapping

      ++ from;                           // Extract them
      r = extract_node (p);              // one by one, and
      update_counters_and_rebalance (r); // fix the tree
                                         // every time
      if (reverse)
      {                                  // Build the list
        p->m_next = first;               // according to the
        first = first->m_prev = p;       // direction of the
      }                                  // iterator that
      else                               // specified the
      {                                  // destination point
        p->m_prev = last;
        last = last->m_next = p;
      }
                         // Advance. Note that ++from is
      p = from.ptr;      // performed prior to extraction
    }
  }
  else             // If there are 'many' elements to extract
  {
    first = last = p = from.ptr;
                                         // Be careful:
    for (i=0; i<n && p->m_parent; i++)   // from can be a
    {                                    // REVERSE iterator!
                                         // (use its ++ optor.)
                                         // and don't extract end

      if (p==dst)                        // Detect src-dest
        dst_extracted = true;            // overlapping

                                         // Just extract them
      ++ from;                           // from the circular
      p->m_next->m_prev = p->m_prev;     // doubly linked list
      p->m_prev->m_next = p->m_next;     // and reorganize the
                                         // whole tree later
      if (reverse)
      {                                  // Build the list
        p->m_next = first;               // according to the
        first = first->m_prev = p;       // direction of the
      }                                  // iterator that
      else                               // specified the
      {                                  // destination point
        p->m_prev = last;
        last = last->m_next = p;
      }
                         // Advance. Note that ++from is
      p = from.ptr;      // performed prior to extraction
    }

    if (delayed_rebuild)         // The tree is broken now, so
    {                            // rebuild it, or indicate
      *delayed_rebuild = true;   // delayed rebuild and adjust
      node_t::m_count -= n;      // the size for it
    }
    else
      build_known_size_tree (size()-n, node_t::m_next);
  }

  first->m_prev = NULL;    // Isolate the extracted nodes
  last->m_next = NULL;     // list (mark both ends)

  return dst_extracted;
}

    // Vector erase: remove a given number of elements from the
    // rlist (and delete them) starting at a given position. If
    // this method was public, it might be called with const
    // iterators (wrong). That's why it is private.
    //
    // Complexity: n<N/2 ? O(n*log(N)) : O(N)
    // (where N is the number of elements in the rlist and n is
    // the number of elements to erase)

template<class T,class A,class W,class P>
template <class IT>
//not inline
  IT
  rank_list<T,A,W,P>::erase_it
  (IT from,                                  // Where to start
   typename rank_list<T,A,W,P>::size_type n) // # to erase
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires< InputIteratorConcept<IT> >();
#endif

  node_t * p, * first, * last;

  RL_ASSERT_HO (owner(from.ptr)==this);  // from must point
                                         // into this rlist
  if (n==0) return from;

  extract_nodes (from, n, first, last);

  while (n)
  {                         // Destruct removed nodes
    p = first;
    first = first->m_next;
    delete_node (p);
    n --;
  }

  return from;
}




//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

