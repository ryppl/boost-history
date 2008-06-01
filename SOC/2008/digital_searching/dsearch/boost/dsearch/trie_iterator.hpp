#include<iostream>
#include<stack>
#include<boost/iterator/iterator_facade.hpp>
#include<assert.h>

//forces the thing to be bidirectional horizontally.
//TODO:Add specializations for ascending cursors without use of stack.
//BUG:currently this has no indicator to say that a particular thing is pointing to a value.

namespace boost{
namespace dsearch{

template<class Key,class Mapped,class Cursor>
class trie_iterator
:public iterator_facade< trie_iterator<Key,Mapped,Cursor>,Mapped,bidirectional_traversal_tag >
{
	private:
	std::stack<Cursor> cur_st;
	friend class iterator_core_access;
	bool end_flag;
	void to_right_most()
	{
		Cursor c=cur_st.top();
		while(c.begin()!=c.end())
		{
			c=--c.end();
			cur_st.push(c);
			if(c.has_value()) break;
		}
		assert(cur_st.top().has_value());
	}

	void to_left_most()
	{
		Cursor c=cur_st.top();
		if(c.has_value()) return ;
		while(c.begin()!=c.end())
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
		Cursor top=cur_st.top();
		cur_st.pop();
		while(cur_st.top().begin()==top)
		{
			if(top.has_value())
				return;
			top=cur_st.top();
			cur_st.pop();
		}
		--top;
		cur_st.push(top);
		to_right_most();
	}

	void increment()
	{
		Cursor top=cur_st.top();
		cur_st.pop();
		while(cur_st.top().end()==top+1)
		{
			top=cur_st.top();
			cur_st.pop();
		}
		++top;
		cur_st.push(top);
		to_left_most();
	}

	Mapped &dereference() const
	{
		assert(cur_st.top().has_value());
		return cur_st.top().get_value();
	}

	public:
	trie_iterator():end_flag(0) //An end cursor
	{} 
	trie_iterator(Cursor const &cursor_root) //returns begin cursor
	{
		Cursor c=cursor_root;
/*		if(c.begin()==c.end() && c.)
		{
			end_flag=true;
			return;
		}
		end_flag=false;*/
		cur_st.push(c);
		to_left_most();
	}
};

}
}
