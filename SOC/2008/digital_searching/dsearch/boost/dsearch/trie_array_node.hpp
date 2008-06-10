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

template <typename trie_type>
class trie_array_node_iterator
:public iterator_facade<trie_array_node_iterator<trie_type>,trie_type *,boost::bidirectional_traversal_tag>
{
	private:
	friend class boost::iterator_core_access;
	template<class Trie_t> friend class trie_array_node_iterator;

	template<class K,class M,class Ke,class Allocator> friend class trie_array_node;

	struct enabler {};

	trie_type** child_ptr;
	std::size_t pos;

	public:
	typedef trie_array_node_iterator<trie_type> type;
	trie_array_node_iterator()
	:child_ptr(0), pos(0){}

	trie_array_node_iterator(trie_type*node)
	{
		child_ptr=(trie_type**)(node->child_ptr);
		int i=0;
		for(i=0;i<trie_type::max;i++)
			if(child_ptr[i]!=0) break;
		pos=i;
	}

	trie_array_node_iterator(trie_type *node,int p):pos(p)
	{
		child_ptr=(trie_type**)node->child_ptr;
	}
	private:
	template<class Trie_t>
	bool equal( trie_array_node_iterator<Trie_t> const &other ) const
	{
		if(other.child_ptr==child_ptr && other.pos==pos)
			return true;
		return false;
	}

	void increment()
	{
		for(pos++; pos<trie_type::max; pos++)
			if(child_ptr[pos]!=0) break;
	}

	void decrement()
	{
		for(pos--; pos>0; pos--)
			if(child_ptr[pos]!=0) break;
	}

	trie_type *&dereference() const 
	{
		return child_ptr[pos];  
	}

	public:
	template<class Trie_t>
	trie_array_node_iterator(const trie_array_node_iterator<Trie_t> &other,
			typename enable_if< is_convertible<Trie_t*,trie_type*>, 
			enabler >::type = enabler()
		     )
	{
		child_ptr=(trie_type **)(other.child_ptr);//converting from x* to const x*
		pos=other.pos;
	}
};

//TODO:const_cast (if safe) at other places to avoid duplication. Remove const functions here.
template<class Key,class Mapped,class Key_traits,class Alloc=std::allocator<char> >
class trie_array_node
{
	public:
	typedef trie_array_node<Key,Mapped,Key_traits,Alloc> type;
	template<class T> friend class trie_array_node_iterator;

	private:
	type* child_ptr[Key_traits::max+1];
	//typedef typename Alloc::template rebind<node_type>::other node_allocator_type;

	public:
	typedef typename Key_traits::element_type element_type;
	typedef trie_array_node_iterator<type> iterator;
	typedef trie_array_node_iterator<const type> const_iterator;
	typedef element_type key_type;
	typedef type* value_type;

	Mapped *value_ptr;

	enum{
		max=Key_traits::max
	};

	trie_array_node()
	{
//		std::cout<<"here"<<std::endl;
		memset(child_ptr,0,sizeof(child_ptr));
		value_ptr=0;
	}

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

	void insert(const element_type &key,type * const &child_cursor)
	{
		child_ptr[Key_traits::get_value(key)]=child_cursor;
	}


	//*iterator should give reference to the value Mapped to by key;
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

	void erase(const iterator&it)
	{
		child_ptr[it.pos]=0;
	}

	iterator begin()
	{
		return iterator(this);
	}

/*	const_iterator begin() const
	{
		return const_iterator(this);
	}*/

	iterator end()
	{
		return iterator(this,max);
	}

/*	const_iterator end() const
	{
		return const_iterator(this,max);
	}*/

	//called only from insert function and trie::iterator 
	Mapped &get_value_ref()
	{
		if(value_ptr==0)
			value_ptr=new Mapped;
		return *value_ptr;
	}

	//called to get const reference fron const_* iterators and cursors
	Mapped &get_value_ref() const
	{
		return *value_ptr;
	}

	void erase_value()
	{
		delete value_ptr;
		value_ptr=0;
	}

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

	element_type get_element(const iterator &it)
	{
		return Key_traits::get_element(it.pos);
	}

/*	iterator lower_bound(const element_type &e) 
	{
		int k=Key_traits::get_value(e);
		for(;k>=0;k--) 
			if(child_ptr[k]!=0) break;
		if(k<0)	return end();
		return iterator(this,k);
	}*/

	bool empty() const
	{
		for(int i=0;i<max;i++)
		if(child_ptr[i]!=0)
			return false;
		return true;
	}
	
	element_type get_element(const_iterator it) const
	{
		return Key_traits::get_element(it.pos);
	}

	~trie_array_node()
	{
		if(value_ptr!=0)
			delete value_ptr;
	}
};

}
}
#endif //BOOST_DSEARCH_TRIE_ARRAY_NODE_HPP

