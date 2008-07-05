#ifndef BOOST_DSEARCH_TRIE_ARRAY_NODE_HPP
#define BOOST_DSEARCH_TRIE_ARRAY_NODE_HPP

#include<iostream>
#include<string.h>
#include<assert.h>
#include<boost/iterator/iterator_facade.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include<memory>

namespace boost{
namespace dsearch{

/// iterator for trie array node.
/**
 \param trie_type is the type of trie node used.
 */
template <typename trie_type>
class trie_array_node_iterator
:public iterator_facade<trie_array_node_iterator<trie_type>,trie_type *,boost::bidirectional_traversal_tag>
{
	private:
	friend class boost::iterator_core_access;
	template<class Trie_t> friend class trie_array_node_iterator;
	template<class K,class M,class Ke,class Allocator> friend class trie_array_node;

	/// helps in avoiding conversion of const_iterator to iterator.
	struct enabler {};

	/// pointer to the child array.
	trie_type** child_ptr;
	/// pos is the position in the child array.
	std::size_t pos;

	public:
	///Self
	typedef trie_array_node_iterator<trie_type> type;
	
	/// Default constructor
	trie_array_node_iterator()
	:child_ptr(0), pos(0){}

	/// Intialize a iterator from the node. ie begin().
	trie_array_node_iterator(trie_type*node)
	{
		child_ptr=(trie_type**)(node->child_ptr);
		int i=0;
		//* find the first non zero position in the node.
		for(i=0;i<trie_type::max;i++)
			if(child_ptr[i]!=0) break;
		pos=i;
	}

	/// Intialize the iterator to a particular position.
	trie_array_node_iterator(trie_type *node,int p):pos(p)
	{
		child_ptr=(trie_type**)node->child_ptr;
	}
	private:
	
	/// Used to check whether two iterators are equal.
	template<class Trie_t>
	bool equal( trie_array_node_iterator<Trie_t> const &other ) const
	{
		if(other.child_ptr==child_ptr && other.pos==pos)
			return true;
		return false;
	}

	/// increment the iterator.
	void increment()
	{
		//* check for the next non zero entry.
		for(pos++; pos<trie_type::max; pos++)
			if(child_ptr[pos]!=0) break;
	}

	/// decrement the iterator.
	void decrement()
	{
		for(pos--; pos>0; pos--)
			if(child_ptr[pos]!=0) break;
	}

	/// dereference the iterator
	trie_type *&dereference() const 
	{
		return child_ptr[pos];  
	}

	public:
	///copy constructor
	/**
	 \param other is the iterator to be copied
	 */
	template<class Trie_t>
	trie_array_node_iterator(const trie_array_node_iterator<Trie_t> &other,
			typename enable_if< is_convertible<Trie_t*,trie_type*>, 
			enabler >::type = enabler()
		     )
	{
		//* converting from x* to const x*
		child_ptr=(trie_type **)(other.child_ptr);
		pos=other.pos;
	}
};

/// This makes the trie class use an array for index children in every node.
/**
 \param Key is trie::key_type.
 \param Mapped is trie::data_type.
 \param Alloc is the allocator used.
 */
template<class Key,class Mapped,class Key_traits,class Alloc=std::allocator<char> >
class trie_array_node
{
	public:
	///Self
	typedef trie_array_node<Key,Mapped,Key_traits,Alloc> type;
	template<class T> friend class trie_array_node_iterator;

	private:
	/// An array of children indexed by the element_type.
	type* child_ptr[Key_traits::max+1];
	//typedef typename Alloc::template rebind<node_type>::other node_allocator_type;

	public:
	/// Element_type is type of each element in the key.
	typedef typename Key_traits::element_type element_type;
	
	/// iterator for the array node. Should iterate in increasing order or
	/// decreasing order of elements.
	typedef trie_array_node_iterator<type> iterator;

	/// iterator for the array node. Should iterate in increasing order or
	/// decreasing order of elements.	
	typedef trie_array_node_iterator<const type> const_iterator;
	
	/// element_type is the key_type of trie_array node
	typedef element_type key_type;
	
	/// type* is the value type.
	typedef type* value_type;

	/// stored the value of the array in a pointer
	Mapped *value_ptr;

	/// maximum value of element_type
	enum{
		max=Key_traits::max
	};

	/// Default constructor
	trie_array_node()
	{
//		std::cout<<"here"<<std::endl;
		memset(child_ptr,0,sizeof(child_ptr));
		value_ptr=0;
	}

	/// Copy constructor
	trie_array_node(const type &other)
	{
		if(other.value_ptr!=0)
		{
			value_ptr=new Mapped;
			*value_ptr=*(other.value_ptr);
		}
		else
			value_ptr=0;
		assert( memcpy(child_ptr, other.child_ptr, sizeof(child_ptr) ) == child_ptr);
	}

	/// insert a new child corresponding to the key.
	/**
	 \param key element_type specifying the edge to the child.
	 \param child is the pointer to the child.
	 */
	void insert(const element_type &key,type * const &child)
	{
		child_ptr[Key_traits::get_value(key)]=child;
	}



	//  *iterator should give reference to the value Mapped to by key;
	/// find a particular key in trie_array_node
	/**
	 \param key for which the iterator is to be found.
	 */
	iterator find(const element_type &key)
	{
		if(child_ptr[Key_traits::get_value(key)]==0)
			return end();
		else
			return iterator(this,Key_traits::get_value(key));
	}

/*	//TODO:do a const cast at places to avoid duplication
	const_iterator find(const element_type &key) const
	{
		if(child_ptr[Key_traits::get_value(key)]==0)
			return end();
		else
			return const_iterator(this,Key_traits::get_value(key));
	}*/

	/// erase value correspoding the iterator.
	void erase(const iterator&it)
	{
		child_ptr[it.pos]=0;
	}

	/// returns an iterator which shows the elements in sorted order.
	/**
	 \returns iterator pointing to the least element.
	 */
	iterator begin()
	{
		return iterator(this);
	}

/*	const_iterator begin() const
	{
		return const_iterator(this);
	}*/

	/// returns an iterator which shows the elements in sorted order.
	/**
	 \returns iterator pointing to the greatest element.
	 */	
	iterator end()
	{
		return iterator(this,max);
	}

/*	const_iterator end() const
	{
		return const_iterator(this,max);
	}*/

	/// get reference to value stored
	/**
	 \brief if has_value is false, assign default to value and return reference 
	 to value.
	 Called only by insert and operator []
	 \note This function should be const	
	 */

	Mapped &get_value_ref()
	{
		if(value_ptr==0)
			value_ptr=new Mapped;
		return *value_ptr;
	}

	/// get reference to value stored
	/**
	 \note This function should be const.
	 */
	Mapped &get_value_ref() const
	{
		return *value_ptr;
	}

	///erase value from trie_array_node
	/**
	 */
	void erase_value()
	{
		delete value_ptr;
		value_ptr=0;
	}

	/// check whether this node has value or not.
	/**
	 \returns true if node has a value stored. false otherwise.
	 */
	bool has_value() const
	{
		return value_ptr!=0;
	}

/*	std::size_t size()
	{
		int t_size=0;
		for ( int i=0; i<max; i++ )
			if( child_ptr[i]!=0 )
				t_size++;
		return t_size;
	}*/

/*	iterator lower_bound(const element_type &e) 
	{
		int k=Key_traits::get_value(e);
		for(;k>=0;k--) 
			if(child_ptr[k]!=0) break;
		if(k<0)	return end();
		return iterator(this,k);
	}*/

	/// check whether the node is empty or not.
	/**
	 \returns true if node is empty false otherwise.
	 */
	bool empty() const
	{
		for(int i=0;i<max;i++)
		if(child_ptr[i]!=0)
			return false;
		return true;
	}
	
	///  get element at a particular position specified by the iterator.
	/**
	 \param it is the iterator whose element is to be got.
	 */
	element_type get_element(const_iterator it) const
	{
		return Key_traits::get_element(it.pos);
	}

	/// Class destructor 
	~trie_array_node()
	{
		if(value_ptr!=0)
			delete value_ptr;
	}
};

}
}
#endif //BOOST_DSEARCH_TRIE_ARRAY_NODE_HPP

