//  (C) Copyright Chintan Rao H 2008. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DSEARCH_PATRICIA_HPP
#define BOOST_DSEARCH_PATRICIA_HPP

#include<algorithm>
#include<memory>
#include<iostream>
#include<assert.h>
#include<vector>
#include<iterator>
#include<boost/iterator/iterator_traits.hpp>
#include<boost/dsearch/details/patricia_iterator.hpp>
#include<boost/dsearch/details/pat_key_traits.hpp>


//note //* has been used where there is an inline comment documenting the code.


#if 0
//* when FORWARD is defined, the class uses sequential access for the key.
#define FORWARD
#endif 

namespace boost{
namespace dsearch{

/// The patricia class
/** \ingroup group_nothing
    \param Key key currently accepts a string only.
    \param Mapped can be any datatype which is supposed to be indexed using string
    \warning Test waring
    \todo 
    replace all key assignement operations with key_copy.\n
    remove key.size() calculation where not needed.

*/
template<class Key,class Mapped,class Key_traits,class Alloc=std::allocator<char> >
class patricia{
	private:

	/// Self type.
	typedef patricia<Key,Mapped,Key_traits,Alloc> self;
	/// key_iterator to iterator over elements of the key.
	typedef typename Key_traits::const_iterator key_iterator;
	/// key_element_type is the type of each element.
	typedef typename Key_traits::element_type key_element_type;

	/**
	 \todo replace this enum.
	 */
	enum{
		///Maximum number of bits in a element_type. ex 8 bits for a char type.
		bit_width=8*sizeof(typename Key_traits::element_type)
	};

	public:
	///key_type is the Key which "indexs" the patricia
	typedef Key key_type;
	///data_type is the data or Mapped which the key points to
	typedef Mapped data_type;
	///In every node, std::pair<const key_type,data_type> is stored, similar to a map.
	/// This is the value returned when iterator is dereferenced
	typedef std::pair<const key_type,data_type> value_type;

	private:
	///iterator category used to over load functions when key iterator is random access iterator
	typedef typename std::iterator_traits<typename Key_traits::const_iterator>::iterator_category iterator_cat;
	///shorter name for random access iterator
	typedef typename std::random_access_iterator_tag rand_tag;

	/// represents node which patricia uses
	class patricia_node
	{
		/// Self type
		typedef patricia_node self;
		public:
		/// pointer to parent
		self *par;
		/// stores key_type and data_type pair
		value_type value;
		/// index of the bit which is to matched to search
		/// trick here is that when index%(bit_width+1) == 0, it signifies the end of a string.
		std::size_t index;
		/// left child. When bit pointed to by index is 0 go left
		self *left;
		/// right child. When bit pointed to by index is 1 go left
		self *right;

		/// Default constructor
		patricia_node(): par(0),  index(0), left(0), right(0)
		{
		}

		/// Used while creating a new node.
		patricia_node(const key_type& key_,const std::size_t &index_,patricia_node *left_
		,patricia_node *right_,patricia_node *par_) 
		:par(par_),value(std::make_pair(key_,data_type())),index(index_),left(left_),right(right_)
		{
		}

		/// Copy one patricia node to other. but pointers are not copied. they are set to zero
		patricia_node(const patricia_node &other)
		:par(0),value(other.value),index(other.index),left(0),right(0)
		{
		}

		/// Never used. but should accidentally be not used thats why assert(false) :)
		bool operator =(const patricia_node &other)
		{
			assert(false);
		}
	};

	///patricia_node allocator type
	typedef typename Alloc::template rebind<patricia_node>::other node_allocator_type;
	///node allocator object
	node_allocator_type node_allocator;
	///root node
	patricia_node *root;

	public:
	
	/// Bidirectional iterator which when dereferenced returns value_type.
	typedef patricia_iterator<value_type, patricia_node, Alloc> iterator;
	/// Bidirectional const_iterator which when dereferenced returns const value_type.
	typedef patricia_iterator<const value_type, const patricia_node, Alloc> const_iterator;
	/// Bidirectional reverse_iterator which when dereferenced returns value_type.
	typedef patricia_reverse_iterator<value_type, patricia_node, Alloc> reverse_iterator;
	/// Bidirectional const_reverse_iterator which when dereferenced returns const value_type.
	typedef patricia_reverse_iterator<const value_type, const patricia_node, Alloc> const_reverse_iterator;


	///patricia's allocator type.
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
	typedef typename allocator_type::difference_type      difference_type; 
    
    /// Default constructor.
    /** the default constructor
    */
	patricia(): node_allocator(node_allocator_type()),root(0)
	{
	}

	/// Copy constructor
	/** creates a copy of the patricia object used as parameter
	*/
	patricia(const self &other)
	{
		copy_patricia(other.root);
	}

	/// operator [ ]  works similar to std::map
	/** \returns returns a reference to the object that is associated with a particular key. 
		If the map does not already contain such an object, operator[] inserts the default object data_type()	 
    	\param k corresponding to the data_type's reference to retrieved.
	*/
	data_type &operator [ ] (const key_type &k)
	{
		#ifndef FORWARD
		//call insert node with insert node category
			return insert_new_node(k,iterator_cat());
		#else
			return insert_new_node(k,0);
		#endif
	}

	/// Assignement operator
	/** \returns reference to patricia.
    	\param other: patricia to be copied
    	
	*/
	self operator = (const self& other) 
	{
		this->clear();
		copy_patricia(other.root);
		return *this;
	}

	/// insert pair<key,mapped> into the patricia
	/** \returns returns a reference to the object that is associated with a particular key. 
		If the map does not already contain such an object, operator[] inserts the default object data_type()	 
    	\param v: value_type, that is std::pair<key_type,data_type>, to be inserted
	*/
	void insert(const value_type &v)
	{
		this->operator [](v.first)=v.second;
	}

	/// returns a bidirectional iterator pointing to the begin of the patricia
	/** \returns patricia::iterator
	*/
	iterator begin()
	{
		return iterator ( root, true );
	}

	/// returns a bidirectional iterator pointing to the after end of the patricia
	/** \returns patricia::iterator
	*/
	iterator end()
	{
		return iterator ( root, false );
	}

	/// returns a bidirectional const_iterator pointing to the begin of the patricia
	/** \returns  patricia::const_iterator
	*/
	const_iterator begin() const
	{
		return const_iterator ( root, true );	
	}

	/// returns a bidirectional const_iterator pointing to the after end of the patricia
	/** \returns patricia::const_iterator
	*/
	const_iterator end() const
	{
		return const_iterator ( root, false );
	}

	/// returns a bidirectional reverse iterator pointing to the end of the patricia
	/** \returns patricia::reverse_iterator
	*/
	reverse_iterator rbegin()
	{
		return reverse_iterator(root,true);
	}

	/// returns a bidirectional reverse iterator pointing to the before begin of the patricia
	/** \returns patricia::reverse_iterator
	*/
	reverse_iterator rend()
	{
		return reverse_iterator(root,false);
	}

	/// returns a bidirectional const reverse iterator pointing to the end of the patricia
	/** \returns patricia::const_reverse_iterator
	*/
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(root,true);
	}

	/// returns a bidirectional const reverse iterator pointing to the before begin of the patricia
	/** \returns patricia::const_reverse_iterator
	*/
	const_reverse_iterator rend() const 
	{
		return const_reverse_iterator(root,false);
	}

	/// finds whether a key exists in the map
	/** \returns true if key exists false if does not exist
	   \param k is the key to be found
	   \warning this exists for debugging puposes only
	*/
	bool exists(const key_type &k)
	{
		std::pair<std::size_t,int> common;
		patricia_node *node;
		#ifndef FORWARD
			//*call find_node with iterator_cat()
			node=find_node(root,k,iterator_cat());
		#else
			node=find_node(root,k,0);
		#endif
		if(node==0) return 0;
		
		//*get the common bits . equivalent to key compare.
		common=get_common_bits(node->value.first,k);
		//*return true is the keys are equal.
		return (common.second==2);
	}

	/// finds iterator pointing to the key
	/** \returns patricia::iterator
	    \param k is the key to be found
	*/
	iterator find(const key_type &k)
	{
		std::pair<patricia_node*,patricia_node*> node_pair;
		#ifndef FORWARD
			//*call find_node_prev with iterator_cat()
			node_pair=find_node_prev(root,k,iterator_cat());
		#else
			node_pair=find_node_prev(root,k,0);
		#endif
		
		if ( node_pair.first==0) return end();
		//*return end() if the found keys are not equal.
		if(get_common_bits(node_pair.first->value.first,k).second!=2)
			return end();
		//*construct the iterator and then return it.
		return iterator(node_pair.first,node_pair.second);
	}

	/// finds const_iterator pointing to key.
	/** \returns patricia::const_iterator
	    \param k is the key to be found
	*/
	const_iterator find(const key_type &k) const
	{
		return const_cast<self *>(this)->find(k);
	}

	/// finds the smallest value greater that equal to the given key
	/** \returns patricia::iterator
	*/
	iterator upper_bound(const key_type &k)
	{
		#ifndef FORWARD
			//*call upper bound with iterator category.
			return upper_bound(k,iterator_cat());
		#else
			return upper_bound(k,0);
		#endif
	}

	/// finds the smallest value greater that equal to the given key
	/** \returns patricia::const_iterator
	*/
	const_iterator upper_bound(const key_type &k) const
	{
		return const_cast<self *>(this)->upper_bound(k);
	}

	/// finds the largest value lesser that equal to the given key
	/** \returns patricia::iterator
	*/
	iterator lower_bound(const key_type &k) 
	{
		#ifndef FORWARD
			//*call upper bound with iterator_cat();
			return lower_bound(k,iterator_cat());
		#else
			return lower_bound(k,0);
		#endif
	}

	/// finds the largest value lesser that equal to the given key
	/** \returns patricia::const_iterator
	*/
	const_iterator lower_bound(const key_type &k)  const 
	{
		return const_cast<self *>(this)->lower_bound(k);
	}

	/// finds the range of iterators the patricia is prefix of.
	/** \returns std::pair<iterator,iterator> 
	    \param k is prefix to be found
	    \todo add mask parameter to allow search of prefix bitwise
	*/
	std::pair<iterator,iterator> prefix_range(const key_type &k)
	{
		#ifndef FORWARD
			//*call prefix_range with iterator category
			return prefix_range(k,iterator_cat());
		#else
			return prefix_range(k,0);
		#endif
	}

	/// finds the range of const iterators the patricia is prefix of.
	/** \returns std::pair<const_iterator,const_iterator> 
	    \param k is prefix to be found
	*/
	std::pair<const_iterator,const_iterator> prefix_range(const key_type &k) const
	{
		return const_cast<self*>(this)->prefix_range(k);
	}

	/// finds the range of iterators the patricia is prefix of.
	/** \returns std::pair<iterator,iterator> 
	    \param k is prefix to be found
	*/
	void erase(const key_type &k)
	{
		#ifndef FORWARD
			erase(k,iterator_cat());
		#else
			erase(k,0);
		#endif
	}

	/// erase a particular value pointer to by the iterator
	/** \returns void
	    \param it: iterator to be erased
	*/
	void erase(const iterator &it)
	{
		erase(it.next,it.cur);
	}

	/// checks whether the patricia is empty or not.
	/** \returns true if empty and false if not empty.
	*/
	bool empty() const
	{
		return root==0;
	}

	/// clears the patricia
	/** \returns void
	*/
	void clear()
	{
		pat_clear();
	}
	
	/// to find the no of values hashed into the array
	/** \returns void
	*/
	std::size_t size() const 
	{
		return pat_size();
	}

	/// class destructor
	~patricia()
	{
		this->clear();
	}

	private:

	#include<boost/dsearch/details/patricia_details.hpp>

};

	///check the equality of the two patricia objects
	/**
	\returns true is equal false otherwise
	\param p1 is the first patricia node
	\param p2 is the second patricia node
	\todo check whether are actually better ways.
	*/

template<class Key,class Mapped,class Key_traits,class Alloc >
bool operator == (const patricia<Key,Mapped,Key_traits,Alloc> & p1, 
	const patricia<Key,Mapped,Key_traits,Alloc>& p2)
{
	typedef typename patricia<Key,Mapped,Key_traits,Alloc>::const_iterator const_iterator;
	const_iterator it_1, it_2, end_it_1, end_it_2;
	
	it_1=p1.begin();
	it_2=p2.begin();
	std::cout<<"Hello in checking equal"<<std::endl;

	//* simply check whether all keys and values are equal when enumerated 
	//* in ascending order
	while ( it_1 != p1.end() && it_2 != p2.end() 
	&& (*it_1).first == (*it_2).first &&  (*it_1).second == (*it_2).second )
	{
		it_1++;
		it_2++;
	}

	if( it_1 == p1.end() && it_2 == p2.end() )
		return true;
	return false;
}

}//namespace dsearch
}//namespace boost

#endif //BOOST_DSEARCH_PATRICIA_HPP
