// Copyright (c) 2006 Bernhard Reiter.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file searcher.hpp
 * Generalized tree search algorithms and front-end class
 */
// TODO: 
// * Concept checks all over.
// * add versions without cmp argument (just using <)
// * Move functionality to TR1 (wrappers)
// * use boost:: (or tr1::)function

// Rip out node_search finally. Clean up & refactor helpers.

#ifndef BOOST_TREE_SEARCHER_HPP
#define BOOST_TREE_SEARCHER_HPP


#include <boost/tree/inorder.hpp>
#include <boost/tree/cursor.hpp>
#include <boost/tree/binary_tree.hpp>

#include <boost/tree/detail/range_helpers.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>

#include <boost/multi_index/identity.hpp> //remove

#include <algorithm>	
#include <functional>


using boost::function;
using boost::multi_index::identity; //remove

namespace boost {
namespace tree {


/******************************************************************************/
/* Wrappers for other key_{lower|upper}_bounds */
//TODO: revisit, unclutter.

template<class Cursor, typename T, typename Compare, 
		 typename cursor_size<Cursor>::type s>
Cursor
do_lower_bound(Cursor first, Cursor last, T const& val, Compare cmp, 
			   integral_constant<typename cursor_size<Cursor>::type,s>
			   const&)
{
	return std::lower_bound(first, last, val, cmp);
}

template<class Cursor, typename T, typename Compare, 
		 typename cursor_size<Cursor>::type s>
Cursor
do_upper_bound(Cursor first, Cursor last, T const& val, Compare cmp, 
			   integral_constant<typename cursor_size<Cursor>::type,s>
			   const&)
{
	return std::upper_bound(first, last, val, cmp);
}

template<class Cursor, typename T, typename Compare>
Cursor
do_lower_bound(Cursor first, Cursor last, T const& val, Compare cmp, 
			   integral_constant<typename cursor_size<Cursor>::type,2>
			   const&)
{
	return binary_lower_bound(first, last, val, cmp);
}

template<class Cursor, typename T, typename Compare>
Cursor
do_upper_bound(Cursor first, Cursor last, T const& val, Compare cmp, 
			   integral_constant<typename cursor_size<Cursor>::type,2>
			   const&)
{
	return binary_upper_bound(first, last, val, cmp);
}

template<class ForwardIterator, typename T, typename Compare>
class lower_bound_wrapper
{
	ForwardIterator operator() (ForwardIterator first, ForwardIterator last, T const& val, Compare cmp)
	{
		return binary_lower_bound(first, last, val, cmp); //XIXME
	}
};

template<class ForwardIterator, typename T, typename Compare>
class upper_bound_wrapper
{
	ForwardIterator operator() (ForwardIterator first, ForwardIterator last, T const& val, Compare cmp)
	{
		return std::upper_bound(first, last, val, cmp);
	}
};

template<class Cursor, typename T, typename Compare>
class lower_bound_default
{
	Cursor operator() (Cursor first, Cursor last, T const& val, Compare cmp)
	{
		return do_lower_bound(first, last, val, cmp, typename cursor_arity<Cursor>::type());
	}
};

template<class Cursor, typename T, typename Compare>
class upper_bound_default
{
	Cursor operator() (Cursor first, Cursor last, T const& val, Compare cmp)
	{
		return do_upper_bound(first, last, val, cmp, typename cursor_arity<Cursor>::type());
	}
};

// The following functions make assumptions about how cmp is invoked by 
// node_search. Although this seems pretty straight-forward, is this also 
// justifiable by the standard?

// STD: make the following functions specializations of {lower|upper}_bound for 
// binary cursors? ranges/sortables even?

/** 
 * @brief \c lower_bound key tree searcher
 * @param c				A \c Cursor
 * @param key			Search key
 * @param node_search	The search algorithm to use within a node
 * @param extract		A key extractor 
 * (see http://www.boost.org/libs/multi_index/doc/reference/key_extraction.html)
 * @param compare		Comparison functor
 * 
 * @ingroup searchers
*/
template</* class NodeSearch, */class Extract, class Compare, class Node>
std::pair<tree_cursor<Node>, std::pair<bool, bool> > //TODO: ugly return type. use tuple instead?
key_lower_bound(tree_cursor<Node> c, typename Extract::result_type key, 
//				NodeSearch node_search, 
				Extract extract = Extract(), Compare compare = Compare())
{
	       bool end_flag = false;
	       typedef tree_cursor<Node> cursor;
	       while (has_child(c))
	        {
	                Compare cmp = compare;
	                cursor p = c;
					c = binary_lower_bound(p.begin(), p.end(), key, //FIXME: use node_search instead.
	                		bind<bool>(cmp, bind<typename Extract::result_type>(extract, _1), _2)); 
	                if ((end_flag = (c != p.end())) && (!cmp(key, extract(*c)))) // success!
	                        return std::make_pair(c, std::make_pair(false, end_flag));
	        }
	        return std::make_pair(c, std::make_pair(true, end_flag)); // not found.
}


//FIXME: specialization for binary tree using binary_lower

template</*class NodeSearch,*/ class Extract, class Compare, class Range>
std::pair<typename Range::iterator, std::pair<bool, bool> > //TODO: ugly return type. use tuple instead?
key_lower_bound(Range& r, typename Extract::result_type key, 
//				NodeSearch node_search, 
				Extract extract = Extract(), Compare cmp = Compare())
{
	typename Range::iterator c = std::lower_bound(r.begin(), r.end(), key, //FIXME: use node_search instead.
    		bind<bool>(cmp, bind<typename Extract::result_type>(extract, _1), _2)); 
	// end() cannot be dereferenced; if not "not less" => equal                		
    if ((c != r.end()) && (!cmp(key, extract(*c)))) // success!
            return std::make_pair(c, std::make_pair(false, true));
    return std::make_pair(c, std::make_pair(true, false)); // not found.
}

//TODO: don't use binary_tree explicitly.

template</* class NodeSearch, */class Extract, class Compare, class Node, class Balance, class ValAlloc, class NodeAlloc>
std::pair<typename sortable_traits<binary_tree<Node, Balance, ValAlloc, NodeAlloc> >::cursor, std::pair<bool, bool> > //TODO: ugly return type. use tuple instead?
key_lower_bound(binary_tree<Node, Balance, ValAlloc, NodeAlloc>& t, typename Extract::result_type key, 
//				NodeSearch node_search, 
				Extract extract = Extract(), Compare compare = Compare())
{        
	        return key_lower_bound(head(t), key, /*node_search,*/ extract, compare); // not found.
}

/******************************************************************************/


/** 
 * @brief \c lower_bound key tree searcher
 * @param c				A \c Cursor to a binary tree node
 * @param key			Search key
 * @param extract		A key extractor (see
 * @see http://www.boost.org/libs/multi_index/doc/reference/key_extraction.html)
 * @param compare		Comparison functor
 * 
 * @pre The subtree @a c points to is alreadysorted (by @a Cmp)
 * 
 * @ingroup searchers
 * 
 * Uses different methods of node search depending on @a c's \c cursor_traits
 * (and its node class' @a node_traits) in order to guarantee efficient lookup
 * for cursors to binary nodes, cursors modelling ForwardIterator and cursors
 * (only) modelling InputIterator.
 * 
*/
//template<class Cursor, class Extract, class Compare>
//std::pair<Cursor, std::pair<bool, bool> > //TODO: ugly return type. use tuple instead?
//key_lower_bound(Cursor c, typename Extract::result_type key, 
//				Extract extract = Extract(), Compare compare = Compare())
//{
//	typedef typename cursor_arity<Cursor>::type arity;
//	return lower_bound_key(c, key, 
//		   bind(do_lower_bound, _1, _2, _3, _4, arity()), 
//		   extract, compare);
//}

//one general one using cursor.


/** 
 * @brief \c upper_bound key tree searcher
 * @param c				A \c Cursor to a binary tree node
 * @param key			Search key
 * @param extract		A key extractor (see
 * @see http://www.boost.org/libs/multi_index/doc/reference/key_extraction.html)
 * @param compare		Comparison functor
 * 
 * @pre The subtree @a c points to is alreadysorted (by @a Cmp)
 * 
 * @ingroup searchers
 * 
 * Uses different methods of node search depending on @a c's \c cursor_traits
 * (and its node class' @a node_traits) in order to guarantee efficient lookup
 * for cursors to binary nodes, cursors modelling ForwardIterator and cursors
 * (only) modelling InputIterator.
 * 
*/
template<class Cursor, class Extract, class Compare>
std::pair<Cursor, std::pair<bool, bool> > //TODO: ugly return type. use tuple instead?
key_upper_bound(Cursor c, typename Extract::result_type key, 
				Extract extract = Extract(), Compare compare = Compare())
{
	typedef typename cursor_arity<Cursor>::type arity;
	return upper_bound_key(c, key, 
		   bind(do_upper_bound, _1, _2, _3, _4, arity()), 
		   extract, compare);
}


/** @brief A %searcher
 * 
 * This class models the 
 * \link http://www.sgi.com/tech/stl/SortedAssociativeContainer.html
 * Sorted Associative Container \endlink concept; it is 
 * actually nothing else than a container adaptor bundling a Container 
 * together with key extraction and comparison functors, guaranteeing
 * that insertion of a value will not violate the ordering (which is established
 * by comparing the values' keys).
 * For an "ordinary", one-dimensional Sequence, the STL's binary search 
 * algorithms (lower_bound, upper_bound etc.) are wrapped as members and used to
 * determine the correct insertion position. If the Container however models a 
 * Hierarchical Container, corresponding sorted tree search algorithms are
 * used instead.
 * Note that this declaration is empty;
 * there are two specializations, one for each of the two possible values of the
 * %Multi template argument:
 * 	- true for a 
 * \link http://www.sgi.com/tech/stl/MultipleSortedAssociativeContainer.html
 * Multiple Sorted Associative Container \endlink , i.e. one that allows
 * multiple values to share the same key
 * 	- false for a 
 * \link http://www.sgi.com/tech/stl/UniqueSortedAssociativeContainer.html
 * Unique Sorted Associative Container \endlink , allowing only one value for 
 * each key.
 * 
 * These specializations are necessary as their interfaces differ.
 */
template <bool Multi, class Sortable, /* class Cursor = typename Sortable::cursor, */
		  class Extract = 
		  	boost::multi_index::identity<typename Sortable::value_type>,
		  class Compare = std::less<typename Extract::result_type> //,
		  /*class NodeLowerBound = 
		  	lower_bound_default<typename sortable_traits<Sortable>::cursor, typename Extract::result_type, 
		  						Compare>*/
		   >
class searcher {}; 

//derive both specializations from common base modelling only 
//SortedAssociativeContainer

/** @brief Unique %searcher specialization
 * 
 * This class models the 
 * \link http://www.sgi.com/tech/stl/UniqueSortedAssociativeContainer.html
 * Unique Sorted Associative Container \endlink concept. Refer to the 
 * unspecialized template's (boost::tree::searcher)
 * documentation for further information.
 */
template <class Sortable, class Extract, class Compare>
class searcher<false, Sortable, Extract, Compare> {

	typedef searcher<false, Sortable, Extract, Compare> self_type;

public:
 	typedef Sortable container_type; // type name as in STL sequence adaptors
 

 	
	typedef typename sortable_traits<container_type>::cursor cursor; //private?
	
	// TODO: use traits instead.
	typedef typename container_type::iterator iterator; //inorder.
	typedef typename container_type::const_iterator const_iterator;
		
 	typedef typename container_type::value_type value_type;
 	typedef typename container_type::size_type size_type;
 
 	//typedef NodeLowerBound node_lower_bound;
 	typedef Extract key_extract;
 	typedef typename key_extract::result_type key_type;
 	typedef Compare key_compare;
 	
	//TODO: invoke sortable ctors properly
 	searcher(key_extract const& extract = key_extract(), 
 			 key_compare const& compare = key_compare(),
			 /*node_lower_bound const& node_lower = node_lower_bound(),*/ 
			 const container_type& sortable = container_type())
 	 : c(/* sortable */), //copyconstr? FIXME TODO
 	   /* m_node_lower(node_lower_bound()),*/ 
 	   comp(compare), ext(extract) { }
 	   	 
 	/**
	 * Returns a read/write ("mutable") iterator to first value in the 
	 * %searcher. Iteration is done in ascending order as  dictated by comparing
	 * the keys (extracted from the values via %extract) using %cmp.
	 */
 	iterator begin()
	{
		return c.begin();
	}

	/**
	 * Returns a read-only const_iterator to the first value in the %searcher. 
	 * Iteration is done in ascending order as dictated by comparing the keys 
	 * (extracted from the values via %extract) using %cmp.
	 */ 	 
	const_iterator begin() const
	{
		return c.begin();
	}
 	   	 
	/**
	 * Returns a read/write ("mutable") iterator to the position one past the 
	 * last value in the %searcher. Iteration is done in ascending order as 
	 * dictated by comparing the keys (extracted from the values via 
	 * %extract) using %cmp.
	 */
	iterator end()
	{
		return c.end();
	}
	
	/**
	 * Returns a read-only const_iterator to the position one past the last 
	 * value in the %searcher. Iteration is done in ascending order as dictated 
	 * by comparing the keys (extracted from the values via 
	 * %extract) using %cmp.
	 */
	const_iterator end() const
	{
		return c.end();
	}
	
	// include search algorithms or not?
	// are their interfaces congruent for sequences and hierarchies?
//	iterator lower_bound (key_type const& k)
//	{
//		
//	}


 	/**
	 * @brief		Attempts to insert @a val into the %searcher
	 * @param val	Value to be inserted
	 * @return		A pair, whose 
	 * 				- second member is a bool that is true iff the
	 * 				insertion was successful (which is only the case if there 
	 * 				was no element with the same key as @a val before the
	 * 				attempted insertion)
	 * 				- and whose first member is an iterator indicating the 
	 * 				position where @val was inserted (if the second pair member 
	 * 				is true) or the element's position whose key part is equal 
	 * 				to @a val's (if the second pair member is false)
	 */
	std::pair<iterator, bool> insert(value_type const& val)
	{
		std::pair<iterator, std::pair<bool, bool> > ret = 
			key_lower_bound(c, ext(val), 
			/*m_node_lower,*/ ext, comp);
 		if (ret.second.first) // true if it's not there yet.
			return std::make_pair(c.insert(ret.first, val), true);
		return std::make_pair(iterator(ret.first), false);
 	}
 	
	
	/**
	 * @brief		Attempts to insert @a val into the %searcher
	 * @param pos	An iterator that serves as a hint to indicate the position
	 * 				in front of which @a val must be inserted so the %searcher's
	 * 				order (determined by the key part of @a val) is kept.
	 * @param val	Value to be inserted
	 * @return		An iterator pointing to the element whose key is equal to
	 * 				@a val's (may be different from @a val if such an element
	 * 				already existed before the attempted insertion).
	 */
	iterator insert (iterator pos, value_type const& val)
 	{
 		key_type key = ext(val);
		// FIXME: hint check isn't quite correct yet.
		if ((pos != iterator(c.end())) && comp(key, *pos)) {
			iterator pre = pos;
			--pre;
			if (comp(*pre, key))
 				return c.insert(pos, val);
 		} 
 		
 		// the pos hint was not useful
 		std::pair<cursor, std::pair<bool, bool> > ret = 
 			key_lower_bound(c, key,
 			/*m_node_lower,*/ ext, comp);
 		if (ret.second.first) // true if it's not there yet.
 			return iterator(c.insert(ret.first, val));
 		return iterator(ret.first);
 	}
 	

 	template <class InputIterator>
	void insert (InputIterator a, InputIterator b)
	{
		
	}
	
	/** 
	 * @brief Erases the value at pos
	 * @param pos	The iterator pointing to the value to be erased.
	 */
 	void erase (iterator pos)
 	{
 	}
 	
 	size_type erase (key_type const& k)
 	{
 	}
 	
 	void erase (cursor a, cursor b)
 	{
 	}
 	
 	void clear()
 	{
 		c.clear();
 	}
 	
 	// If we want to keep this, we need copy semantics.
 	container_type get_container() const 
 	{
 		return c;
 	}
 	
// protected. It's a sequence (and hierarchy) adaptor, and the STL ones do the 
// same.
protected: 
 	container_type c;
 	key_compare comp;
 	key_extract ext;
 	
// TODO: kick this out finally.
//private:
// 	node_lower_bound m_node_lower;

};

/* @brief Multiple key %searcher specialization
 * 
 * This class models the 
 * \link http://www.sgi.com/tech/stl/MultipleSortedAssociativeContainer.html
 * Multiple Sorted Associative Container \endlink concept. Refer to the 
 * unspecialized template's documentation for further information.
 */
// (TODO)

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_SEARCHER_HPP


