#ifndef BOOST_DSEARCH_TRIE_ITERATOR_HPP
#define BOOST_DSEARCH_TRIE_ITERATOR_HPP
#include<iostream>
#include<stack>
#include<boost/iterator/iterator_facade.hpp>
#include<assert.h>

//forces the thing to be bidirectional horizontally.
//TODO:Add specializations for ascending cursors without use of stack.
//iterator not convertible to const iterator

namespace boost{
namespace dsearch{

template<class Key,class Mapped,class Cursor>
class trie_iterator
:public iterator_facade< trie_iterator<Key,Mapped,Cursor>,Mapped,bidirectional_traversal_tag >
{
	private:
	std::stack<Cursor> cur_st;
	typedef trie_iterator<Key,Mapped,Cursor> self;
	friend class boost::iterator_core_access;
	template<class K,class M,template<class K1,class M1,class K_t1,class A1 > class t_n,class K_t,class A >  
	friend class trie;
	bool end_flag;

	void push(const Cursor &c)
	{
		cur_st.push(c);
	}
	
	void pop()
	{
		cur_st.pop();
	}

	Cursor &top()
	{
		assert(!cur_st.empty());
		return cur_st.top();
	}

	void to_right_most()
	{
		Cursor c=cur_st.top();
		std::cout<<"in right most"<<std::endl;
		while(c.begin()!=c.end())
		{
			std::cout<<"PUSHING"<<std::endl;
			c=--c.end();
			cur_st.push(c);
		}
		assert(cur_st.top().has_value());
	}

	void to_left_most()
	{
		Cursor c=cur_st.top();
		if(c.has_value()) return ;
		while(!c.empty())
		{
			std::cout<<"PUSHING"<<std::endl;
			if(c.has_value()) break;
			c=c.begin();
			cur_st.push(c);
		}
		assert(cur_st.top().has_value());
	}

	void decrement()
	{
		Cursor top;
		if(end_flag)
		{
			to_right_most();
			end_flag=false;
			return;
		}
		
		assert(!cur_st.empty());
		cur_st.pop();
		top=cur_st.top();
		cur_st.pop();
		while(top==cur_st.top().begin())
		{
			if(top.has_value()) 
			{
				cur_st.push(top); //to make sure to_right_most still works
				return;
			}
			top=cur_st.top();
			cur_st.pop();
			if(cur_st.empty())
				return;
		}
		--top;
		cur_st.push(top);
		to_right_most();
	}

	void increment()
	{
		assert(end_flag==false && NULL!="incrementing at end");//just for debugging
		assert(!cur_st.empty() && NULL!="incrementing before begin");
		Cursor top=cur_st.top().begin();

		while(cur_st.top().end()==top)
		{
			top=cur_st.top();
			cur_st.pop();
			if(cur_st.empty())
			{
				cur_st.push(top);
				end_flag=true;
				return;
			}
			++top;
		}
		cur_st.push(top);
		to_left_most();
	}

	Mapped &dereference() const
	{
		assert(cur_st.top().has_value());
		return cur_st.top().get_value();
	}
	
	bool equal(const self &other) const
	{
		assert(!cur_st.empty());
		if(other.cur_st.top()==cur_st.top())
			return true;
		return false;
	}

	public:
	trie_iterator():end_flag(false) 
	{
	}

	trie_iterator(Cursor const &cursor_root,bool end_flag=false) //returns begin cursor
	{
		Cursor c=cursor_root;
		this->end_flag=end_flag;
		if(this->end_flag)
		{
			cur_st.push(cursor_root);
			return;
		}

		if(cursor_root.empty() && !cursor_root.has_value()) //special case of only empty root
		{
			end_flag=true;
			cur_st.push(c);
			return;
		}
		cur_st.push(c);
		to_left_most();
	}

	template<class It>
	trie_iterator(const It &begin,const It &end):end_flag(0)
	{
		It it=begin;
		while(it!=end)
		{
			cur_st.push(*it);
			it++;
		}
	}
};

}
}

#endif //BOOST_DSEARCH_TRIE_ITERATOR_HPP
