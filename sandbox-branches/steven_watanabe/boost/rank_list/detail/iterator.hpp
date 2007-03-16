//  boost/rank_list/detail/iterator.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/iterator.hpp
  -------------------

  'Straight' rank_list iterators (both mutable and const
  implemented with a single template).

  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_ITERATOR_HPP_
#define _BOOST_RANK_LIST_ITERATOR_HPP_

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
         class W, class P,             // 2-in-1 trick: Ref and
         class Ref, class Ptr>         // Ptr are re-defined for
class rank_list_iterator               // const_iterator
{
  friend class boost::rank_lists::rank_list<T,A,W,P>;

  typedef rank_list_node_tree_fields<T,A,W,P>    node_t;
  typedef rank_list_iterator<T,A,W,P,Ref,Ptr>    my_class;
  typedef rank_list_rev_iter<T,A,W,P,Ref,Ptr>    my_reverse;
  typedef boost::rank_lists::rank_list<T,A,W,P>  my_rlist;

  public: // -------------- PUBLIC INTERFACE ----------------

#ifdef RL_USE_RANDOM_ACCESS_TAG
    typedef std::random_access_iterator_tag      iterator_category;
#else
    typedef std::bidirectional_iterator_tag      iterator_category;
#endif

    typedef typename my_rlist::value_type        value_type;
    typedef Ref                                  reference;
    typedef typename my_rlist::const_reference   const_reference;
    typedef Ptr                                  pointer;
    typedef typename my_rlist::size_type         size_type;
    typedef typename my_rlist::difference_type   difference_type;
    typedef typename my_rlist::const_iterator    const_iterator;

    // Constructors: all O(1)

    rank_list_iterator ();                    // Singular iterator
    rank_list_iterator (const my_class & it); // Copy constructor
    explicit
    rank_list_iterator (const my_reverse & it); // From reverse

    operator const_iterator ();     // Conversion to const

    // Dereference: O(1)

    reference operator* () const;
    pointer operator->() const;

    // Indexing: O(log N)

    reference operator[] (difference_type n) const;
    const_reference operator() (difference_type n) const;

    // Iterating through the list: O(1) just like std::list

    my_class & operator++ ();   // (pre++)
    my_class & operator-- ();   // (pre--)
    my_class operator++ (int);  // (post++)
    my_class operator-- (int);  // (post--)

    // Iterating through the tree: O(log N)

    my_class operator+ (difference_type n) const;
    my_class operator- (difference_type n) const;
    my_class & operator+= (difference_type n);
    my_class & operator-= (difference_type n);

    // Iterators difference: O(log N)

    template<class X,class Y> difference_type operator-
      (const rank_list_iterator<T,A,W,P,X,Y> & it) const;

    // Equality comparisons: O(1)

    template<class X,class Y> bool operator==
      (const rank_list_iterator<T,A,W,P,X,Y> & it) const;

    template<class X,class Y> bool operator!=
      (const rank_list_iterator<T,A,W,P,X,Y> & it) const;

    // Lesser/greater comparisons: O(log N)

    template<class X,class Y> bool operator<
      (const rank_list_iterator<T,A,W,P,X,Y> & it) const;

    template<class X,class Y> bool operator>
      (const rank_list_iterator<T,A,W,P,X,Y> & it) const;

    template<class X,class Y> bool operator<=
      (const rank_list_iterator<T,A,W,P,X,Y> & it) const;

    template<class X,class Y> bool operator>=
      (const rank_list_iterator<T,A,W,P,X,Y> & it) const;

  private: // ----- PRIVATE DATA MEMBER AND HELPER FUN. ------

    node_t * ptr;  // Poiter to referenced node

    rank_list_iterator (node_t * node)  // Only rank_list can
      : ptr(node) {}                    // access this constructor

    template<class IT>                          // Access the ptr
    static node_t * it_ptr (IT & it)            // member of other
    {                                           // iterators
      return my_rlist::iterator_pointer (it);   // through the
    }                                           // container class

};

//////////////////////////////////////////////////////////////////

// Default constructor: create a singular iterator

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr>::
  rank_list_iterator () : ptr(NULL) {}

// Copy constructor: just copy the embedded pointer

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr>::
  rank_list_iterator (const my_class & it) { ptr = it.ptr; }

// Conversion from reverse iterator: copy the pointer (yes, the
// same pointer; reverse iterators point to the refered element,
// not to its neighbor). The helper method it_ptr() calls a
// method of the rank_list class, which has access to the pointer

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr>::
  rank_list_iterator (const my_reverse & it) { ptr = it_ptr(it); }

// Conversion to const iterator. Again through the rank_list class

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr>::
  operator typename rank_list_iterator<T,A,W,P,Ref,Ptr>::
  const_iterator ()
{ return my_rlist::make_const_iterator(ptr); }

// Dereference. data() asserts that this is neither a singular
// iterator nor an end node.

template<class T,class A,class W,class P,class Ref,class Ptr>
inline typename rank_list_iterator<T,A,W,P,Ref,Ptr>::reference
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator* () const
{ return my_rlist::data (ptr); }

// The arrow can be used when T is a struct or class

template<class T,class A,class W,class P,class Ref,class Ptr>
inline typename rank_list_iterator<T,A,W,P,Ref,Ptr>::pointer
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator->() const
{ return &**this; }

// Index operator [] indirectly calls rank_lists::jump(), which
// takes O(log N) time. NOTE: rank_lists::jump() does check the
// range

template<class T,class A,class W,class P,class Ref,class Ptr>
inline typename rank_list_iterator<T,A,W,P,Ref,Ptr>::reference
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator[]
  (typename rank_list_iterator<T,A,W,P,Ref,Ptr>::difference_type n)
                                                              const
{ return *(*this+n); }

// Index operator () does exactly the same as operator []
// but returns const references only

template<class T,class A,class W,class P,class Ref,class Ptr>
inline
  typename rank_list_iterator<T,A,W,P,Ref,Ptr>::const_reference
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator()
  (typename rank_list_iterator<T,A,W,P,Ref,Ptr>::difference_type n)
                                                              const
{ return *(*this+n); }

// Operators ++ and -- iterate through the list. They require a
// single step through the next pointer or the previous pointer.
// They need to call helper methods of rank_list because the
// iterator class is not friend of the node class

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr> &
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator++ ()  // (pre++)
{
  ptr = my_rlist::next (ptr);    // Step forward
  return *this;
}

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr> &
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator-- ()  // (pre--)
{
  ptr = my_rlist::prev (ptr);    // Step back
  return *this;
}

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr>
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator++ (int) // (post++)
{
  my_class tmp(*this);
  ptr = my_rlist::next (ptr);    // Step forward
  return tmp;                    // Return unmodified copy
}

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr>
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator-- (int) // (post--)
{
  my_class tmp(*this);
  ptr = my_rlist::prev (ptr);    // Step back
  return tmp;                    // Return unmodified copy
}

// Operators +(it,n), +(n,it), -(it,n), +=(it,n) and -=(it,n)
// iterate through the tree. They call jump() on the rank_list,
// which takes between O(log n) and O(log N) time (n is the size
// of the jump, and N is the size of the rank_list)

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr>
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator+
  (difference_type n)                             const
{
  my_class tmp(*this);
  tmp.ptr = my_rlist::jump (tmp.ptr, n, false);
  RL_ASSERT_EXC (tmp.ptr, index_out_of_bounds());
  return tmp;
}                           // jump() takes logarithmic time

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr> operator+
  (typename rank_list<T,A,W,P>::difference_type n,
   const rank_list_iterator<T,A,W,P,Ref,Ptr> & it)
{ return it + n; }

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr>
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator-
  (difference_type n)                             const
{ return *this + -n; }

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr> &
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator+=
  (difference_type n)
{
  *this = *this + n;
  return *this;
}

template<class T,class A,class W,class P,class Ref,class Ptr>
inline rank_list_iterator<T,A,W,P,Ref,Ptr> &
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator-=
  (difference_type n)
{ return *this += -n; }

// Operator -(it,it') can mix const and var iterators. It takes
// O(log N) time. It checks the consistency of operands regarding
// the container they refer (should be the same for both)

template<class T,class A,class W,class P,class Ref,class Ptr>
template                                <class X,  class Y>
inline
  typename rank_list_iterator<T,A,W,P,Ref,Ptr>::difference_type
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator-
  (const rank_list_iterator<T,A,W,P,X,Y> & it)    const
{
  my_rlist * a, * b;
  size_type m, n;

  if (!ptr && !it_ptr(it)) return 0;  // Both singular

  m = my_rlist::position_of_node (ptr, a, false);
  n = my_rlist::position_of_node (it_ptr(it), b, false);

  RL_ASSERT (a==b); // Inter-rlist distance has no sense

  return difference_type(m) - difference_type(n);
}

// Equality and inequality operators take O(1) time. They can
// also mix const and var iterators

template<class T,class A,class W,class P,class Ref,class Ptr>
template                                <class X,  class Y>
inline bool
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator==
  (const rank_list_iterator<T,A,W,P,X,Y> & it)     const
{ return ptr==it_ptr(it); }

template<class T,class A,class W,class P,class Ref,class Ptr>
template                                <class X,  class Y>
inline bool
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator!=
  (const rank_list_iterator<T,A,W,P,X,Y> & it)     const
{ return ptr!=it_ptr(it); }

// Greater and lesser operators take O(log N) time in general.
// They will take just O(1) in the cases where the result can
// be decided with a simple equality/inequality comparison.
// The compared iterators must refer the same container

template<class T,class A,class W,class P,class Ref,class Ptr>
template                                <class X,  class Y>
inline bool
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator<
  (const rank_list_iterator<T,A,W,P,X,Y> & it)     const
{ return ptr==it_ptr(it) ? false : *this-it<0; }

template<class T,class A,class W,class P,class Ref,class Ptr>
template                                <class X,  class Y>
inline bool
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator>
  (const rank_list_iterator<T,A,W,P,X,Y> & it)     const
{ return ptr==it_ptr(it) ? false : *this-it>0; }

template<class T,class A,class W,class P,class Ref,class Ptr>
template                                <class X,  class Y>
inline bool
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator<=
  (const rank_list_iterator<T,A,W,P,X,Y> & it)     const
{ return ptr==it_ptr(it) ? true : *this-it<0; }

template<class T,class A,class W,class P,class Ref,class Ptr>
template                                <class X,  class Y>
inline bool
  rank_list_iterator<T,A,W,P,Ref,Ptr>::operator>=
  (const rank_list_iterator<T,A,W,P,X,Y> & it)     const
{ return ptr==it_ptr(it) ? true : *this-it>0; }

//////////////////////////////////////////////////////////////////

// Iterator tag function iterator_category()

template<class T, class A, class W, class P,
         class Ref, class Ptr>
inline
  typename rank_list_iterator<T,A,W,P,Ref,Ptr>::iterator_category
  iterator_category (const rank_list_iterator<T,A,W,P,Ref,Ptr>&)
{
  return typename rank_list_iterator<T,A,W,P,Ref,Ptr>::
                                          iterator_category();
}

// Iterator tag function value_type()

template<class T, class A, class W, class P,
         class Ref, class Ptr>
inline
  typename rank_list_iterator<T,A,W,P,Ref,Ptr>::value_type *
  value_type (const rank_list_iterator<T,A,W,P,Ref,Ptr>&)
{
  return reinterpret_cast<
          typename rank_list_iterator<T,A,W,P,Ref,Ptr>::
                                             value_type *>(0);
}

// Iterator tag function distance_type()

template<class T, class A, class W, class P,
         class Ref, class Ptr>
inline
  typename rank_list_iterator<T,A,W,P,Ref,Ptr>::difference_type *
  distance_type (const rank_list_iterator<T,A,W,P,Ref,Ptr>&)
{
  return reinterpret_cast<
          typename rank_list_iterator<T,A,W,P,Ref,Ptr>::
                                        difference_type *>(0);
}

//////////////////////////////////////////////////////////////////

    }  // namespace detail
  }  // namespace rank_lists
}  // namespace boost

#endif

