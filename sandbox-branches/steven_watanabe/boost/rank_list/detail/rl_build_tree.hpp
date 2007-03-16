//  boost/rank_list/detail/rl_build_tree.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_build_tree.hpp
  ------------------------

  Private helper methods for massive operations (tree building)

  worth_rebuild(): decide how to perform a massive op. (O(1))
  build_known_size_tree(): make a tree of a given size (O(N))
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_BUILD_TREE_HPP_
#define _BOOST_RANK_LIST_BUILD_TREE_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    using namespace detail;

//////////////////////////////////////////////////////////////////

// ------------------- PRIVATE HELPER METHODS --------------------

// worth_rebuild(): decide whether a massive operation is worth
// rebuilding the whole tree (O(N)), or it is better to
// insert/extract the nodes one by one (O(n log N)).
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  bool
  rank_list<T,A,W,P>::worth_rebuild
  (typename rank_list<T,A,W,P>::size_type n, // # to insert/erase
   typename rank_list<T,A,W,P>::size_type N, // Current size
   bool erase)                               // true=erase,
{                                            //   false=insert
  size_type average_size, final_size, ratio;

  if (n<=1)               // We need to choose between
    return false;         // O(n log N) and O(N), or more
                          // exactly: O(n log(average_size))
                          // and O(final_size). If the latter
  if (erase)              // is lesser, it is worth rebuilding
  {                       // the whole tree.
    RL_ASSERT (N>=n);
                            // Demo:
    average_size = N - n/2; //
    final_size = N - n;     // n log2(aver) >    final
  }                         //              "
  else                      //  log2(aver)  >   final/n
  {                         //              "
    average_size = N + n/2; //     aver     >  pow(2,final/n)
    final_size = N + n;     //              "
  }                         //     aver     > (1 << (final/n))

  ratio = (final_size + n/2) / n;   // (n/2)/n==.5 for rounding
                                    // the integer division
  if (ratio >= sizeof(size_type)*8)
    return false;                   // Avoid << overflow
  else
    return average_size >           // O(n log N) > O(N) ?
           ((size_type)1<<ratio);
}

// build_known_size_tree(): build a new tree of a given
// number of nodes, populating it with existing nodes taken
// from a list.
// This takes linear time. That is, proportional to the
// number of nodes. The tree is directly built in perfect
// balance, while traversed in-order. Two stack rlists of
// fixed size (one element per bit in an integer) are used
// for making this in-order travel thorugh the tree under
// construction. No function recursion is used. Can it be
// more efficient?
//
// Complexity: O(N)

template<class T,class A,class W,class P>
//not inline
  typename rank_list<T,A,W,P>::node_t *     // First unused node
  rank_list<T,A,W,P>::build_known_size_tree
  (typename rank_list<T,A,W,P>::size_type n,   // Total # of nodes
   typename rank_list<T,A,W,P>::node_t * next) // List with nodes
{                                              // to link
  size_type depth;     // Current depth
  node_t * p, * last;  // Current and last nodes

  size_type                 // Per level: number of nodes
    counts[8*sizeof         // that still have to be created
             (size_type)];  // in the subtree

  node_t *                  // Per level...
    nodes[8*sizeof          // a) above depth: NULL or already
            (size_type)];   //    existing parent of a subtree
                            //    under creation
                            // b) bellow depth: NULL or already
                            //    existing nodes that have to
                            //    be linked to their parent
                            //    (still not created)
  init ();  // Fresh start

  if (n<=0)
    return next;

  for (depth=0; depth<8*sizeof(size_type); depth++) // Clear
    nodes[depth] = NULL;                            // stack

  counts[0] = n;                     // Nodes to create: n
  depth = 0;
  last = dummy ();  // The first node will be linked to dummy

  for (;;)
  {
    while (counts[depth]>1 &&  // If we visit a subtree for the
           !nodes[depth+1])    // first time, go down-left to
    {                          // the leftmost position, where
      depth ++;                              // we will place
      counts[depth] = counts[depth-1] >> 1;  // the first node
      counts[depth-1] -= counts[depth];      // Half the count
    }                                        // on every step

    RL_ASSERT (next);   // Enough nodes in the list?

    p = next;              // Grab the next node
    next = next->m_next;   // Advance in the list
    p->init ();            // Clear the node

    p->m_prev = last;          // Insert the node after the
    p->m_next = last->m_next;  // last one in the circular
    p->m_prev->m_next = p;     // doubly linked list
    p->m_next->m_prev = p;
                               // The last one is now the one
    last = p;                  // we've just inserted

    nodes[depth] = p;          // Put it in the stack
    counts[depth] --;          // One node less to go

    if (nodes[depth+1])              // If there was something
    {                                // in the next level, it
      nodes[depth+1]->m_parent = p;      // is the left subtree
      p->m_children[L] = nodes[depth+1];
      p->m_count += nodes[depth+1]->m_count;
      p->m_height += nodes[depth+1]->m_height; // Link it

      p->m_total_width = p->m_node_width +
                         nodes[depth+1]->m_total_width;

      nodes[depth+1] = NULL;                   // Forget it
    }

    if (counts[depth])         // More nodes to add here?
    {
      depth ++;                         // They will go on the
      counts[depth] = counts[depth-1];  // right subtree, so
      counts[depth-1] = 0;              // go down
    }
    else                       // Current subtree is done
      while (!counts[depth])   // Go up to a place where more
      {                        // nodes need to be added and
        p = nodes[depth];      // link the subtrees in the way

        if (!depth)            // Back in the top level?
        {                                      // Finished!
          p->m_parent = dummy ();              // Link the whole
          node_t::m_children[L] = p;           // tree to dummy and
          node_t::m_count = p->m_count + 1;    // update its count
          node_t::m_height = p->m_height + 1;  // and height

          node_t::m_total_width = p->m_total_width;

          return next;  // Return remaining nodes of the list
        }

        depth --;              // Step up

        if (nodes[depth])         // If there's a node there,
        {                         // what we have just built is
          nodes[depth+1] = NULL;         // its right subtree
          p->m_parent = nodes[depth];

          nodes[depth]->m_total_width += p->m_total_width;

          nodes[depth]->m_children[R] = p;      // Link it
          nodes[depth]->m_count += p->m_count;  // (the height
        }                                       // is already
      }                         // ok because the left subtree
  }                             // is allways >= the right one)
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

