#ifndef BOOST_DSEARCH_TRIE_ITERATOR_HPP
#define BOOST_DSEARCH_TRIE_ITERATOR_HPP
#include<iostream>
#include<stack>
#include<boost/iterator/iterator_facade.hpp>
#include<assert.h>
#include<vector>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

//TODO:Add specializations for ascending cursors without use of stack.

namespace boost{
namespace dsearch{

template<class Key,class Mapped,class Cursor>
class trie_iterator
:public iterator_facade< trie_iterator<Key,Mapped,Cursor>,Mapped,bidirectional_traversal_tag >
{
	private:
	friend class boost::iterator_core_access;
	template<class K,class M,template<class K1,class M1,class K_t1,class A1 > class t_n,class K_t,class A >  
	friend class trie;
	template<class K,class M,class C> friend class trie_iterator;

	struct enabler {};
	typedef trie_iterator<Key,Mapped,Cursor> self;

	std::vector<Cursor> cur_st;
	int cur_size;
	bool end_flag;

	void push ( const Cursor &c )
	{
		++cur_size;
		cur_st.push_back(c);
	}
	
	void pop()
	{
		--cur_size;
		cur_st.pop_back();
	}

	Cursor &top() 
	{
		assert(!(this->empty()));
		return cur_st[cur_size-1];
	}
	
	std::size_t size()
	{
		return cur_size;
	}

	Cursor get_top() const
	{
		assert(!(this->empty()));
		return cur_st[cur_size-1];
	}

	bool empty() const
	{
		return cur_st.empty();
	}

	void to_right_most()
	{
		Cursor c=this->top();
//		std::cout<<"in right most"<<std::endl;
		while(!c.empty())
		{
//			std::cout<<"PUSHING"<<std::endl;
			c=--c.end();
			this->push(c);
		}
		assert(this->top().has_value());
	}

	template<class K,class M,class C>
	bool equal(trie_iterator<K,M,C> const& other) const
	{
		if( this->end_flag == other.end_flag && this->get_top() == other.get_top() )
			return true;
		return false;
	}

	void to_left_most()
	{
		Cursor c=this->top();
		if(c.has_value()) 
		{
//			std::cout<<"in has_value"<<std::endl;
			return ;
		}
//		std::cout<<"out of has_value"<<std::endl;
		while(!c.empty())
		{
//			std::cout<<"PUSHING"<<std::endl;
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
		this->push (top);
		to_left_most();
	}

	Mapped &dereference() const
	{
		assert ( (cur_st[cur_size-1]).has_value() );
		return ( (cur_st[cur_size-1]).get_value() );
	}
	
	bool equal( const self &other ) const
	{
		assert(!this->empty());
		if(end_flag!=other.end_flag) return false;
		if(other.cur_st[other.cur_size-1]==cur_st[cur_size-1])
			return true;
		return false;
	}

	public:
	trie_iterator():end_flag(false) //to be pushed later.. thats why
	{
		cur_size=0;
	}

	trie_iterator( Cursor const &cursor_root,bool end_flag=false ) //returns begin cursor
	{
		cur_size=0;
		this->end_flag=end_flag;
		if(this->end_flag)
		{
			this->push(cursor_root);
			return;
		}

		if( cursor_root.empty() && !cursor_root.has_value() ) //special case of only empty root
		{
//			std::cout<<"IN"<<std::endl;
			this->end_flag=true;
			this->push(cursor_root);
			return;
		}
//		std::cout<<"OUT"<<std::endl
		this->push(cursor_root);
		to_left_most();
	}

	public:
	template<class K,class M,class C>
	trie_iterator( trie_iterator<K,M,C> const& other,
			typename enable_if< is_convertible<M*,Mapped*>, 
			enabler >::type = enabler()
		     )
	{
		end_flag=other.end_flag;
		cur_size=0;

		typename std::vector<C>::const_iterator it=other.cur_st.begin();
		while ( it!=other.cur_st.end() )
		{
			push(*it);
			it++;
		}
	}
};

template<class Key,class Mapped,class Cursor>
class trie_reverse_iterator
:public iterator_facade< trie_reverse_iterator<Key,Mapped,Cursor>,Mapped,bidirectional_traversal_tag >
{
	private:
	friend class boost::iterator_core_access;
	template<class K,class M,template<class K1,class M1,class K_t1,class A1 > class t_n,class K_t,class A >  
	friend class trie;

	struct enabler {};
	template<class K,class M,class C> friend class trie_reverse_iterator;
	typedef trie_reverse_iterator<Key,Mapped,Cursor> self;
	typedef trie_iterator<Key,Mapped,Cursor> iterator;

	iterator correspond_it,beg_it;
	bool end_flag;

	void increment()
	{
		assert(!end_flag);
		if(beg_it==correspond_it)
			end_flag=true;
		else
		--correspond_it;
	}
	
	const iterator &get_iterator() const
	{
		return correspond_it;
	}

	void decrement()
	{
		if(end_flag)
			end_flag=false;
		else
			++correspond_it;
	}

/*	bool equal(const self &other) const
	{
		return ( this->correspond_it==other.correspond_it  && this->end_flag==other.end_flag);
	}
*/
	template<class K,class M,class C>
	bool equal(trie_reverse_iterator<K,M,C> const& other) const
	{
		if( this->end_flag == other.end_flag && this->correspond_it==other.correspond_it )
			return true;
		return false;
	}

	Mapped &dereference() const
	{
		assert(!end_flag);
		return *correspond_it;
	}

	public:
	trie_reverse_iterator(): end_flag(0)
	{
	}
	trie_reverse_iterator(const iterator &beg_it,const iterator &end_it,bool is_beg_flag)//should be intialized with end iterator
	{
		if(is_beg_flag)
		{
			this->beg_it=beg_it;
			correspond_it=end_it;
			--correspond_it;
			end_flag=false;
		}
		else
		{
			this->beg_it=correspond_it=beg_it;
			end_flag=true;
		}
	}

	template<class K,class M,class C>
	trie_reverse_iterator( 	trie_reverse_iterator<K,M,C> const& other,
				typename enable_if< is_convertible<M*,Mapped*>, 
				enabler >::type = enabler()
		    		)
	{
		this->end_flag=other.end_flag;
		this->correspond_it=other.correspond_it;
		this->beg_it=other.beg_it;
	}
};

}
}

#endif //BOOST_DSEARCH_TRIE_ITERATOR_HPP
