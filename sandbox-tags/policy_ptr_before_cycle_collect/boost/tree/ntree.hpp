//  Boost-sandbox general library ntree.hpp header file  ----------------------

//  (C) Copyright Marc Cromme 2002, marc@cromme.dk. Permission to
//  copy, use, modify, sell and distribute this software is granted
//  provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_NTREE_HPP
#define BOOST_NTREE_HPP

#include <memory>
#include <iterator>
#include <map>
 
// compiled on a debian linux box "testing" using 
// g++-3.1 -g -p -ansi -pedantic -W -Wall -Wshadow -Weffc++ -Wfloat-equal
// you might get tired of the -Weffc++ -Wfloat-equal options since they
// flag warnings in the g++-3.1 implementation of the STL

namespace boost 
{

   /**
    *  @brief A standard container which implements N-ary trees 
    *
    *  This is a reversible STL-like container template class which implements
    *  a rooted, ordered n-ary tree  stucture. The interface however is 
    *  generic to any rooted tree, unordered or/and with unresticted child 
    *  count.
    *
    *  The basic idea is that standard iterators are used to acess the
    *  data of the nodes, and that all nodes can be parsed linearly
    *  from ntree.begin() to ntree.end() to ensure maximal conformance
    *  to the STL containers. In fact, this class adheres to the
    *  concept "reversible container", thus also allowing reverse
    *  linear parsing. Linear parsing will be done in levels from root
    *  (= ntree.begin()) to last leave (= ntree.end() -1). Most STL
    *  container algorithms can be applied to ntree's.  Note though
    *  that the concept of reversible container does not apply to
    *  un-ordered trees - on the other hand: how to represent unordered trees
    *  in the first place ?
    *
    *  In addition to the reversible container concept interface, the ntree 
    *  template adheres to the (yet not well-defined) tree concept interface,
    *  which essentially has member functions for inserting and erasing nodes
    *  into the tree stucture, and member functions returning iterators to
    *  parent, child and sibling nodes.  
    *
    *  Additionally, the plan is to implement traits and algorithms in
    *  the extend needed, obvious candidates are pre-order, in-order
    *  and post-order tree parsing, path parsing from a given node to
    *  the root node, parsing of leaf nodes in level-order, pre-order,
    *  in-order and post-order. These apply certainly only for ordered
    *  trees. Also algorithms testing the staus of a node as child, parent
    *  root or leaf will be added.
    *
    *  The implementation reuses an STL map for simplicity, better
    *  and/or faster implementations using double linked nodes are
    *  also possible (like the usual red-black trees found in
    *  implementations of the STL container classes). The
    *  implementation relies on the fact that the nodes of ordered
    *  trees with at most N childen can be numbered by 0 for root, [1
    *  N+1) for the children, [N+1 N*(N+1)+1) for the grandchildren,
    *  and so on. Thus, kparent = (kchild - 1)/N and 
    *  kchild(n) = (kparent*N) + 1 + n    
    *
    *  PLAIN WRONG DESIGN USING std::map ITERATORS AS ITERATORS FOR NTREE
    */
   template < size_t N, class Type, class Alloc = std::allocator<Type > >
   class ntree 
   {
      public:
	 // typedefs ----------------------------------------------------------
	 typedef typename std::map<size_t, Type, std::less<Type>, 
				   Alloc>::value_type value_type;
	 typedef typename std::map<size_t, Type, std::less<Type>, 
				   Alloc>::pointer pointer;
	 typedef typename std::map<size_t, Type, std::less<Type>, 
				   Alloc>::const_pointer const_pointer;
	 typedef typename std::map<size_t, Type, std::less<Type>,
				   Alloc>::reference reference;
	 typedef typename std::map<size_t, Type, std::less<Type>,
				   Alloc>::const_reference const_reference;
	 typedef typename std::map<size_t, Type, std::less<Type>, 
				   Alloc>::size_type size_type;
	 typedef typename std::map<size_t, Type, std::less<Type>,
				   Alloc>::difference_type difference_type;
	 typedef typename std::map<size_t, Type, std::less<Type>, 
				   Alloc>::iterator iterator;
	 typedef typename std::map<size_t, Type, std::less<Type>,
				   Alloc>::const_iterator const_iterator;
	 typedef typename std::map<size_t, Type, std::less<Type>,
				   Alloc>::reverse_iterator reverse_iterator;
	 typedef typename std::map<size_t, Type, std::less<Type>,
				   Alloc>::const_reverse_iterator
	 const_reverse_iterator;
	 

	 // allocation/deallocation -------------------------------------------
	 /**
	  *  Constructs an empty ntree with the specified Allocator class
	  */
	 explicit ntree(const Alloc& a = Alloc()) 
	    : rep(rep.key_comp(), a) 
	 {
	 }
      
	 /**
	  *  Standard copy constructor
	  */
	 ntree(const ntree<N,Type,Alloc>& x) 
	    : rep(x.rep) 
	 {
	 }

	 /**
	  *  Constructs a dense filled ntree with n nodes. The node content
	  *  is copied from the given input sequence range such that the 
	  *  content of first is copied into the root node, first++ is then
          *  succevely copied into the zero´th, first, second ... child of
	  *  the root and so on, by level.
	  */
	 template <class InputIterator>
	 ntree(InputIterator first, InputIterator last,
	       const Alloc& a = Alloc())
	    : rep(rep.key_comp(), a)
	 {
	    difference_type n = std::distance(first, last);
	    for (size_type id = 0; n > 0; id++ , n--, first++)
	    {
 	        std::pair<const size_type, Type> valuepair =
 		  std::make_pair<const size_type, Type>(id, *first);
 	       rep.insert(valuepair);
	    }
	 }

// 	 /**
// 	  *  Destructor  
// 	  */
// 	 ~ntree()
// 	 {
// 	 }

	 // assignment --------------------------------------------------------
	 /**
	  * Standard assignment operator 
	  */
	 ntree<N,Type,Alloc>&
	 operator=(const ntree<N, Type, Alloc>& x)
	 {
	    if (*this == x)
	       return *this;
	    
	    rep = x.rep;
	    return *this;
	    
	 }

	 // reversible container iterators ------------------------------------
	 /**
	  *  Returns a read/write iterator that points to the root node in the 
	  *  ntree. Iteration is done from root to leafs.
	  */
	 iterator begin() 
	 { 
	    return rep.begin(); 
	 }

	 /**
	  *  Returns a read-only (constant) iterator that points to
	  *  the root node in the ntree.  Iteration is done from root
	  *  to leafs.
	  */
	 const_iterator begin() const 
	 { 
	    return rep.begin(); 
	 }

	 /**
	  *  Returns a read/write iterator that points one past the
	  *  last leaf in the ntree.  Iteration is done from root to
	  *  leafs.
	  */
	 iterator end() 
	 { 
	    return rep.end(); 
	 }

	 /**
	  *  Returns a read-only (constant) iterator that points one
	  *  past the last leaf in the ntree. Iteration is done from
	  *  root to leafs.
	  */
	 const_iterator end() const 
	 { 
	    return rep.end(); 
	 }

	 /**
	  *  Returns a read/write reverse iterator that points to the
	  *  last leaf in the ntree. Iteration is done from last leaf
	  *  to root.
	  */
	 reverse_iterator rbegin() 
	 { 
	    return rep.rbegin(); 
	 }

	 /**
	  *  Returns a read-only (constant) reverse iterator that
	  *  points to the last leaf in the ntree.  Iteration is done
	  *  from last leaf to root.
	  */
	 const_reverse_iterator rbegin() const 
	 { 
	    return rep.rbegin(); 
	 }

	 /**
	  *  Returns a read/write reverse iterator that points to one
	  *  before the root in the ntree.  Iteration is done from
	  *  last leaf to root.
	  */
	 reverse_iterator rend() 
	 { 
	    return rep.rend(); 
	 }

	 /**
	  *  Returns a read-only (constant) reverse iterator that
	  *  points to one before the root in the ntree.  Iteration is
	  *  done from last leaf to root.
	  */
	 const_reverse_iterator rend() const 
	 { 
	    return rep.rend(); 
	 }

	 // container member functions ----------------------------------------
	 /** Returns true if the ntree is empty. */
	 bool empty() const 
	 { 
	    return rep.empty(); 
	 }

	 /** Returns the size of the ntree.  */
	 size_type size() const 
	 { 
	    return rep.size(); 
	 }

	 /** Returns the maximum size of the ntree.  */
	 size_type max_size() const 
	 { 
	    return rep.max_size(); 
	 }

	 /** 
	  *  Erases all nodes in a ntree.  Note that this function only erases
	  *  the nodes, and that if the nodes themselves are pointers, the
	  *  pointed-to memory is not touched in any way.
	  */
	 void clear() 
	 { 
	    rep.clear(); 
	 }

	 /** 
	  *  Standard swap operation
	  */
	 void swap(ntree<N,Type,Alloc>& x) 
	 {
	    this->rep.swap(x.rep);
	    
	 }
      

	 // container operators -----------------------------------------------
	 /** 
	  *  Standard equality operator
	  */
	 template <size_t N1, class T1, class A1>
	 friend bool operator== (const ntree<N1, T1, A1>&,
				 const ntree<N1, T1, A1>&);

	 /** 
	  *  Standard less operator
	  */
	 template <size_t N1, class T1, class A1>
	 friend bool operator< (const ntree<N1, T1, A1>&,
				const ntree<N1, T1, A1>&);



	 // ntree specific iterators ------------------------------------------
	 /**
	  *  Returns a read/write iterator that points to the root node in the 
	  *  ntree. Synonym for begin(). Iteration is done from root to leafs.
	  */
	 iterator root() 
	 { 
	    return rep.begin(); 
	 }

	 /**
	  *  Returns a read-only (constant) iterator that points to
	  *  the root node in the ntree. Synonym for
	  *  begin(). Iteration is done from root to leafs.
	  */
	 const_iterator root() const 
	 { 
	    return rep.begin(); 
	 }

	 /**
	  *  Returns a read/write iterator that points to the first
	  *  node of level n. Synonym for root() if n = 0. Iteration
	  *  is done from root to leafs. Returns end() if n too large.
	  */
	 iterator level(size_type n) 
	 { 
	    return rep.end(); 
	 }

	 /**
	  *  Returns a read-only (constant) iterator that points to
	  *  first node of level n. Synonym for root() if n =
	  *  0. Iteration is done from root to leafs. Returns end() if
	  *  n too large.
	  */
	 const_iterator level(size_type n) const 
	 { 
	    return rep.end(); 
	 }

	 /**
	  *  Returns a read/write iterator that points to the parent
	  *  node in the ntree. Iteration is done from root to leafs.
	  */
	 iterator parent(iterator ichild) 
	 { 
	    return rep.end(); 
	 }

	 /**
	  *  Returns a read-only (constant) iterator that points to the parent 
	  *  node in the ntree. Iteration is done from root to leafs.
	  *  Returns end() if ichild is root.
	  */
	 const_iterator parent(const_iterator ichild) const 
	 { 
	    return rep.end(); 
	 }

	 /**
	  *  Returns a read/write iterator that points to the child
	  *  node n in the ntree. Iteration is done from root to
	  *  leafs. Returns end() if ichild is root.
	  */
	 iterator child(iterator iparent, size_type n) 
	 { 
	    return rep.end(); 
	 }

	 /**
	  *  Returns a read-only (constant) iterator that points to the child 
	  *  node n in the ntree. Iteration is done from root to leafs.
	  *  Returns end() if iparent is leaf.
	  */
	 const_iterator child(const_iterator iparent, size_type n) const 
	 { 
	    return rep.end(); 
	 }

	 /**
	  *  Returns a range of read/write iterators that points to
	  *  the children in the ntree. Iteration is done from root to
	  *  leafs. Returns (end(),end()) if iparent is leaf.
	  */
	 std::pair<iterator, iterator> children(iterator iparent) 
	 { 
	    return std::make_pair(rep.end(), rep.end()); 
	 }

	 /**
	  *  Returns a range of read-only (constant) iterators that
	  *  points to the children in the ntree. Iteration is done
	  *  from root to leafs. Returns (end(),end()) if iparent is leaf.
	  */
	 std::pair<const_iterator, const_iterator> 
	 children(const_iterator iparent) const 
	 { 
	    return std::make_pair(rep.end(), rep.end()); 
	 }

	 /**
	  *  Returns a range of read/write iterators that points to
	  *  the siblings in the ntree. Iteration is done from root to
	  *  leafs.
	  */
	 std::pair<iterator, iterator> siblings(iterator isib) 
	 { 
	    return std::make_pair(rep.end(), rep.end()); 
	 }

	 /**
	  *  Returns a range of read-only (constant) iterators that
	  *  points to the siblings in the ntree. Iteration is done
	  *  from root to leafs.
	  */
	 std::pair<const_iterator, const_iterator> 
	 siblings(const_iterator isib) const 
	 { 
	    return std::make_pair(rep.end(), rep.end()); 
	 }


	 // ntree specific insert/erase  --------------------------------------
	 /**
	  *  Attempts to insert the root node into the ntree.  Returns
	  *  an iterator that points to the possibly inserted root, or
	  *  to end() on failure.  ntree relies on unique nodes and
	  *  thus the root is only inserted if the ntree is empty.
	  */
	 iterator insert_root(const Type& x = Type())
	 {
	    if (!rep.empty())
	       return rep.end();

	    std::pair<const size_type, Type> valuepair =
	       std::make_pair<const size_type, Type>(0,x);
	    std::pair<iterator, bool> result = rep.insert(valuepair);
	    return result.first;
	 }
      
	 /**
	  *  Attempts to insert the n-th leaf child node of the parent
	  *  node into the ntree.  Returns an iterator that points to
	  *  the possibly inserted node, or to end() on failure.
	  *  ntree relies on unique nodes and thus a node is only
	  *  inserted if it is not already present in the ntree.
	  */
	 iterator insert_child(iterator iparent, size_type n,
					       const Type& x = Type())
	 {
	    if(!check(n))
	       return rep.end();

	    key_type kchild = child_key(iparent->first, n);
 	    std::pair<const size_type, Type> valuepair =
 	       std::make_pair<const size_type, Type>(kchild,x);
 	    std::pair<iterator, bool> result = rep.insert(valuepair);
	    if (result.second)
	       return result.first;
	    else
	       return rep.end();
	 }
      
	 /**
	  *  Attempts to insert all leaf children nodes of the parent
	  *  node into the ntree.  Returns a range of iterators that
	  *  point to the possibly inserted nodes, or a pait of end()
	  *  on failure.  ntree relies on unique nodes and thus all
	  *  children nodes are only inserted if parent is a
	  *  childrenless leaf node.
	  */
	 std::pair<iterator,iterator> insert_children(iterator iparent, 
						  const Type& x = Type())
	 {
	    return std::make_pair(rep.end(), rep.end()); 
	 }
      
	 /**
	  *  A template function that attemps to insert at most N
	  *  nodes from another range (possibly another ntree).
	  *  Iterator first is pointing to the start of the range to
	  *  be inserted.  Iterator last is pointing to the end of the
	  *  range. Returns a range of iterators that
	  *  point to the possibly inserted nodes, or a pait of end()
	  *  on failure.  ntree relies on unique nodes and thus all
	  *  children nodes are only inserted if parent is a
	  *  childrenless leaf node.
	  */
	 template <class InputIterator>
	 std::pair<iterator,iterator> insert_children(iterator iparent,
						  InputIterator first, 
						  InputIterator last) 
	 {
	    return std::make_pair(rep.end(), rep.end()); 
	 }

	 /**
	  *  This function erases the root node if and only if it is a
	  *  leaf. Note that if the root node contains a pointer, the
	  *  pointed-to memory is not touched in any way. Returns true
	  *  if root node has been erased. Synonym for
	  *  erase_leaf(tree.root())
	  */
	 bool erase_root() 
	 { 
	    return false;
	 }

	 /**
	  *  This function erases a leaf node, pointed to by the given
	  *  iterator, from an ntree.  Note that this function erases
	  *  the node if and only if it is a leaf, and that if the
	  *  leaf node contains a pointer, the pointed-to memory is
	  *  not touched in any way. Returns true if leaf node has
	  *  been erased.
	  */
	 bool erase_leaf(iterator iposition) 
	 { 
	    return false;
	 }

	 /**
	  *  Erases a [first,last) range of nodes from a ntree.  Note
	  *  that this function only erases the nodes if and only if
	  *  they are leafs. If the nodes contain pointers, the
	  *  pointed-to memory is not touched in any way.
	  */
	 bool erase_leaf(iterator first, iterator last)
	 { 
	    return false;
	 }

	 /**
	  *  This function erases a node, pointed to by the given iterator, 
	  *  from an ntree.  Note that this function erases the node and it's
	  *  possible subtree, and that if the node or subtree contains 
	  *  pointers, the pointed-to memory is not touched in any way.
	  */
	 bool erase_subtree(iterator iposition) 
	 { 
	    return false;
	 }

	 /**
	  *  Erases a [first,last) range of nodes from a ntree.
	  *  Note that this function only erases the nodes, and their possible 
	  *  subtrees. If the nodes contain pointers, the pointed-to memory is 
	  *  not touched in any way.
	  */
	 bool erase_subtree(iterator first, iterator last)
	 { 
	    return false;
	 }


	 // other ntree specific member functons ------------------------------
	 /** Returns the number of levels of the ntree.  */
	 size_type no_levels() const 
	 { 
	    return 0;
	 }

	 /** Returns the maximum number of levels of the ntree.  */
	 size_type max_no_levels() const 
	 {
	    return 0;
	 }


// 	 /**
//           *  Returns true if node is root node, false otherwise
// 	  */
// 	 bool is_root(iterator iposition) 
// 	 { 
// 	 }


// 	 /**
//           *  Returns true if node is parent node, false otherwise
// 	  */
// 	 bool is_parent(iterator iposition) 
// 	 { 
// 	 }


// 	 /**
//           *  Returns true if node is child node, false otherwise
// 	  */
// 	 bool is_child(iterator iposition) 
// 	 { 
// 	 }


// 	 /**
//           *  Returns true if node is leaf node, false otherwise
// 	  */
// 	 bool is_leaf(iterator iposition) 
// 	 { 
// 	 }



      private:
 	 // Private ntree types:-----------------------------------------------
	 typedef typename std::map<size_t, Type, std::less<Type>, 
				   Alloc>::key_type key_type;


 	 // Private ntree operations:------------------------------------------

	 bool check(size_type n)
	 {
	    if (n < N)
	       return true;
	    else
	       return false;
	 }

	 key_type parent_key(key_type kchild)
	 {
	    return (kchild - 1)/N;
	 }
	 
	 key_type child_key(key_type kparent, size_type n = 0)
	 {
	    return (kparent*N) + 1 + n;
	 }
	 

// 	 /**
// 	  *  This function takes a key and tries to locate the node
// 	  *  with which the key matches.  If successful the function
// 	  *  returns an iterator pointing to the sought after pair. If
// 	  *  unsuccessful it returns the one past the end ( end() )
// 	  *  iterator.
// 	  */
// 	 iterator find(const size_type& k) 
// 	 { 
// 	    return std::map<size_t,Type,std::less<Type>,Alloc>::find(k); 
// 	 }

// 	 /**
// 	  *  This function takes a key and tries to locate the node with which
// 	  *  the key matches.  If successful the function returns a constant 
// 	  *  iterator pointing to the sought after pair. If unsuccessful it 
// 	  *  returns the one past the end ( end() ) iterator.
// 	  */
// 	 const_iterator find(const size_type& k) const 
// 	 { 
// 	    return std::map<size_t,Type,std::less<Type>,Alloc>::find(k); 
// 	 }

      private:
	 // Private ntree representation:--------------------------------------
	 std::map<size_t, Type, std::less<Type>, Alloc>  rep;
	 

   }; // class ntree ----------------------------------------------------------


// comparison operators -------------------------------------------------------

   template <size_t N, class Type, class Alloc>
   inline bool operator==(const ntree<N,Type,Alloc>& x,
			  const ntree<N,Type,Alloc>& y) {
      return (x.rep == y.rep);
   }

   template <size_t N, class Type, class Alloc>
   inline bool operator<(const ntree<N,Type,Alloc>& x,
			 const ntree<N,Type,Alloc>& y) 
   {
      return (x.rep < y.rep);
   }

   template <size_t N, class Type, class Alloc>
   inline bool operator!=(const ntree<N,Type,Alloc>& x,
			  const ntree<N,Type,Alloc>& y) 
   {
      return !(x == y);
   }

   template <size_t N, class Type, class Alloc>
   inline bool operator>(const ntree<N,Type,Alloc>& x,
			 const ntree<N,Type,Alloc>& y) 
   {
      return y < x;
   }

   template <size_t N, class Type, class Alloc>
   inline bool operator<=(const ntree<N,Type,Alloc>& x,
			  const ntree<N,Type,Alloc>& y) 
   {
      return !(y < x);
   }

   template <size_t N, class Type, class Alloc>
   inline bool operator>=(const ntree<N,Type,Alloc>& x,
			  const ntree<N,Type,Alloc>& y) 
   {
      return !(x < y);
   }

   template <size_t N, class Type, class Alloc>
   inline void swap(ntree<N,Type,Alloc>& x,
		    ntree<N,Type,Alloc>& y) 
   {
      x.swap(y);
   }


} // namespace boost

#endif  // BOOST_NTREE_HPP










