#ifndef BOOST_DSEARCH_TRIE_ITERATOR_HPP
#define BOOST_DSEARCH_TRIE_ITERATOR_HPP
#include<iostream>
#include<stack>
#include<boost/iterator/iterator_facade.hpp>
#include<assert.h>
#include<vector>
#include<boost/type_traits/is_convertible.hpp>
#include<boost/utility/enable_if.hpp>


namespace boost{
namespace dsearch{

/// iterator class for trie
/**
 	\brief iterator is implemented using a stack of cursor.
 	\param Key is patricia::key_type
 	\param Mapped is patricia::data_type
 	\param Cursor is the type of cursor to be used iterate through the node.
 	\todo add specialization when the node has a parent pointer
 */
template<class Key,class Mapped,class Cursor>
class trie_iterator
:public iterator_facade< trie_iterator<Key,Mapped,Cursor>,Mapped,bidirectional_traversal_tag >
{
	private:
	friend class boost::iterator_core_access;
	template<class K,class M,template<class K1,class M1,class K_t1,class A1 > class t_n,class K_t,class A >  
	friend class trie;
	template<class K,class M,class C> friend class trie_iterator;

	/// helps in avoiding conversion of const_iterator to iterator.
	struct enabler {};
	/// Self.
	typedef trie_iterator<Key,Mapped,Cursor> self;

	/// "stack" of cursors.
	std::vector<Cursor> cur_st;
	/// size of the stack.
	int cur_size;
	/// flag is true if the iterator is end()
	bool end_flag;

	/// push cursor into the stack
	/**
	 	\param c is the cursor to be pushed.
	 */
	void push ( const Cursor &c )
	{
		++cur_size;
		cur_st.push_back(c);
	}
	
	/// pop the cursor from the stack.
	void pop()
	{
		--cur_size;
		cur_st.pop_back();
	}

	/// get reference to top of the stack
	Cursor &top() 
	{
		assert(!(this->empty()));
		return cur_st[cur_size-1];
	}
	
	/// returns the size of the stack
	std::size_t size()
	{
		return cur_size;
	}

	/// get the top of the stack
	Cursor get_top() const
	{
		assert(!(this->empty()));
		return cur_st[cur_size-1];
	}

	///checks whether the stack is empty.
	bool empty() const
	{
		return cur_st.empty();
	}

	///go to the right most leaf from the current top.
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

	///check whether two iterators are equal.
	/**
	 	\param other is the other iterator. It can be const or non const.
	 */
	template<class K,class M,class C>
	bool equal(trie_iterator<K,M,C> const& other) const
	{
		if( this->end_flag == other.end_flag && this->get_top() == other.get_top() )
			return true;
		return false;
	}

	///go to the left most leaf from the current top.
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

	///decrement the iterator. 
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
		
		//* if we one the left edge of a node.
		while(top==this->top().begin())
		{
			//* If top has value then we need to stop.
			if(top.has_value() && !first_loop) 
			{
				//* to make sure to_right_most still works
				this->push(top); 
				return;
			}
			
			first_loop=false;
			top=this->top();
			this->pop();
			
			//* we have reached the and empty stack? then we have reached 
			//* then we can go up no more.Also is top has value then 
			//* root has value.
			if(this->empty())
			{
				this->push(top);
				if(!top.has_value())
					end_flag=true;
				return;
			}
		}
		
		//finally we found an edge which we can shift to the right
		--top;
		this->push(top);
		
		//* move to the right most.
		to_right_most();
	}

	///increment the iterator.
	void increment()
	{
		assert(end_flag==false && NULL!="incrementing at end");
		assert(!this->empty() && NULL!="incrementing before begin");
		Cursor top=this->top().begin();

		//*if if it is the leaf node || we reached the edge a node.
		while(this->top().end()==top)
		{
			//* pop in search of a new top.
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
		//* go to left most.
		to_left_most();
	}

	///return a reference to the node the the is pointing to.
	Mapped &dereference() const
	{
		assert ( (cur_st[cur_size-1]).has_value() );
		return ( (cur_st[cur_size-1]).get_value() );
	}
	
	/*
	bool equal( const self &other ) const
	{
		assert(!this->empty());
		if(end_flag!=other.end_flag) return false;
		if(other.cur_st[other.cur_size-1]==cur_st[cur_size-1])
			return true;
		return false;
	}*/

	public:
	///default constructor
	trie_iterator():cur_size(0), end_flag(false)
	{	
	}

	///constructor for begin() or end()
	/**
	 \param cursor_root is patricia::node_root
	 \param end_flag specifies whether its begin cursor or end cursor.
	 */
	trie_iterator( Cursor const &cursor_root,bool end_flag=false ) //returns begin cursor
	{
		cur_size=0;
		this->end_flag=end_flag;
		if(this->end_flag)
		{
			this->push(cursor_root);
			return;
		}
		//*special case of only empty root
		if( cursor_root.empty() && !cursor_root.has_value() )
		{
			this->end_flag=true;
			this->push(cursor_root);
			return;
		}

		this->push(cursor_root);
		//goto the begin();
		to_left_most();
	}

	public:
	///copy constructor
	/**
	 \param other is the iterator to be copied
	 */
	template<class K,class M,class C>
	trie_iterator( trie_iterator<K,M,C> const& other,
			typename enable_if< is_convertible<M*,Mapped*>, 
			enabler >::type = enabler()
		     )
	{
		end_flag=other.end_flag;
		cur_size=0;

		//* copy stack elements one by one.
		typename std::vector<C>::const_iterator it=other.cur_st.begin();
		while ( it!=other.cur_st.end() )
		{
			push(*it);
			it++;
		}
	}
};


/// reverse iterator class for trie
/**
 	\brief reverse iterator class is implemented using an iterator.
 	\param Key is patricia::key_type
 	\param Mapped is patricia::data_type
 	\param Cursor is the type of cursor to be used iterate through the node.
 	\todo add specialization when the node has a parent pointer
 */

template<class Key,class Mapped,class Cursor>
class trie_reverse_iterator
:public iterator_facade< trie_reverse_iterator<Key,Mapped,Cursor>,Mapped,bidirectional_traversal_tag >
{
	private:
	friend class boost::iterator_core_access;
	template<class K,class M,template<class K1,class M1,class K_t1,class A1 > class t_n,class K_t,class A >  
	friend class trie;
	template<class K,class M,class C> friend class trie_reverse_iterator;
	
	/// helps in avoiding conversion of const_iterator to iterator.
	struct enabler {};
	/// Self
	typedef trie_reverse_iterator<Key,Mapped,Cursor> self;
	/// type of iterator corresponding to reverse_iterator.
	typedef trie_iterator<Key,Mapped,Cursor> iterator;

	/// use the corresponding iterator to iterate.
	iterator correspond_it;
	/// one must know when one hits rend :). thats this exists
	iterator beg_it;
	/// end_flag=true ==> we have reached rend
	bool end_flag;

	///increments the reverse iterator.
	void increment()
	{
		assert(!end_flag);
		if(beg_it==correspond_it)
			end_flag=true;
		else
		--correspond_it;
	}
	
	/// gets the corresponding iterator.
	const iterator &get_iterator() const
	{
		return correspond_it;
	}

	/// decrements the reverse iterator.
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
	/// checks whether two reverse iterator.
	/**
	 \param other is the other reverse iterator.
	 */
	template<class K,class M,class C>
	bool equal(trie_reverse_iterator<K,M,C> const& other) const
	{
		if( this->end_flag == other.end_flag && this->correspond_it==other.correspond_it )
			return true;
		return false;
	}
	
	/// dereferences iterator.
	Mapped &dereference() const
	{
		assert(!end_flag);
		return *correspond_it;
	}

	public:
	
	/// default constructor
	trie_reverse_iterator(): end_flag(0)
	{
	}
	
	/// constructor of instilaizing to rbegin and rend
	/**
	 	\param beg_it is trie::begin()
	 	\param end_it is trie::end()
	 	\param is_beg_flag is true if its rbegin() otherwise false if its rend()
	 */
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

	///copy constructor
	/**
	 \param other is the reverse_iterator to be copied
	 */
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
