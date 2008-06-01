#ifndef BOOST_DSEARCH_TRIE_ARRAY_NODE_HPP
#define BOOST_DSEARCH_TRIE_ARRAY_NODE_HPP

#include<iostream>
#include<string.h>
#include<boost/iterator/iterator_facade.hpp>
#include<memory>

namespace boost{
namespace dsearch{


//template<class Key,class Mapped,class Key_traits,class Allocator=std::allocator<char> >
//class trie_array_node;

template <typename trie_type>
class trie_array_node_iterator
:public iterator_facade<trie_array_node_iterator<trie_type>,trie_type *,boost::bidirectional_traversal_tag>
{
	private:
	trie_type** child_ptr;
	std::size_t pos;
	template<class K,class M,class Ke,class Allocator> friend class trie_array_node;
	
	public:
	typedef trie_array_node_iterator<trie_type> type;
	trie_array_node_iterator()
	:child_ptr(0), pos(0){}

	trie_array_node_iterator(trie_type*node)
	{
		child_ptr=(trie_type**)node->child_ptr;
		int i=0;
		for(i=0;i<trie_type::max;i++)
			if(child_ptr[i]!=0) break;
		pos=i;
	}

	trie_array_node_iterator(trie_type *node,int p):pos(p)
	{
		child_ptr=(trie_type**)node->child_ptr;
	}

	bool equal( const type  &other ) const
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
};

//TODO:write a copy constructor for good
template<class Key,class Mapped,class Key_traits,class Alloc=std::allocator<char> >
class trie_array_node
{
	public:
	typedef trie_array_node<Key,Mapped,Key_traits,Alloc> type;
	friend class trie_array_node_iterator<type>;

	private:
	typedef typename Key_traits::element_type element_type;
	type* child_ptr[Key_traits::max+1];

	public:
	typedef trie_array_node_iterator<type> iterator;
	typedef element_type key_type;
	typedef type* value_type;
	bool value_indicator;
	Mapped value;			//should it be mapped *? depending on sizeof(mapped)

	enum{
		max=Key_traits::max
	};

	
	trie_array_node()
	{
		std::cout<<"here"<<std::endl;
		value_indicator=false;
		memset(child_ptr,0,sizeof(child_ptr));
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

	void erase(const iterator&it)
	{
		child_ptr[it.pos]=0;
	}

	iterator begin()
	{
		return iterator(this);
	}

	iterator end()
	{
		return iterator(this,max);
	}

	/*void insert_value(const Mapped &v)
	{
		value=v;
	}*/

	Mapped &get_value_ref() //get pointer to value of [] operator of trie class
	{
		value_indicator=true;
		return value;
	}

	void erase_value()
	{
		value_indicator=false;
	}

	bool has_value()
	{
		return value_indicator;
	}

	std::size_t size()
	{
		int t_size=0;
		for(int i=0;i<max;i++)
			if(child_ptr[i]!=0)
				t_size++;
		return t_size;
	}

	bool empty()
	{
		for(int i=0;i<max;i++)
		if(child_ptr[i]!=0)
			return false;
		return true;
	}

	~trie_array_node()
	{
	}
};

}
}
#endif //BOOST_DSEARCH_TRIE_ARRAY_NODE_HPP

