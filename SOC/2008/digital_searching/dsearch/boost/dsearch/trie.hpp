#ifndef BOOST_DSEARCH_TRIE_HPP
#define BOOST_DSEARCH_TRIE_HPP

#include<limits>
#include<memory>
#include<stack>
#include<algorithm>
#include<assert.h>
#include<boost/dsearch/node_cursor.hpp>
#include<boost/dsearch/trie_iterator.hpp>
#include<vector>

namespace boost{
namespace dsearch{

template<class Key,class Mapped,
template<class K,class M,class K_t,class A > class trie_node, //wonder allocator<char> is correct
class Key_traits,
class Alloc=std::allocator<char> > 
class trie
{
	private:
	typedef trie_node<Key,Mapped,Key_traits,Alloc> node_type;
	friend class trie_node<Key,Mapped,Key_traits,Alloc>;
	typedef typename Alloc::template rebind<node_type>::other node_allocator_type;
	node_allocator_type node_allocator;
	node_type *node_root;
	public:
	typedef Key key_type;
	typedef Mapped data_type;
	typedef std::pair<Key,Mapped> value_type;
	typedef trie<Key,Mapped,trie_node,Key_traits,Alloc> type;

	//need to define an iterator before that will need to define cursor for trie_node
	//TODO iterator, const_iterator, reverse_iterator, const_reverse_iterator, begin(), end()
	//rbegin(),size(),trie(trie &), overload =, lots of other things

	typedef typename Alloc::template rebind<value_type>::other allocator_type;
    	typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer        const_pointer;
	typedef typename allocator_type::reference            reference;
	typedef typename allocator_type::const_reference      const_reference;
	typedef typename allocator_type::size_type            size_type;//should actually depend on iterator(.|?)
	typedef typename allocator_type::difference_type      difference_type; //should actually depend on iterator(.|?)
	
	typedef trie_cursor<key_type,data_type,node_type> cursor;
	typedef trie_iterator<key_type,data_type,cursor> iterator;
//	typedef trie_cursor<key_type,data_type,node_type> cursor;
	typedef trie_iterator<key_type,const data_type,cursor> const_iterator;

	trie()
	{
		node_root=node_allocator.allocate(1);
		new(node_root) node_type();
	}

	data_type &operator [] (const key_type &k) 
	{
		return	insert_(k)->get_value_ref();
	}

	void insert(const value_type &v)
	{
		this->operator[](v.first)=v.second;
	}

	std::size_t size()
	{
		return 0; //for now
	}

	std::size_t max_size()
	{
		return  std::numeric_limits<std::size_t>::max();
		//return (std::size_t)(-1);//does this work everywhere? 
	}

	bool empty()
	{
		return node_root->empty();
	}

	void erase(const key_type &key) 
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
			end_it=Key_traits::end(key),temp_it=it;
		typename node_type::iterator fit;

		node_type* cur=node_root,*par=node_root,*temp_cur=node_root;

		if(it==end_it) return;
		fit=cur->find(*it);
		if(fit==cur->end())
			return;
		cur=*fit;

		it++;
		while(!(it==end_it))
		{
			
			fit=cur->find(*it);
			if(fit==cur->end()) 
			{
				return;
			}
			if(cur->size()!=1 || cur->has_value()) //should we delete things backwards?
			{
				temp_cur=cur;
				temp_it=it;
			}
			cur=*fit;
			it++;
		}

		cur->erase_value();
		if(!cur->empty()) return;

		fit=temp_cur->find(*temp_it);
		cur=*fit;
		std::cout<<"deleting:"<<*temp_it<<std::endl;
		temp_cur->erase(fit);

		it=temp_it+1;

		while(it!=end_it)
		{
			std::cout<<"deleting:"<<*it<<std::endl;
			par=cur;
			cur=*cur->find(*it);
			node_allocator.deallocate(par,1);
			it++;
		}
		node_allocator.deallocate(cur,1);		
	}

	iterator find(const key_type &key) const //make this iterator instead of bool;
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
					end_it=Key_traits::end(key);
		std::vector<cursor> cur_st;
		cursor cur,next;
		cur=root();
		while(!(it==end_it))
		{
			cur_st.push_back(cur);
			next=cur.find(*it);
			if( next == cur.end() ) return end();
			cur=next;
			it++;
		}
		cur_st.push_back(cur);
		if(cur->has_value())
		{
			return iterator(cur_st.begin(),cur_st.end());
		}
		return end();
	}



	void swap(const type &other)
	{
		std::swap(other.node_root,node_root);
	}
	
	void clear()
	{
		typedef typename node_type::iterator node_it;
		std::stack< std::pair<node_type*,node_it> > node_stack; 
		int size=1;
		node_stack.push(std::make_pair(node_root,node_root->begin()));
		while(1)
		{		
			if(node_stack.top().first->end()==node_stack.top().second)
			{
				if(size==1) break;
				node_allocator.destroy(node_stack.top().first);
				node_allocator.deallocate(node_stack.top().first,1);
				node_stack.pop();
				size--;
				node_stack.top().second++;
				continue;
			}
			node_stack.push( std::make_pair(*(node_stack.top().second)
			,(*(node_stack.top().second))->begin()) );
			size++;
		}
		node_stack.pop();
		node_allocator.destroy(node_root);
		node_allocator.deallocate(node_root,1);
		node_root=node_allocator.allocate(1);
		new(node_root) node_type();
	}

	iterator begin() const
	{
		return iterator(root(),false);
	}

	iterator end() const
	{
		return iterator(root(),true);
	}

	cursor root() const
	{
		return cursor(node_root);
	}
	
	~trie()
	{
		std::cout<<"trie class destructor"<<std::endl;
		this->clear();
		node_allocator.deallocate(node_root,1);
	}
	private:

	//get reference to the leaf node of the key
	node_type *insert_(const key_type &key)
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
			end_it=Key_traits::end(key);
		node_type *cur=node_root,*next;
		typename node_type::iterator fit;
		int i=0;
		while(it!=end_it)
		{
			fit=cur->find(*it);
			if(fit==cur->end())
				break;
			cur=*fit;
			it++;
			assert(i!=10);
		}
		i=0;
		while(it!=end_it)
		{
			i++;
			next=node_allocator.allocate(1);
			std::cout<<"Allocating:"<<*it<<std::endl;
			new(next) node_type();
			cur->insert(*it,next);
			cur=next;
			assert(i!=10);
			it++;
		}
		return cur;
	}
	bool exist(const key_type &key) const //make this iterator instead of bool;
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
					end_it=Key_traits::end(key);
		typename node_type::iterator fit;
		node_type *cur=node_root;
		while(!(it==end_it))
		{
			fit=cur->find(*it);
			if(fit == cur->end() ) return false;
			cur=*fit;
			it++;
		}
		if(cur->has_value())
		{
			return true;
		}
		return false;
	}
};

}
}
#endif //BOOST_DSEARCH_TRIE_HPP

