//  boost/rank_list/detail/rl_npsv.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_npsv.hpp
  ------------------

  Methods for "Non-Proportional Sequence View" support:

  npsv_update_sums(): udate width sums O(1) or O(N)*
  npsv_width(): get total width O(1) or O(N)*
  npsv_width(it): get an element's width O(1)
  npsv_set_width(): set an element's width O(log N) or O(1)**
  npsv_pos_of(): get an element's position O(log N) or O(N)*
  npsv_at_pos(): get elem. of a position O(log N) or O(N)*
  (*) width sums need to be updated
  (**) don't update width sums (lazy mode)
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_NON_PROPORTIONAL_SEQUENCE_VIEW_HPP_
#define _BOOST_RANK_LIST_NON_PROPORTIONAL_SEQUENCE_VIEW_HPP_

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

// npsv_update_sums(): Update the m_total_width field of
// every node in the tree. This is achieved in O(N) time
// with a post-order traversal of the tree. This method
// will be called when, after changing NPSV widths in the
// lazy mode (see npsv_set_width()), they are required for
// any operation. If the lazy mode is not used, or if the
// sums are already up to date, then this method is a nop.
//
// Complexity: O(N)

template<class T,class A,class W,class P>
//not inline
  void
  rank_list<T,A,W,P>::npsv_update_sums () const
{
  node_t * p;

  if (!m_sums_out_of_date)    // Already ok?
    return;                   // get out

  p = node_t::m_next;   // Go to leftmost node in the tree

  if (!p->m_parent)               // If the rank_list is empty
  {                               // just reset the
    p->m_total_width = W(0);      // width, clear the
    m_sums_out_of_date = false;   // NPSV dirty flag
    return;                       // and get out
  }

  for (;;)
  {                                              // Go down as
    while (p->m_children[L] || p->m_children[R]) // deep as
      p = p->m_children[L] ?                     // possible
          p->m_children[L] : p->m_children[R];   // (preferably
                                                 // left)
    p->m_total_width = p->m_node_width;

    while (p->m_parent->m_children[R]==p ||
           !p->m_parent->m_children[R])          // If p is the
    {                                            // last child
      p = p->m_parent;                           // update, go
                                                 // up an see
      p->m_total_width = p->left_width () +      // again
                         p->right_width () +
                         p->m_node_width;

      if (!p->m_parent)                // If we reached the
      {                                // root, it's done
        m_sums_out_of_date = false;
        return;
      }
    }
                                       // p is the left child,
    p = p->m_parent->m_children[R];    // go to the right one
  }
}

// npsv_width(): retrieve the total width of the rlist
// (the position of end in the alternative sequence (in
// which every node can occupy a different width instead
// of just one unit).
//
// Complexity: O(1), or O(N) if sums were not up to date

template<class T,class A,class W,class P>
inline
  W rank_list<T,A,W,P>::npsv_width () const
{
  if (m_sums_out_of_date)
    npsv_update_sums ();

  return node_t::m_total_width;
}

// npsv_width(): retrieve the width of a node (the
// width is the amount of 'space' it occupies in the
// alternative sequence).
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  W rank_list<T,A,W,P>::npsv_width
  (typename rank_list<T,A,W,P>::const_iterator it) const
{
  RL_ASSERT (it.ptr);          // it must point somewhere
  return it.ptr->m_node_width;
}

// npsv_set_width(): modify the width of a node (the
// width is the amount of 'space' it occupies in the
// alternative sequence). If the third parameter is false,
// don't update width sums (it will be done later for the
// whole tree). This lazy technique will save time in
// those cases where many widths need to be updated in a
// row, getting O(N) complexity instead of O(n log N).
//
// Complexity: O(log N), or O(1) if update_sums==false

template<class T,class A,class W,class P>
//not inline
  void
  rank_list<T,A,W,P>::npsv_set_width
  (const typename rank_list<T,A,W,P>::iterator & it,
   W w,
   bool update_sums)
{
  node_t * p;

  RL_ASSERT (it.ptr);                 // it must point somewhere
  RL_ASSERT_HO (owner(it.ptr)==this); // it must point here
  RL_ASSERT (w>=W(0));                // Neg. width is forbidden

  RL_ASSERT_EXC (it.ptr->m_parent,       // Can't change
                 invalid_op_with_end()); // end's width

  if (it.ptr->m_node_width==w)  // If the width w is not new
    return;                     // there's nothing to do

  it.ptr->m_node_width = w;     // Set the new width

  if (update_sums)              // If required, update all sums
  {                             // of the tree, or just climb
    if (m_sums_out_of_date)     // to the root updating sums
      npsv_update_sums ();      // in the way
    else
      for (p=it.ptr; p; p=p->m_parent)
        p->m_total_width = p->left_width () +
                           p->right_width () +
                           p->m_node_width;
  }
  else                          // If no sums update,
    m_sums_out_of_date = true;  // set NPSV dirty bit
}

// npsv_pos_of(): given a node, calculate its position
// in the alternative sequence (in which every node can
// occupy a different width instead of just one unit).
//
// Complexity: O(log N), or O(N) if sums are out of date

template<class T,class A,class W,class P>
//not inline
  W
  rank_list<T,A,W,P>::npsv_pos_of
  (typename rank_list<T,A,W,P>::const_iterator it) const
{
  W pos;
  const node_t * p, * parent;

  p = it.ptr;

  RL_ASSERT (p);  // NULL pointer dereference

  if (m_sums_out_of_date)
    npsv_update_sums ();

  if (!p->m_parent)               // Already in the dummy node?
    return node_t::m_total_width;
                                  // Otherwise, start with the
  for (pos=p->left_width();       // left width of the node and
       p->m_parent;               // climb up to the root adding
       p=parent)                  // the widths of nodes that stay
  {                               // in the left of the path
    parent = p->m_parent;
                                        // That is, when we
    if (parent->m_children[R]==p)       // step up-left, the
      pos += parent->left_width () +    // width of the parent
             parent->m_node_width;      // node and its left
  }                                     // width

  return pos;
}

// npsv_at_pos (): find a node, given its position in the
// alternative sequence (in which every node can occupy a
// different width instead of just one unit).
// It travels down from the root to the searched node. This
// takes logarithmic time both on average and in the worst
// case.
//
// Complexity: O(log N), or O(N) if sums are out of date

template<class T,class A,class W,class P>
//not inline
  typename rank_list<T,A,W,P>::iterator
  rank_list<T,A,W,P>::npsv_at_pos
  (W pos)
{
  node_t * p;
  W left, right;

  if (m_sums_out_of_date)
    npsv_update_sums ();

  if (size()==0 || pos<W(0) ||
      pos>node_t::m_total_width ||     // Out of bounds --> end
      (pos==node_t::m_total_width &&
       node_t::m_prev->m_node_width!=W(0)))
    return dummy ();

  p = node_t::m_children[L]; // Start with the element at the
                             // root (remember that the dummy
                             // node has no T element)

      // For every subtree, the position (in it) of its root
      // node is equal to the left width of this root node...

  while (p)                 // Travel down updating pos to make
  {                           // it the index in the visited
    left = p->left_width ();    // subtree, until it fits the
                                // index of the subtree's
    right = left +              // root node.
            p->m_node_width;    // A step down-left doesn't
                                  // touch pos, while a step
    if (pos<left ||                 // down-right decreases pos
        (p->m_children[L] &&               // by the sum of the
         pos==left &&                           // widths of
         p->m_prev->m_node_width==W(0)))       // the nodes
      p = p->m_children[L];                   // we leave at
    else if (pos<right ||                    // the left side
             (pos==right &&                 // (including the
              p->m_node_width==W(0)))      // subtree's root)
      return p;
    else                       // Note that there might be
    {                          // nodes with zero width
      pos -= right;            // (some nodes standing in the
                               // same position of the
      p = p->m_children[R];    // alternative view). In these
    }                          // cases return the first one
  }
                   // We sould never step out of the tree, but
  return dummy (); // don't trust the coherency of W and its
}                  // operators (just in case)

// npsv_at_pos() _const_: See non-const version (above) for
// details.

template<class T,class A,class W,class P>
//not inline
  typename rank_list<T,A,W,P>::const_iterator
  rank_list<T,A,W,P>::npsv_at_pos
  (W pos)                          const
{
  return (const_cast<my_class*>(this))->npsv_node_at_pos (pos);
}

// npsv_at_pos (): find a node, given its position in the
// alternative sequence (in which every node can occupy a
// different width instead of just one unit).
// Use a functor (passed as parameter) instead of the usual
// operators <, >, ==, !=, <=, >=.
// This version of the method is intended for using a
// composite width type. The functor parameter cmp must
// have an overloaded operator() which receives two W
// const references, and returns an integer indicating
// the result of the comparison (<0: first is lesser;
// 0: equal; >0: first is greater).
//
// Complexity: O(log N), or O(N) if sums are out of date

template<class T,class A,class W,class P>
template<class CMP>
//not inline
  typename rank_list<T,A,W,P>::iterator
  rank_list<T,A,W,P>::npsv_at_pos
  (W pos, CMP cmp)
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires<
      BinaryFunctionConcept<CMP,int,const W&,const W&> >();
#endif

  node_t * p;             // See comments in the first version
  W left, right;          // of this method (without cmp). The
  int c;                  // algorithm used here is the same.
                          // The only difference is the
  if (m_sums_out_of_date) // comparison method.
    npsv_update_sums ();

  if (size()==0 ||
      cmp(pos,W(0))<0 ||                            // pos<(W)0
      (c=cmp(pos,node_t::m_total_width))>0 ||       //  " >total_w
      (c==0 &&                                      //  " == "
       cmp(node_t::m_prev->m_node_width,W(0))!=0))  // prev_w != 0
    return dummy ();

  p = node_t::m_children[L];

  while (p)
  {
    left = p->left_width ();

    right = left +
            p->m_node_width;

    if ((c=cmp(pos,left))<0 ||              // pos < left
        (p->m_children[L] &&
         c==0 &&                            // pos == left
         cmp(p->m_prev->m_node_width,W(0))
                                      ==0)) // prev_w == 0
      p = p->m_children[L];
    else if ((c=cmp(pos,right))<0 ||        // pos < right
             (c==0 &&                       // pos == right
              cmp(p->m_node_width,W(0))
                                      ==0)) // p_w == 0
      return p;
    else
    {
      pos -= right;

      p = p->m_children[R];
    }
  }

  return dummy ();
}

// npsv_at_pos() _const_: See non-const version (above) for
// details.

template<class T,class A,class W,class P>
template<class CMP>
inline
  typename rank_list<T,A,W,P>::const_iterator
  rank_list<T,A,W,P>::npsv_at_pos
  (W pos, CMP cmp)                      const
{
#ifdef BOOST_CLASS_REQUIRE
  function_requires<
      BinaryFunctionConcept<CMP,int,const W&,const W&> >();
#endif

  return (const_cast<my_class*>(this))->
                                 npsv_node_at_pos (pos, cmp);
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

