//  boost/rank_list/detail/rollback_list.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rollback_list.hpp
  ------------------------

  Temporary container (list form) for groups of nodes under
  creation. If an exception is thrown during the creation of a
  node, the rollback_list destructor will deallocate all
  previously created nodes. The method commit() takes the nodes
  out of the rollback_list, cancelling the rollback.
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_ROLLBACK_LIST_HPP_
#define _BOOST_RANK_LIST_ROLLBACK_LIST_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    namespace detail
    {

//////////////////////////////////////////////////////////////////

template<class T, class A,
         class W, class P>            // A list of nodes to
class rollback_list                   // complete or delete
{
  friend class boost::rank_lists::rank_list<T,A,W,P>;

  typedef rank_list_node_tree_fields<T,A,W,P>    node_t;
  typedef boost::rank_lists::rank_list<T,A,W,P>  my_rlist;

  private:
                          // rank_list that will deallocate
    my_rlist * m_owner;   // the nodes in case of rollback

    node_t * m_first;     // Ends of the doubly linked list
    node_t * m_last;      // with the nodes to commit (detach)
                          // or rollback (deallocate)

    rollback_list (my_rlist * owner); // The one and only constr.

    rollback_list<T,A,W,P> & operator=   // Assignment is a no-op
      (const rollback_list<T,A,W,P> &);  // (just prevents messing
                                         // lists)

    ~rollback_list ();  // Rollback on destruction (if nodes are
                        // still here) O(1) or O(n)

    void push_front (node_t * newnode);  // Add a new node to the
    void push_back (node_t * newnode);   // rollback list: O(1)

    void commit (node_t *& first,        // Detach the nodes from
                 node_t *& last);        // the rollback list,
};                                       // (cancel dealloc.) O(1)

//////////////////////////////////////////////////////////////////

// Constructor: initialize the list with null pointers and store
// the address of the rank_list. This address will be required for
// deallocating the nodes in the destructor (in case of rollback)

template<class T,class A,class W,class P>
inline rollback_list<T,A,W,P>::rollback_list (my_rlist * owner)
   : m_owner(owner),
     m_first(NULL),
     m_last(NULL)
{}

// The assignment operator is just a no-op. It should never be
// called. The no-op implementation avoids the posibility of a
// default assignment that would leave memory leaks and/or
// provoke double destructions

template<class T,class A,class W,class P>
inline rollback_list<T,A,W,P> &
  rollback_list<T,A,W,P>::operator=
  (const rollback_list<T,A,W,P> &)
{}

// Destructor: iff there are nodes in the list (as a result of the
// destructor being called before commit() due to an exception)
// deallocate them. If the list is empty (the usual case), this
// will take O(1). Otherwise, it will take O(n) time

template<class T,class A,class W,class P>
inline rollback_list<T,A,W,P>::~rollback_list ()
{
  node_t * p;

  while (m_first)
  {
    p = m_first;
    m_first = m_first->m_next;
    m_owner->delete_node (p);
  }
}

// push_front(): insert a node at the beginning of the list. Time
// required: O(1)

template<class T,class A,class W,class P>
inline void
  rollback_list<T,A,W,P>::
  push_front (node_t * newnode)
{
  newnode->m_next = m_first;
  newnode->m_prev = NULL;

  if (m_first)
    m_first->m_prev = newnode;
  else
    m_last = newnode;

  m_first = newnode;
}

// push_back(): append a node at the end of the list. Time
// required: O(1)

template<class T,class A,class W,class P>
inline void
  rollback_list<T,A,W,P>::
  push_back (node_t * newnode)
{
  newnode->m_prev = m_last;
  newnode->m_next = NULL;

  if (m_last)
    m_last->m_next = newnode;
  else
    m_first = newnode;

  m_last = newnode;
}

// commit(): detach the list and return it in (the pointers first
// and last are received by reference). Reset the list (cancel
// rollback)

template<class T,class A,class W,class P>
inline void
  rollback_list<T,A,W,P>::
  commit (node_t *& first,
          node_t *& last)
{
  first = m_first;
  last = m_last;

  m_first = m_last = NULL;
}

//////////////////////////////////////////////////////////////////

    }  // namespace detail
  }  // namespace rank_lists
}  // namespace boost

#endif

