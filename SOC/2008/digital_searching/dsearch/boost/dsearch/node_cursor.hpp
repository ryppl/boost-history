//  (C) Copyright Chintan Rao H 2008. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_DSEARCH_NODE_CURSOR_HPP
#define BOOST_DSEARCH_NODE_CURSOR_HPP

#include <boost/dsearch/from_tree/cursor_helpers.hpp> //namespace boost::tree
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include<assert.h>

namespace boost {
namespace dsearch {
	using boost::tree::cursor_facade;
	
///cursor poiting to the node.
/**
 	\param Key is trie::key_type
 	\param Mapped is trie::data_type
 	\param Node is the type of node to be used.
 */
template<class Key,class Mapped,class Node>
class trie_cursor:public cursor_facade<trie_cursor<Key,Mapped,Node>,Node,
				forward_traversal_tag,
				bidirectional_traversal_tag>

{
	private:
	/// cursor
	typedef trie_cursor<Key,Mapped,Node> cursor;
	/// element_type as spefied in the key_traits
	typedef typename Node::element_type element_type;

	friend class boost::iterator_core_access;
	friend class boost::tree::cursor_core_access;
	template<class K,class M,template<class K1,class M1,class K_t1,class A1 > class t_n,class K_t,class A >  
	friend class trie;
	template<class K,class M,class N> friend class trie_cursor;
	
	/// helps in avoiding conversion of const_cursor to cursor.
	struct enabler {};
	
	///if Node is const the node_iterator is const_iterator otherwise its 
	///just iterator.
	typedef typename mpl::eval_if<is_const<Node>, typename Node::const_iterator, 
			typename Node::iterator>::type node_iterator;

	/// if the cursor points to root. then only root is set.
	unsigned char only_root;
	/// for specifying the imaginary node above root
	bool top;
	/// position of the child in cur. (cur,pos) specfies the child node
	/// ie the node which cursor points to.
	node_iterator pos;
	/// usually the parent of the node which cursor points to.
	Node *cur;



	public:
	/// Default constructor.
	trie_cursor():only_root(0),top(0),cur(0)
	{
	}

	/// copy constructer 
	/**
	 \param other is the cursor to be copied
	 */
	template<class K,class M,class N>
	trie_cursor( 	trie_cursor<K,M,N> const& other,
				typename enable_if< is_convertible<N*,Node*>, 
				enabler >::type = enabler()
		   )
	{
		this->only_root=other.only_root;
		this->top=other.top;
		this->pos=other.pos;
		this->cur=other.cur;
	}

	///specific the cursor using the root.
	/**
	 	\param n_ptr is the pointer to the root.
	 */
	trie_cursor(Node * const &n_ptr): only_root(1) , top(false)
	{
		cur=n_ptr;
	}

	///construct the node usin node pointer and iterator.
	/**
	 \param n_ptr pointer to the node.
	 \param it: is the node iterator in the child.
	 */
	trie_cursor(Node* const &n_ptr,const node_iterator &it): only_root(0), top(false)
	{
		cur=(n_ptr);
		pos=it;
	}

	private:

	///for cursor::begin()
	cursor left() const
	{
		if(top)
		{
			return cursor(cur);
		}
		if(only_root)
			return cursor(cur,cur->begin());
		return cursor(*pos,(*pos)->begin());
	}

	///for cursor::end()
	cursor right() const
	{
		if(top)
			return (++cursor(cur));
		if(only_root)
			return cursor(cur,cur->end());
		return cursor(*pos,(*pos)->end());
	}

	/// decrement the cursor.
	void decrement()
	{
		if(only_root)
			only_root--;
		else
			pos--;
	}

	/// check whether the particular node is leaf node or not.
	bool empty_() const
	{
		return get_node()->empty();
	}

	/// dereference the node
	/**
	 	\returns reference to the Node
	*/
	Node &dereference() const
	{
		return *get_node();
	}

	///  get the current node.
	/**
	 	\returns the pointer to current node.
	 */
	Node *get_node() const
	{
		if(only_root)
			return cur;
		return *pos;
	}

	///  get the current node.
	/**
	 	\returns the pointer to current node.
	 */
	Node *get_node()
	{
		if(only_root)
			return cur;
		return *pos;
	}
	
	///get the iterator pointing to the node.
	/**
	\warning returns const_iterator for const_cursor 
	\note used by trie erase function
	*/
	node_iterator get_iterator() const
	{
		assert(!only_root);
		return pos;
	}

	///increment the cursor.
	void increment()
	{
		if(only_root)
			only_root++;
		else
			pos++;
	}

	/// used to check whether two cursors are equal.
	/**
	 \returns true if the are equyal false otherwise
	 \param other is other cursor whose equality with cursor needs to be
	 checked
	 
	 */
	template<class K,class M,class N>
	bool equal(trie_cursor<K,M,N> const& other) const
	{
		if(top==other.top)
		{
			if(top) return true;
		}
		else
			return false;
		if(only_root==other.only_root) 
		{
			if (only_root==1 || only_root==2)
				return	true;
			assert(only_root==0);
		}
		else 
			return false;

		if(cur==other.cur && pos==other.pos) return true;
			return false;
	}

	public:
	
	/// check whether node has a value. ie end of an insert key.
	/**
	 	\returns true is it the end of an inserted key.
	 */
	bool has_value() const
	{
		return get_node()->has_value();
	}

	
	/// get value at the particular node. Should have a value.
	/**
	 \returns reference to the value.
	 \note const because dereference is const.
	 \warning should not be called when has_value is false.
	 */
	Mapped &get_value() const
	{
		return 	get_node()->get_value_ref();
	}
	
	/// get element of the edge which points to the node.
	/**
	 \returns the element.
	*/
	element_type get_element() const
	{
		assert(!only_root && !top);
		return cur->get_element(pos);
	}
	
	///find the cursor corresponding to the element
	/**
	\returns cursor pointed to be edge specified by e
	\param e is the egde for which cursor is to be found
	\note const_cast here? trouble is only with const_cursor as get_node()=*pos = const Node *:(.
	*/
	cursor find(const element_type &e) const
	{
		return cursor(this->get_node(),get_node()->find(e));
	}
};
}
}

#endif //BOOST_DSEARCH_NODE_CURSOR_HPP
