#ifndef BOOST_DSEARCH_NODE_CURSOR_HPP
#define BOOST_DSEARCH_NODE_CURSOR_HPP

#include <boost/dsearch/from_tree/cursor_helpers.hpp> //namespace boost::tree
#include<assert.h>

namespace boost {
namespace dsearch {
	using boost::tree::cursor_facade;
	template<class Key,class Mapped,class Node>
	class trie_cursor:public cursor_facade<trie_cursor<Key,Mapped,Node>,Node,
				forward_traversal_tag,
				bidirectional_traversal_tag>

{
	private:
	typedef trie_cursor<Key,Mapped,Node> cursor;
	typedef typename Node::element_type element_type;
	
	friend class boost::iterator_core_access;
	friend class boost::tree::cursor_core_access;
	unsigned char only_root;
	bool top;

	Node *cur;
	typedef typename Node::iterator node_iterator;
	node_iterator pos;

	public:
	trie_cursor()//	:only_root(0),cur(0),top(0) (gave warnings!?!)
	{
		only_root=0;cur=0;top=0;
	}

	trie_cursor(Node * const n_ptr): only_root(1) , top(false)
	{
		cur=n_ptr;
	}

	trie_cursor(const Node * n_ptr,const node_iterator it): only_root(0), top(false)
	{
		cur=const_cast<Node *>(n_ptr);
		pos=it;
	}

	private:
	bool equal(cursor const& other) const
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

	cursor right() const
	{
		if(top)
			return (++cursor(cur));
		if(only_root)
			return cursor(cur,cur->end());
		return cursor(*pos,(*pos)->end());
	}

	void decrement()
	{
		if(only_root)
			only_root--;
		else
			pos--;
	}
		
	bool empty_() const
	{
		return get_node()->empty();
	}

	Node &dereference() const
	{
		return *get_node();
	}

	Node *get_node() const
	{
		if(only_root)
			return cur;
		return *pos;
	}

	void increment()
	{
		if(only_root)
			only_root++;
		else
			pos++;
	}

	public:
	bool has_value() const
	{
		return get_node()->has_value();
	}

	Mapped &get_value() const
	{
		return 	get_node()->get_value_ref();
	}
		
	cursor find(const element_type &e)
	{
		return cursor(get_node(),get_node()->find(e));
	}
};
}
}

#endif //BOOST_DSEARCH_NODE_CURSOR_HPP

