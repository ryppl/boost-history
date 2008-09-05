//  (C) Copyright Chintan Rao H 2008. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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

//TODO size(), ==,erase (iterator,iterator)
///The trie class.
/**
 	\param Key is the trie's key type.
 	\param Mapped is the trie's data type.
 	\param trie_node is the node type to be used.
 	\param Key_traits is the traits of the key.
 	\param Alloc is the allocator to be used for allocation.
 */ 
template<class Key,class Mapped,
template<class K,class M,class K_t,class A > class trie_node, 
class Key_traits,
class Alloc=std::allocator<char> > 
class trie
{
	private:
	/// node_type of every ndoe in the trie.
	typedef trie_node<Key,Mapped,Key_traits,Alloc> node_type;
	/// the type of allocator for the node
	typedef typename Alloc::template rebind<node_type>::other node_allocator_type;
	/// node_allocator object for the node.
	node_allocator_type node_allocator;
	/// root of the patricithe trie.
	node_type *node_root;
	public:
	/// type of the key used by the trie
	typedef Key key_type;
	/// type which key maps to
	typedef Mapped data_type;
	/// value type which can be used to insert. Note, unlike map,  this is not the type
	/// of a deferenced iterator
	typedef std::pair<Key,Mapped> value_type;
	/// self type
	typedef trie<Key,Mapped,trie_node,Key_traits,Alloc> type;

	///trie's allocator type.
	typedef typename Alloc::template rebind<value_type>::other allocator_type;
	///pointer to value_type.
    typedef typename allocator_type::pointer pointer;
    ///const pointer to value_type.
    typedef typename allocator_type::const_pointer        const_pointer;
    ///reference to value_type.
	typedef typename allocator_type::reference            reference;
	///const reference to value_type.
	typedef typename allocator_type::const_reference      const_reference;
	///return type of size()
	typedef typename std::size_t           size_type;
	///represents distance between two iterators.
	typedef typename allocator_type::difference_type      difference_type; //should actually depend on iterator(.|?)
	
	///trie's cursor which represents the trie node.
	typedef trie_cursor<key_type,data_type,node_type> cursor;
	///trie's const_cursor which represents the const trie node.
	typedef trie_cursor<key_type,data_type,const node_type> const_cursor;
	///iterator used to iterate through a map. dereferencing this gives data_type.
	typedef trie_iterator<key_type,data_type,cursor> iterator;
	///const_iterator used to iterate through a map. dereferencing this gives const data_type.
	typedef trie_iterator<key_type,const data_type,const_cursor> const_iterator;
	///reverse_iterator used to iterate through a map. dereferencing this gives data_type.
	typedef trie_reverse_iterator<key_type,data_type,cursor> reverse_iterator;
	///const_reverse_iterator used to iterate through a map. dereferencing this gives const data_type.
	typedef trie_reverse_iterator<key_type,const data_type,const_cursor> const_reverse_iterator;

	///default contructor
	trie()
	{
		node_root=node_allocator.allocate(1);
		new(node_root) node_type();
	}

	///copy constructor
	/**
	 \param other is the trie to be copied
	 */
	trie ( const type &other )
	{
		copy_trie_preserve(const_cast<node_type *>(other.node_root) );
	}
	
	///Assignment operator
	/**
	 \returns reference to self.
	 \param other is the trie to be copied
	 */
	type &operator = ( const type &other )
	{
		if(node_root==other.node_root) return *this;

		clear();
		node_allocator.destroy(node_root);
		node_allocator.deallocate(node_root,1);
		copy_trie_preserve(other.node_root);
		return *this;
	}

	///works std::maps's [ ] operator
	/**
	 \returns reference to data_type correspoding to the key.
	 \param k is the key whose data type is to be found.
	 */
	data_type &operator [] (const key_type &k) 
	{
		return	insert_(k)->get_value_ref();
	}

	///insert a value_type
	/**
	 \param v is std::pair<key_type,data_type> to be inserted.
	 */
	void insert(const value_type &v)
	{
		this->operator[](v.first)=v.second;
	}

	///determines the size of the trie
	/**
	 \returns the size of the trie
	 */
	std::size_t size() const
	{
		return const_cast<type *>(this)->size_();
	}

	public:

	///maximum size of the trie
	/**
	 \returns maximum possible size of the trie. Thats is maximum value of an integer.
	 */
	std::size_t max_size()
	{
		return  std::numeric_limits<std::size_t>::max();
	}

	///determines whether the trie is empty or not.
	/**
	 \returns true if it is empty and false otherwise.
	 */
	bool empty()
	{
		return node_root->empty();
	}

	///finds the key in the trie
	/**
	 \returns iterator to the found key. If not found returns end().
	 \param key is the key for which iterator is to be found
	 */
	iterator find(const key_type &key)
	{
		return find< iterator, cursor, type* > (this,key);
	}

	///finds the key in the trie
	/**
	 \returns const_iterator to the found key. If not found returns end().
	 \param key is the key for which const iterator is to be found
	 */
	const_iterator find(const key_type &key) const
	{
		return const_cast<type *>(this)->find(key);
		//return find< const_iterator, const_cursor, const type* > (this,key);
	}

	///finds the upper bound of a particular 
	/**
	 \returns iterator to the upper bound. If not found returns end().
	 \param key is the key for which upper bound is to be found
	 */
	const_iterator upper_bound(const key_type &key) const
	{
		return const_cast<type *>(this)->upper_bound(key);
		//return upper_bound<const_iterator,const_cursor,const type*>(this,key);
	}

	///finds the upper bound of a particular 
	/**
	 \returns const_iterator to the upper bound. If not found returns end().
	 \param key is the key for which upper bound is to be found
	 */
	iterator upper_bound(const key_type &key)
	{
		return upper_bound<iterator,cursor,type*>(this,key);
	}

	///finds the lower bound of a particular 
	/**
	 \returns iterator to the lower bound. If not found returns end().
	 \param key is the key for which lower bound is to be found
	 */
	iterator lower_bound(const key_type &key)
	{
		return lower_bound<iterator,cursor,type*>(this,key);
	}

	///finds the lower bound of a particular 
	/**
	 \returns const_iterator to the lower bound. If not found returns end().
	 \param key is the key for which lower bound is to be found
	 */
	const_iterator lower_bound(const key_type &key) const
	{
		//return lower_bound<const_iterator,const_cursor,const type*>(this,key);
		return const_cast<type *>(this)->lower_bound(key);
	}
	
	//not very clean but works
	///find the key corresonding the iterator
	/**
	 \returns key_type corresponding to the iterator.
	 \param cit const_iterator
	 */
	key_type get_key(const const_iterator &cit) const
	{
		typedef typename std::vector<const_cursor>::const_iterator cur_const_iterator;
		std::vector<typename Key_traits::element_type> e_vector;
		Key ret_key;
		///iterate through all elements in the vector and then collect the elements
		for(cur_const_iterator it=++cit.cur_st.begin();it!=cit.cur_st.end();it++)
			e_vector.push_back((*it).get_element());
		return Key_traits::get_key(e_vector.begin(),e_vector.end());
	}

	//not very clean but works
	///find the key corresonding the reverse iterator
	/**
	 \returns key_type corresponding to the const_reverse_iterator.
	 \param crit const_reverse_iterator
	 */
	Key get_key(const const_reverse_iterator &crit) const
	{
		return get_key(crit.get_iterator());
	}

	///erase the key from the trie
	/**
		\param key to be erase.
	*/
	void erase(const key_type &key) 
	{
		iterator it;
		it=find(key);
		if(it!=end())
			erase(it);
/*		typename Key_traits::const_iterator it=Key_traits::begin(key),
			end_it=Key_traits::end(key),temp_it=it;
		typename node_type::iterator fit;



		node_type* cur=node_root,*par=node_root,*temp_cur=node_root;

		if(it==end_it) 
		{
			if(node_root->has_value())
				node_root->erase_value();
			return;
		}

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
	//	std::cout<<"deleting:"<<*temp_it<<std::endl;
		temp_cur->erase(fit);

		it=temp_it;
		++it;

		while(it!=end_it)
		{
	//		std::cout<<"deleting:"<<*it<<std::endl;
			par=cur;
			cur=*cur->find(*it);
			node_allocator.deallocate(par,1);
			it++;
		}
		node_allocator.deallocate(cur,1);*/	
	}

	///range of iterators with patricular prefix
	/**
	 \returns iterator range as (const_iterator,const_iterator)
	 \param key specifes the prefix.
	 */ 
	std::pair<const_iterator,const_iterator> prefix_range(const key_type &key) const
	{
		return const_cast<type *>(this)->prefix_range(key);
		//return prefix_range<const_iterator,const_cursor,const type*>(this,key);
	}

	///range of iterators with patricular prefix
	/**
	 \returns iterator range as (const_iterator,const_iterator)
	 \param key specifes the prefix.
	 */ 	
	std::pair<iterator,iterator> prefix_range(const key_type &key)
	{
		return prefix_range<iterator,cursor,type*>(this,key);
#if 0
		typename Key_traits::const_iterator it=Key_traits::begin(key),
				end_it=Key_traits::end(key);
		iterator ret_it,right;
		cursor cur,next;
		cur=root();
		ret_it.push(cur);
		//* find where the leaf is.
		while(it!=end_it)
		{
			if(cur.empty()) return std::make_pair(end(),end());
			next=cur.find(*it);
			if(cur.end()==next)
				return std::make_pair(end(),end());
			cur=next;
			ret_it.push(cur);
			it++;
		}
		
		right=ret_it;
		//* and left most from the leaf.
		ret_it.to_left_most();
		//* move right most from the leaf.
		right.to_right_most();
		//* since we want right range not to be closed we increment right
		//* before returning.
		return std::make_pair(ret_it,++right);
#endif

	}

	///clears the trie. 
	void clear()
	{
		typedef typename node_type::iterator node_it;
		//* store the node and node iterator in a stack.
		std::stack< std::pair<node_type*,node_it> > node_stack; 
		int size=1;
		
		//* push root and its begin()
		node_stack.push(std::make_pair(node_root,node_root->begin()));
		while(1)
		{
			//* if the iterator has reached the end we will need to pop
			//* and goto to the next sub tree.
			if(node_stack.top().first->end()==node_stack.top().second)
			{
				//* if size of 1 we have done every thing :)
				if(size==1) break;
				node_allocator.destroy(node_stack.top().first);
				node_allocator.deallocate(node_stack.top().first,1);
				//* pop the current sub tree.
				node_stack.pop();
				size--;
				//* move to the next subtree.
				node_stack.top().second++;
				continue;
			}
			//* arrange for a new node.
			node_stack.push( std::make_pair(*(node_stack.top().second)
			,(*(node_stack.top().second))->begin()) );
			size++;
		}
		node_stack.pop();
		//* we have not yet deallocated root.
		//* clean up root.
		node_allocator.destroy(node_root);
		node_allocator.deallocate(node_root,1);
		node_root=node_allocator.allocate(1);
		
		//* and allocate a new one.
		new(node_root) node_type();
	}
	
	/**
	\param it is the iterator to be erased
	\note linear iteration to find where the pointer is grr...
	otherwise i will need to add one more function to the node
	*/
	void erase(const iterator &it)
	{
		iterator iter=it;
		cursor cur_it;
		typename node_type::iterator node_it;
		
		//* check whether the iterator points to a leaf node
		if ( iter.top().begin()==iter.top().end() )
		{
			//* iter.size() refers to size of the iterator's stack.
			//* since there is no parent for the root node it does not 
			//* have a corresponding iterator.
			if(iter.size()!=1)
				node_it=iter.top().get_iterator();
			
			node_allocator.destroy( iter.top().get_node() );
			node_allocator.deallocate( iter.top().get_node() , 1 );
			iter.pop();
		}
		else
		{
			//*if it does not point to a leaf node just erase the data and return
			iter.top().get_node()->erase_value();
			return;
		}

		if ( iter.empty() )//*deallocated the root node so reallocate it.
		{
			node_root=node_allocator.allocate(1);
			new(node_root) node_type();
			return;
		}
			
		cur_it=iter.top().begin();
		//*if (++cur_it)==iter.top().end() then its only child has been erased.
		while( (++cur_it)==iter.top().end() )
		{
			//* iter.size() refers to size of the iterator's stack.
			//* since there is no parent for the root node it does not 
			//* have a corresponding iterator.
			if(iter.size()!=1)
				node_it=iter.top().get_iterator();
			//* deallocate the node.
			node_allocator.destroy ( iter.top().get_node() );
			node_allocator.deallocate ( iter.top().get_node() , 1 );
			iter.pop();
			
			//* deallocated the root node so reallocate it.
			if( iter.empty() ) 
			{
				node_root=node_allocator.allocate(1);
				new(node_root) node_type();
				return;
			}

			//* if a particular node has value then break then and there.
			if(iter.top().has_value())
				break;
			cur_it=iter.top().begin();
		}
		/*node_it=iter.top().get_node()->begin();

		//here is the linear iteration
		while( (*node_it)!=n_t )
		{
			++node_it;
			assert(iter.top().get_node()->end()!=node_it);
		}*/
		//assert( (*node_it) == n_t );
		iter.top().get_node()->erase(node_it);
	}

	///swaps two tries
	/**
	\param other is the other trie to be swapped with
	*/
	void swap(type &other)
	{
		std::swap(other.node_root,node_root);
	}

	///Returns an iterator pointing to the beginning of the trie.
	/**
	 \returns an iterator pointing to the beginning of the trie.
	*/
	iterator begin()
	{
		return iterator(root(),false);
	}

	///Returns a const iterator pointing to the beginning of the trie.
	/**
	 \returns an const_iterator pointing to the beginning of the trie.
	*/
	const_iterator begin() const
	{
		return const_cast<type *>(this)->begin();
		//return const_iterator(root(),false);
	}

	///Returns an iterator pointing to the end of the hash_multiset.
	/**
	 \returns an iterator pointing to the beginning of the trie.
	 */
	iterator end()
	{
		return iterator(root(),true);
	}

	///Returns an const iterator pointing to the end of the hash_multiset.
	/**
	 \returns a const_iterator pointing to the beginning of the trie.
	 */
	const_iterator end() const 
	{
		return const_cast<type *>(this)->end();
		//return const_iterator(root(),true);
	}

	/// Returns a reverse iterator pointing to the beginning of the reversed trie
	/**
	  \returns a reverse_iterator pointing to the beginning of the reversed trie
	 */
	reverse_iterator rbegin()
	{
		return reverse_iterator(begin(),end(),true);
	}

	/// Returns a const reverse iterator pointing to the beginning of the reversed trie
	/**
	  \returns a const_reverse_iterator pointing to the beginning of the reversed trie
	 */
	const_reverse_iterator rbegin() const
	{
		return const_cast<type *>(this)->end();
		//return const_reverse_iterator(begin(),end(),true);
	}

	/// Returns a reverse iterator pointing to the end of the reversed trie
	/**
	  \returns a reverse_iterator pointing to the end of the reversed trie
	 */
	reverse_iterator rend()
	{
		return reverse_iterator(begin(),end(),false);
	}

	/// Returns a const reverse iterator pointing to the end of the reversed trie
	/**
	  \returns a const_reverse_iterator pointing to the end of the reversed trie
	 */
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin(),end(),false);
	}

	///Returns a cursor pointing to the root.
	/**
	  \returns cursor pointing to the root.
	 */	
	cursor root()
	{
		return cursor(node_root);
	}

	///Returns a const cursor pointing to the root.
	/**
	 \returns const_cursor pointing to the root.
	 */
	const_cursor root() const
	{
		return const_cursor(node_root);
	}

	///destructor
	~trie()
	{
		//std::cout<<"trie class destructor"<<std::endl;
		this->clear();
		node_allocator.deallocate(node_root,1);
	}

	private:
	///determines the size of the trie
	/**
	 \returns the size of the trie.
	 */
	inline std::size_t size_()
	{
		int num=0;
		iterator it,end_it=this->end();
		//* just iterate and find out no of elements.
		for(it=this->begin();it!=end_it;it++,num++);
		
		return num;
	}
	
	///get reference to the leaf node of the key
	/**
	 \returns pointer to the leaf node of the key
	 \param key is the whose leaf is suppose to be found
	 */
	node_type *insert_(const key_type &key)
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
			end_it=Key_traits::end(key);
		node_type *cur=node_root, *next;
		typename node_type::iterator fit;
		
		//*go as far as one can.
		while ( it!=end_it )
		{
			//*find the current char in the node.
			fit=cur->find( *it );
			//*if not found go out of the loop.
			if ( fit == cur->end() )
				break;
			//*change cur to pointer of the new node.
			cur=*fit;
			it++;

		}
		
		//*allocate nodes untill the end of the string.
		while(it!=end_it)
		{
			next=node_allocator.allocate(1);
			//std::cout<<"Allocating:"<<*it<<std::endl;
			new(next) node_type();
			//*insert the char in to new node along with pointer to next node.
			cur->insert(*it,next);
			cur=next;
			it++;
		}
		//*return the pointer to the leaf node.
		return cur;
	}

	///checks whether a key exists or not
	/**
	 \returns true if it exists other wise false
	 \param key is the key whose existance in the trie us supposed to be
	 determined
	 */
	bool exist(const key_type &key) const
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
					end_it=Key_traits::end(key);
		typename node_type::iterator fit;
		node_type *cur=node_root;
		
		//* for each element in the key find the corresponding node pointer
		//* and move the the node.
		while(!(it==end_it))
		{
			fit=cur->find(*it);
			if ( fit == cur->end() ) return false;
			cur=*fit;
			it++;
		}
		//* if the "leaf" has the a value return true.
		if(cur->has_value())
			return true;
		//* else
		return false;
	}
	///copy the trie preserving the internal structure of the node
	/**
	 \param other_root is the root node of the other trie to be copied.
	 */
	inline void copy_trie_preserve(node_type*other_root)
	{
		node_type *prev,*temp_node;
		typedef typename node_type::iterator node_iterator;
		//* stack to maintain the list of iterators for the other root.
		std::stack<node_iterator> st;
		//* stack to maintain the list of iterators.
		std::stack<node_iterator> this_st;
		//* maintain iterators for other root.
		node_iterator cur_it=other_root->begin();

		//* allocate the root node
		node_root=node_allocator.allocate(1);
		node_allocator.construct(node_root,*other_root);
		
		//* if the other root is empty return
		if ( other_root->end() == other_root->begin() )
			return;
			
		//* push root on to the stack
		this_st.push(node_root->begin());

		while(true)
		{
			if(st.empty())
				prev=other_root;
			else
				prev=*st.top();

			if( cur_it == prev->end() )
			{
				if(st.empty())  return;
				
				//* change cur_it
				cur_it=++st.top();
				//* pop to change top.
				st.pop();
				
				//* do the same (nearly) to other stack too.
				this_st.pop();
				++this_st.top();
				continue;
			}

			//* assign new node.
			temp_node=node_allocator.allocate(1);

			//* call the copy constructor of the new node.
			//* this helps in preserving the internal node structure.
			node_allocator.construct( temp_node , **cur_it );
			
			//* put the pointer of new node on the parent outward edge
			*(this_st.top())=temp_node;

			//* push the begin of new node on to the stack.
			this_st.push(temp_node->begin());
			
			//* push the cur_it on to the stack and move move one to new it.
			st.push(cur_it);
			
			cur_it=(*cur_it)->begin();
		}
	}

	///find the key in the trie
	/**
	 	\brief *was* used to avid writing two functions one for const find 
	 	and other non const find.
	 	\param key is the key to be found
	 	\param this_ptr is the pointer to this trie
	 */
	template<class It_type,class Cur_type,class Trie_ptr>
	static inline It_type find(Trie_ptr this_ptr,const key_type &key) 
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
				end_it=Key_traits::end(key);
				
		//* iterator is a stack of cursors
		It_type ret_it;
		Cur_type cur,next;
		cur=this_ptr->root();
		//* goto the leaf of the key.
		while(!(it==end_it))
		{
			//* push the current iterator on to the stack.
			ret_it.push(cur);
			//* find the particular charecter in the node.
			next=cur.find(*it);
			//* if not found return null.
			if ( next == cur.end() ) 
				return this_ptr->end();
			cur=next;
			it++;
		}
		
		ret_it.push(cur);
		//* if leaf has value return ret_it.
		if(cur.has_value())
		{
			return ret_it;
		}
		return this_ptr->end();
	}

	///find the lower bound of an element in the node
	/**
	 	\param node to be searched
	 	\param ele is the element whose lower bound is to be found
	 */
	static inline typename node_type::iterator lower_bound(node_type *const &node,const typename Key_traits::element_type & ele)
	{
		typename node_type::iterator it=node->begin(),eit=node->end();
		typename node_type::iterator ret_it=eit;
		
		//* linear iteration to find where the lower bound is.
		while(it!=eit)
		{
			if( node->get_element(it) > ele)
				return ret_it;
			ret_it=it;
			it++;
		}
		return ret_it;
	}
	
	///find the upper bound of the key
	/**
	 	\brief *was* used to avid writing two functions for upperbound
	 	and other non const find.
	 	\param key is the key to be found
	 	\param this_ptr is the pointer to this trie
	 */
	template<class It_type,class Cur_type,class Trie_ptr>
	static inline It_type upper_bound(Trie_ptr this_ptr,const key_type &key)
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
					end_it=Key_traits::end(key);
		It_type ret_it;
		bool not_found=false;
		Cur_type cur,next,r=this_ptr->root();
		cur=r;
		ret_it.push(cur);
		//std::cout<<"CONST UPPER BOUND"<<std::endl;
		
		//go as far as possible with the key.
		while(it!=end_it)
		{
			if(cur.empty())
			{
				//* if the cur is empty then prefix of the key is there in the trie
				//* so goto the iterator adjacent to prefix
				ret_it++;
				return ret_it;
			}
			//* the usual find procedure.
			next=cur.find(*it);
			if ( next == cur.end() )
			{
				not_found=true;
				break;
			}
			//std::cout<<"found "<<*it<<std::endl;
			cur=next;
			ret_it.push(cur);
			it++;				
		}
		
		//* if the entire key is not found
		if( not_found )
		{
			//* if find the lower bound in the last node we could atmost reach.
			next=Cur_type(cur.get_node(),lower_bound(cur.get_node(),*it));
			//next=Cur_type(cur.get_node(),const_cast<node_type *>(cur.get_node())->lower_bound(*it));
			
			if(next==ret_it.top().end())
			{
				//* if there is no lower bound we position the cursor at begin.
				next=Cur_type ( cur.get_node(), cur.get_node()->begin());
			}
			else
			{
				//* if else we just need increment the cursor.
				//* because the upper bound is adjacent to the lower bound.
				next++;
			}
			
			//* next++ could have positoned the cursor at the end.
			//* so pop untill next++ does not position at the end.
			while(next==ret_it.top().end())
			{
				//std::cout<<"popping "<<std::endl;
				next=ret_it.top();
				if(next==r)
					return this_ptr->end();
				ret_it.pop();
				next++;
			}
			ret_it.push(next);
		}
		//* move the iterator to the left most from the current position.
		//* if ret_it.top().has_value() then this does nothing
		ret_it.to_left_most(); 
		return ret_it;
	}

	///find the lower bound of the key
	/**
	 	\brief *was* used to avoid writing two functions for lower bound
	 	and other non const find.
	 	\param key is the key to be found
	 	\param this_ptr is the pointer to this trie
	 */
	template<class It_type,class Cur_type,class Trie_ptr>
	static inline It_type lower_bound(Trie_ptr this_ptr,const key_type &key)
	{

		typename Key_traits::const_iterator it=Key_traits::begin(key),
				end_it=Key_traits::end(key);
		It_type ret_it;
		bool not_found=false;
		Cur_type cur,next,r=this_ptr->root();
		cur=r;
		ret_it.push(cur);
		//std::cout<<"LOWER BOUND"<<std::endl;
		//* try to find the key in the trie
		while(it!=end_it)
		{
			if(cur.empty())
				break;
			//std::cout<<"finding "<<*it<<std::endl;
			//* find the next cursor
			next=cur.find(*it);
			if ( next == cur.end() )
			{
				not_found=true;
				break;
			}
			//std::cout<<"found "<<*it<<std::endl;
			cur=next;
			ret_it.push(cur);
			it++;
		}

		//* if the entire key is not found 
		if(not_found)
		{
			//* then find the lower bound of the element which is not found the node.
			//* then make  cursor out of it.
			next=Cur_type(cur.get_node(),lower_bound(cur.get_node(),*it));
			//next=Cur_type(cur.get_node(),const_cast<node_type *>(cur.get_node())->lower_bound(*it));
			
			//* if lower bound is found we have no problems :).
			if(next!=cur.end()) 
			{
				ret_it.push(next);
				//* move the right most in the subtree.
				//* ie the greatest element/
				ret_it.to_right_most();
				return ret_it;
			}
		}

		do
		{
			//* else pop until --next!= end() or next has value
			next=ret_it.top();
			ret_it.pop();
			if(next.has_value())
			{
				ret_it.push(next);
				return ret_it;
			}
			if(next==r)
			{
				return this_ptr->end();
			}
		}
		while (next==ret_it.top().begin());

		next--;
		ret_it.push(next);
		//* move to highest key in the subtree.
		ret_it.to_right_most();
		return ret_it;
	}
	

	///find the prefix_range of the key
	/**
	 	\brief *was* used to avoid writing two functions for lower bound
	 	and other non const find.
	 	\param key is the key to be found
	 	\param this_ptr is the pointer to this trie
	 */
	template<class Iter_type,class Cur_type,class Trie_t>
	static inline std::pair<Iter_type,Iter_type> prefix_range(Trie_t this_ptr,const key_type &key)
	{
		typename Key_traits::const_iterator it=Key_traits::begin(key),
				end_it=Key_traits::end(key);
		Iter_type ret_it,right;
		Cur_type cur,next;
		cur=this_ptr->root();
		ret_it.push(cur);
		//* as usual try to find the prefix in the trie.
		while(it!=end_it)
		{
			if(cur.empty()) return std::make_pair(this_ptr->end(),this_ptr->end());
			next=cur.find(*it);
			
			//* if not found return end() pairs
			if(cur.end()==next)
				return std::make_pair(this_ptr->end(),this_ptr->end());
			
			cur=next;
			ret_it.push(cur);
			it++;
		}
		right=ret_it;
		//* if prefix is found move to left most and right most.
		ret_it.to_left_most();
		//std::cout<<"To RIGHT MOST"<<std::endl;
		right.to_right_most();
	       //std::cout<<"pair range "<<*right<<std::endl;
		return std::make_pair(ret_it,++right);
	}

#if 0
	//this does not preserve the structure of tst.
	void copy_trie(node_type*other_root)
	{
		node_type *prev,*this_prev,*temp_node;
		typedef typename node_type::iterator node_iterator;
		std::stack<node_iterator> st;
		std::stack<node_type *> this_st;

		node_iterator cur_it=other_root->begin();

		node_root=node_allocator.allocate(1);
		new(node_root) node_type;
		if(other_root->has_value())
			node_root->get_value_ref()=other_root->get_value_ref();

		while(true)
		{
			if(st.empty())
			{
				prev=other_root;
				this_prev=node_root;
			}
			else
			{
				prev=*st.top();
				this_prev=this_st.top();
			}

			if(cur_it==prev->end())
			{
				std::cout<<"reached end"<<std::endl;
				if(st.empty())  return;
				this_st.pop();
				cur_it=++st.top();
				st.pop();
				continue;
			}
			else
				std::cout<<"going to:"<<prev->get_element(cur_it)<<std::endl;

			temp_node=node_allocator.allocate(1);
			new(temp_node) node_type();
			this_prev->insert(prev->get_element(cur_it),temp_node);

			if((*cur_it)->has_value())
				temp_node->get_value_ref()=(*cur_it)->get_value_ref();
			this_st.push(temp_node);
			st.push(cur_it);
			cur_it=(*cur_it)->begin();
		}
	}
#endif
};

}
}
#endif //BOOST_DSEARCH_TRIE_HPP

