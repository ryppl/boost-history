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
		assert(!(this->empty()));
		return cur_st.top();
	}

	bool empty() const
	{
		return cur_st.empty();
	}

	void to_right_most()
	{
		Cursor c=this->top();
		std::cout<<"in right most"<<std::endl;
		while(c.begin()!=c.end())
		{
			std::cout<<"PUSHING"<<std::endl;
			c=--c.end();
			this->push(c);
		}
		assert(this->top().has_value());
	}

	void to_left_most()
	{
		Cursor c=this->top();
		if(c.has_value()) 
		{
			std::cout<<"in has_value"<<std::endl;
			return ;
		}
		std::cout<<"out of has_value"<<std::endl;
		while(!c.empty())
		{
			std::cout<<"PUSHING"<<std::endl;
			if(c.has_value()) break;
			c=c.begin();
			this->push(c);
		}
		assert(this->top().has_value());
	}

	void decrement()
	{
		bool first_loop=true;
		Cursor top;
		if(end_flag)
		{
			to_right_most();
			end_flag=false;
			return;
		}
		
		assert(!this->empty());
		//pop();
		top=this->top();
		this->pop();
		if(this->empty())
		{
			this->push(top);
			end_flag=true;
			return;
		}
		while(top==this->top().begin())
		{
			if(top.has_value() && !first_loop) 
			{
				this->push(top); //to make sure to_right_most still works
				return;
			}
			first_loop=false;
			top=this->top();
			this->pop();
			if(this->empty())
			{
				this->push(top);
				if(!top.has_value())
					end_flag=true;
				return;
			}
			
		}
		--top;
		this->push(top);
		to_right_most();
	}

	void increment()
	{
		assert(end_flag==false && NULL!="incrementing at end");//just for debugging
		assert(!this->empty() && NULL!="incrementing before begin");
		Cursor top=this->top().begin();

		while(this->top().end()==top)
		{
			top=this->top();
			this->pop();
			if(this->empty())
			{
				this->push(top);
				end_flag=true;
				return;
			}
			++top;
		}
		this->push(top);
		to_left_most();
	}

	Mapped &dereference() const
	{
		assert((cur_st.top()).has_value());
		return ((cur_st.top()).get_value());
	}
	
	bool equal(const self &other) const
	{
		assert(!this->empty());
		if(end_flag!=other.end_flag) return false;
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
		this->end_flag=end_flag;
		if(this->end_flag)
		{
			this->push(cursor_root);
			return;
		}

		if(cursor_root.empty() && !cursor_root.has_value()) //special case of only empty root
		{
			end_flag=true;
			this->push(cursor_root);
			return;
		}
		this->push(cursor_root);
		to_left_most();
	}
};

}
}

#endif //BOOST_DSEARCH_TRIE_ITERATOR_HPP
