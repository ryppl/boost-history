#ifndef BOOST_DSEARCH_PATRICIA_HPP
#define BOOST_DSEARCH_PATRICIA_HPP

#include<algorithm>
#include<memory>
#include<iostream>
#include<assert.h>
#include<vector>
#include<iterator>
#include<boost/iterator/iterator_traits.hpp>
#include<boost/dsearch/patricia_iterator.hpp>


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
    replace all key.size() with key_traits::size().\n
    remove key.size() calculation where not needed.

*/
template<class Key,class Mapped,class Key_traits,class Alloc=std::allocator<char> >
class patricia{
	private:

	/// Self type.
	typedef patricia<Key,Mapped,Key_traits,Alloc> self;
	/// key_iterator to iterator over elements of the key.
	typedef typename Key_traits::const_iterator key_iterator;
	/// key_element_type is he type of each element.
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
		:par(par_),value(std::make_pair(key_,0)),index(index_),left(left_),right(right_)
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
		patricia_node *cur,*next;
		if(root==0) return;
		cur=root;
		root=0;
		while(true)
		{
			//*try going left if cur->left==0 try going right
			next=cur->left?cur->left:cur->right;
			
			if(next==0)
			{
				//*if cur->right is also 0 ==> next is 0.
				//*deallocate the node
				next=cur->par;
				deallocate_node(cur);
				if(next==0) return; 
				//*adjust the parent pointer ->left or pp-> right to 0
				if(next->left==cur)
					next->left=0;
				else
					next->right=0;
				cur=next;
			}
			else
			if( next->par!=cur ) 
			{
				//*entered an uplink. so mark the thing as zero.
				if(cur->left==next)
					cur->left=0;
				else
					cur->right=0;
			}
			else
			{
				//*move forward to next
				cur=next;
			}
		}
	}
	
	/// to find the no of values hashed into the array
	/** \returns void
	*/
	std::size_t size() const 
	{
		patricia_node *cur,*prev;
		std::size_t ret_size=0;
		
		prev=cur=root;
		while(cur)
		{
			if(cur->left==prev && cur->left->par==cur)
			{
				//*coming up from the left.
				prev=cur;
				//*try go right other wise got to the parent
				cur=(cur->right && cur->right->par==cur)?cur->right:cur->par;
			}
			else
			if(cur->right==prev && cur->right->par==cur)
			{
				//*coming up from the right
				//*goto the parent
				prev=cur;
				cur=cur->par;
			}
			else
			{
				//*going into a new node so increment size
				ret_size++;
				std::cout<<cur->value.first<<std::endl;
				prev=cur;
				
				//*goto left otherwise goto right other go up.
				cur=(cur->left && cur->left->par==cur)? cur->left
				: ( (cur->right && cur->right->par==cur)? cur->right
				: cur->par );
			}
			//std::cout<<((prev->left==cur)?"GOING LEFT":(prev->right==cur)?
			//"GOING RIGHT":"GOING UP")<<std::endl;
			assert(ret_size!=10);
		}
		return ret_size;
	}

	///class destructor
	~patricia()
	{
		this->clear();
	}

	private:

	///Has a pair of node pointers. usually has firt element as node found 
	///and second element as the node from which the upward edge goes.
	typedef std::pair<patricia_node*,patricia_node*> node_pair_type;

	///find element points to point at which the bits become uncommon between two keys
	///and second element as uncommon bit.
	///second element=2 if two strings are equal.
	typedef std::pair<std::size_t,int> common_type;

	///gets node pair b/w which a key is to inserted also returns a pair of index and bit value of the key at that index
	///to get the pair of node between which a a node is to inserted
	///this is for random access iterator.
	/**
	 	\returns std::pair< node_pair, common_type>. node_pair, ie pair of node between which the new node is to be inserted 
	 	node_pair::second is the expected child of new node
	 	node_pair::first is the current parent of expected child.
	 	\param key is key to be inserted.
	 */
	inline std::pair< node_pair_type, common_type>
	 get_insert_pair(const key_type &key,rand_tag) const 
	{	
		patricia_node *cur=root,*next=root;
		std::size_t pos=0;
		common_type common;
		node_pair_type  node_pair;
		const std::size_t key_size=Key_traits::size(key);
		key_iterator it=Key_traits::begin(key);

		//*get the insert pair for the key.
		node_pair=find_node_prev(root,key,iterator_cat(),key_size);

		next=node_pair.first;
		common=get_common_bits(key,next->value.first );
		
		//*if everything is common return the data type
		if ( common.second==2 ) 
			return  std::make_pair(node_pair, common);

		//*else search for the key again
		next=cur=root;
		while (next->index < common.first) {
			cur=next;
			pos= cur->index / (bit_width + 1);
			//if ( pos >= key_size ) break;
			next=get_nth_bit ( Key_traits::get_element(it + pos), cur->index % (bit_width + 1) )? 
			cur->right: cur->left;
			//*is its an upward link break.
			if(cur->index >= next->index) break;
		}
		
		//*return the node pair along with common pair.
		return std::make_pair(std::make_pair(next,cur), common);
	}

	///gets node pair b/w which a key is to inserted also returns a pair of index and bit value of the key at that index
	///to get the pair of node between which a a node is to inserted
	///This uses forward iteration only
	/**
	 	\returns std::pair< node_pair, common_type>. node_pair, ie pair of node between which the new node is to be inserted 
	 	node_pair::second is the expected child of new node
	 	node_pair::first is the current parent of expected child.
	 	\param key is key to be inserted.
	 */
	template<class T>
	inline std::pair< node_pair_type, common_type>
	get_insert_pair (const key_type &key,T) const
	{
		key_iterator it,end_it;
		key_element_type temp_element;
		patricia_node *cur=root,*next=root;
		std::size_t pos=0;
		node_pair_type node_pair;
		common_type common;
		
		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);
		
		//*get the node pair between which the key is supposed to be inserted
		node_pair=find_node_prev(root,key,T());
		next=node_pair.first;
		cur =node_pair.second;
		common=get_common_bits(key,next->value.first);
		
		//*key already exists
		if(common.second==2) 
			return std::make_pair(std::make_pair(next,cur),common);


		//*find the correct parent and child in between which this node needs to be added.
		//*find until the index 
		//*note: parent can be equal to child
		//*cur=parent, next=child
		it=Key_traits::begin(key);
		cur=root;
		while(it!=end_it)
		{
			if ( (cur->index)%(bit_width+1)==0 )
			{
				//*simply got the right because. left not has a lesser length than the key.
				next=cur->right;
			}
			else
			{
				//*goto left or right appropriately 
				temp_element=Key_traits::get_element(it);
				next=get_nth_bit(temp_element,(cur->index)%(bit_width+1))?
					cur->right:cur->left;
			}

			//*fortunately in this case; this should happen only when it==end_it occures.
			assert(next);
			if ( next->index+1 > common.first ) break;  //insert at cur using next->value->first
			if ( next->index <= cur->index ) break;

			//*increment pos and it until the required pos matching index is reached
			while( pos < (next->index)/(bit_width+1) && it!=end_it )
			{
				++pos;
				++it;
			}
			cur=next;
		}
		
		return std::make_pair(std::make_pair(next,cur),common);
	}

	///Insert new node
	/**
	 	\returns reference to data_type in the new node.
	 	\param key is key to be inserted.
	 */
	template<class T>
	inline data_type &insert_new_node(const key_type &key,T)
	{
		key_iterator it,end_it;
		patricia_node *cur=root,*next=root,*temp_node,*old_root=0;
		std::pair< std::pair<patricia_node*,patricia_node*>,
	std::pair<std::size_t,int> > insert_pair;
		
		//std::cout<<"in random access"<<std::endl;
		
		std::pair<std::size_t,int> common;

		//std::cout<<"\nInserting \""<<key<<"\""<<std::endl;
		
		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);

		//*do some jugglery to ensure that inserting "" first as root wont cause any problems
		//*move root to the right position while inserting the second
		//*that will take care of the assert(next) in find 
		old_root=0;
		if(root!=0 && root->left==root)
		{
			if(it==end_it) return root->value.second; //if one adds "" twice
			old_root=root;
			root=0;
		}

		if(root==0)
		{
			root=node_allocator.allocate(1);
			if(it!=end_it)
			{
				new(root) patricia_node( key, 0, old_root, root, 0 );
				if(old_root) 
				{
					//*reassign the old root the position.
					old_root->par=root;
				}
			}
			else
				new(root) patricia_node( key, 0, root, 0, 0 );
			return root->value.second;
		}
		
		if(it==end_it)  
		{
			//*"" inserted
			
			if(root->left==0) 
			{
				root->left = node_allocator.allocate(1);
				new(root->left) patricia_node(key,0,root->left,0,root);
			}
			return root->left->value.second;
		}

		//*get the insert pair. the pair in between which the things should be inserted
		insert_pair=get_insert_pair(key,T());
		
		common=insert_pair.second;
		next=insert_pair.first.first;
		cur=insert_pair.first.second;
		//*if the key already exists return
		if(common.second==2)
		{
			std::cout<<"The key \""<<key<<"\" already exists"<<std::endl;
			return next->value.second;
		}
		
		//*allocate new node
		temp_node=node_allocator.allocate(1);
		
		//*rewire the cur to temp_node
		if(cur->left==next) 
			cur->left=temp_node;
		else
		if(cur->right==next) 
			cur->right=temp_node;
		else
			assert(false);

		
		if(common.second) 
			//*temp_node should point to inself at 1 ie right
			new (temp_node) patricia_node(key,common.first,next,temp_node,cur);
		else
			//*temp_node should point to inself at 0 ie right
			new (temp_node) patricia_node(key,common.first,temp_node,next,cur);

		
		assert(root->par==0);

		//*if its not an upward link adjust the next->par
		if(cur->index < next->index) next->par=temp_node;
		
		assert(root->par==0);
		return temp_node->value.second;
	}
	
	///get nth element nth bit in the element
	/** \returns nth bit
	 	\param element whose nth bit is to be found
	 	\param n is the bit to be got 
	 */
	inline bool get_nth_bit(const key_element_type &element,const int &n) const 
	{
		return n==0? 1 : ( ( element&(1<<(bit_width - n )) )  !=0 );
	}
	
	///get the position upto which the bits are equal.
	/** 
	  \returns inclusive of end bit after each pos and the not(un) common bit in k1;
	  \returns and the not common bit as two if both keys are equal.
	  \param k1  is the first key
	  \param k2  is the second key
	 */
	inline std::pair<std::size_t,int> get_common_bits(const key_type &k1,const key_type &k2) const
	{
		key_iterator k1_it,k2_it,k1_end,k2_end;
		//std::cout<<"comparing "<<k1<<" and "<<k2<<std::endl;
		int uncommon_bit;
		key_element_type t1,t2;
		bool unequal=false;
		std::size_t pos=0;

		k1_it=k1.begin();
		k2_it=k2.begin();
		k1_end=k1.end();
		k2_end=k2.end();

		//*check until the chars are unequal and increment pos accordingly
		while(k1_it!=k1_end && k2_it!=k2_end )
		{
			if ( Key_traits::get_element(k1_it) != Key_traits::get_element(k2_it) )
			{
				unequal=true;
				break;
			}
			pos+=bit_width+1;
			++k1_it;
			++k2_it;
		}

		if(unequal)
		{
			//*find the unequal bit
			std::size_t bit_pos=0;
			t1=Key_traits::get_element(k1_it);
			t2=Key_traits::get_element(k2_it);
			key_element_type x=t1;
			while((t1)!=( t2))//possibly make this ( t1 &  x) != ( t1 &  x )
			{
				t1>>=1;
				t2>>=1;
				++bit_pos;
			}
			uncommon_bit= (  ( x & (key_element_type(1)<<(bit_pos-1))  )  !=0);	
			pos+=bit_width-bit_pos+1;
			//increment pos accordingly
		}
		else
		{
			//*assign uncommon_bit==2 if ( k1_it==k1_end && k2_it==k2_end )
			//*if k1_it==k1_end && k2_it!=k2_end assing 0
			//*otherwise 1
			uncommon_bit=(k1_it==k1_end)?((k2_it==k2_end)?2:0):1;
		}

		return std::make_pair<std::size_t,int>(pos,uncommon_bit);
	}

	///find a node for the key.
	/**
	 \returns the pointer to the "found" node
	 \warning does not check whether the keys are actually equal. 
	 Will be needed to be done outside.
	 \param cur is the node from which the search need to be made.
	 \param key is the key to be searched
	 */
	template<class T>
	inline patricia_node* find_node(const patricia_node* cur,const key_type &key,T) const 
	{
		patricia_node *next;
		std::size_t pos=0;
		key_iterator it,end_it;
		key_element_type temp_element;

		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);

		if(root==0) return 0;
		if(it==end_it)
			return root->left;
		//*adjust pos to the appropriate value
		while ( pos < (cur->index)/(bit_width+1) && it!=end_it ) 
		{
			++pos;
			++it;
		}

		while(it!=end_it) {
			if ( (cur->index)%(bit_width+1)==0 )
			{
				//*always go right as length of the key on right is lesser.
				next=cur->right;
			}
			else {
				//*go left or right appropriately.
				temp_element=Key_traits::get_element(it);
				//std::cout<<"getting "<<(cur->index)%(bit_width+1)<<"th bit of "<<temp_element<<std::endl;
				next=get_nth_bit ( temp_element , (cur->index)%(bit_width+1) ) ?
					cur->right : cur->left;
			}
			if(next==0) return next;
			//std::cout<<(cur->left==next?"going left":"going right")<<std::endl;
			//std::cout<<"next: "<<next->value.first<<std::endl;

			//*if we are heading toward the parent stop
			if ( next->index <= cur->index ) break;

			while ( pos < (next->index)/(bit_width+1) && it!=end_it ) {
				++pos;
				++it;
			}
			cur=next;
		}

		if ( it == end_it && pos==(cur->index)/(bit_width+1) ) {
			//*just make the required connection
			next=cur->left;
		}
		return next;
	}

	///find a node for the key (over loaded for random access)
	/**
	 \returns the pointer to the "found" node
	 \warning does not check whether the keys are actually equal. 
	 Will be needed to be done outside.
	 \param cur is the node from which the search need to be made.
	 \param key is the key to be searched.
	 \param key_size if the size of the key.
	 */
	inline patricia_node* find_node(const patricia_node *cur, const key_type &key,rand_tag,std::size_t key_size=0) const
	{
		patricia_node  *next=0;
		key_iterator it, end_it;
		it=Key_traits::begin(key);
		std::size_t pos=0;
		if(key_size==0)
			key_size=key.size();
		
		if ( root==0 ) return 0;

		while (true )
		{
			pos= cur->index / (bit_width + 1);
			//*make sure the key length is always lesser
			if ( pos >= key_size ) break;
			std::cout<<"getting "<<cur->index<<"th bit of "<<key<<std::endl;
			next=get_nth_bit ( Key_traits::get_element(it + pos), cur->index % (bit_width + 1) )? 
			cur->right: cur->left;
			if(next==0) return 0;
			//*if we are heading toward the parent stop
			if(cur->index >= next->index) break;
			cur=next;
		}

		if (pos == key_size )
		{
			//*make the required correction if the add the last bit 0 to the node.
			next=cur->left; 
		}

		return next;
	}

	///find a node for the key and prev node also (over loaded for random access)
	/**
	 \returns the pointer to the "found" node and node with upward link to found node
	 as a pair.
	 \warning does not check whether the keys are actually equal. 
	 Will be needed to be done outside.
	 \param cur is the node from which the search need to be made.
	 \param key is the key to be searched
	 \param key_size is the size of key
	 */
	inline node_pair_type
	find_node_prev(patricia_node *cur, const key_type &key, rand_tag,std::size_t key_size=0) const
	{
		patricia_node  *next=0;
		key_iterator it, end_it;
		it=Key_traits::begin(key);
		std::size_t pos=0;
	
		if(key_size==0)
			key_size=key.size();

		if(cur==0) return std::make_pair<patricia_node*,patricia_node*>(0,0);
		
		
		if(it==end_it)
		{
			//*special cases for ""
			if ( cur==root  || cur==root->left )
				return std::make_pair(root->left,root);
			else
				return std::make_pair(static_cast<patricia_node*>(0),static_cast<patricia_node*>(0));
		}
		
	
		//*refer find()
		while (true ) {
			pos= cur->index / (bit_width + 1);
			if ( pos >= key_size ) break;
			//std::cout<<Key_traits::get_element(it + pos)<<std::endl;
			next=get_nth_bit ( Key_traits::get_element(it + pos), cur->index % (bit_width + 1) )? 
			cur->right: cur->left;
			//std::cout<<"In find: cur="<<cur->value.first
			//<<" going to next \""<<next->value.first<<"\""<<std::endl;
			if(next==0) return std::make_pair<patricia_node*,patricia_node*>(0,0);
			if(cur->index >= next->index) break;
			cur=next;
		}

		if(pos == key_size ) { 
			next=cur->left; 
		}
	
		return std::make_pair<patricia_node*,patricia_node*>(next,cur);
	}

	///find a node for the key and prev node also
	/**
	 \returns the pointer to the "found" node and node with upward link to found node
	 as a pair.
	 \warning does not check whether the keys are actually equal. 
	 Will be needed to be done outside.
	 \param cur is the node from which the search need to be made.
	 \param key is the key to be searche
	 */
	template<class T>
	inline std::pair<patricia_node*,patricia_node*>
	find_node_prev(patricia_node *cur, const key_type &key, T ,std::size_t=std::size_t()) const
	{
		patricia_node *next;
		std::size_t pos=0;
		key_iterator it,end_it;
		key_element_type temp_element;

		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);

		if(cur==0) return std::make_pair(cur,cur);
		if(it==end_it)
		{
			//*special cases for ""
			if ( cur==root  || cur==root->left )
				return std::make_pair(root->left,root);
			else
				return std::make_pair(static_cast<patricia_node*>(0),static_cast<patricia_node*>(0));
		}
		
		//*adjust pos to match with cur->index
		while ( pos < (cur->index)/(bit_width+1) && it!=end_it ) {
			++pos;
			++it;
		}

		//*ref find  :)
		while(it!=end_it) {
			if ( (cur->index)%(bit_width+1)==0 )
			{
				next=cur->right;
			}
			else {
				temp_element=Key_traits::get_element(it);
				std::cout<<temp_element<<std::endl;
				next=get_nth_bit ( temp_element , (cur->index)%(bit_width+1) ) ?
					cur->right : cur->left;
			}
			if(next==0) return std::make_pair(next,next);
			
			//std::cout<<next->value.first<<"<--"<<cur->value.first<<std::endl;
			if ( next->index <= cur->index ) { break;}

			while ( pos < (next->index)/(bit_width+1) && it!=end_it ) {
				++pos;
				++it;
			}
			cur=next;
		}

		if ( it == end_it && pos==(cur->index)/(bit_width+1) ) {
			next=cur->left;
		}
		return std::make_pair(next,cur);
	}

	 ///erases the found node.
	/**
	 \returns void
	 \param found is the node to be erase.
	 \param prev is the node with the uplink.
	 */
	void erase(patricia_node*found,patricia_node *prev)
	{
		if(root==0 || found==0) return;
		
		std::cout<<"in erase: found "<<found->value.first<<" and prev "<<prev->value.first<<std::endl;

		if ( found-> par == 0 ) {  
			//*==> found=root node 

			std::cout<<"par"<<std::endl;
			assert(root==found);

			if( (found->right==0 || found->left==0) && found==prev){ 
				//*only root or only ""
				deallocate_node(found);		
				root=0;
				return;
			}
			//std::cout<<"wrong place"<<std::endl;
			
			if(prev==found){ 
				//*also takes care of taking to "" to the root
				root=(found->right==found)?found->left:found->right;
				if(root) root->par=0;
				deallocate_node(found);
				return;
			}
		}
		else
		if ( found->right==0 || found->left==0 )
			prev=found;
		//* now prev contains the node from which found is wired with an uplink

		std::cout<<"here"<<std::endl;
		//*if a node is looping to itself
		//*deleting the leaf node. 
		//*this case for root node is handled before
		if ( prev == found ) {		
				

				//*check how its liked to the par
				//*and found->par!=0 as root node has been handled before.
				if ( found->par->right == found )
				{
					//*rewire the parent.
					found->par->right = (found->right==found)?found->left:found->right;
					//*also take care rewiring the new childs parent
					if( found->par->right && found==found->par->right->par ) found->par->right->par=found->par;
				}
				else
				if ( found->par->left == found )
				{
					//*rewire the parent.
					found->par->left = (found->right==found)?found->left:found->right;
					//*also take care rewiring the new childs parent
					if( found->par->left && found==found->par->left->par ) found->par->left->par=found->par;
				}
				else
					assert(false);
				//std::cout<<"prev==found with key="<<key<<std::endl;
				if ( found->right==0 || found->left==0 )
				{
					//*Probably erasing "" for lets be extra careful
					//*check whether the par is as expected proper.
					assert(found->par==root);
					std::cout<<"\"\" node to be erased"<<std::endl;
				}
				deallocate_node(found);	
				return;
		}
		
		//*since we are moving prev to where found is we need to rewire the prev parents
		//*check how the prev->par is attached to prev
		if(prev->par->right==prev) {
			//*rewrite the prev->parent to the prev's other child
			prev->par->right=(prev->right==found)?prev->left:prev->right;
			//*if prev->right == prev or prev->left == prev there is a self loop so we need to take for it too.
			//*and not re-adjust par.
			if( prev->right!=prev && prev->left!=prev && prev->par->right ) prev->par->right->par=prev->par;
		}
		else {
			//std::cout<<"far away here"<<std::endl;
			//*rewrite the prev->parent to the prev's other child
			prev->par->left=(prev->right==found)?prev->left:prev->right;
			//*if prev->right == prev or prev->left == prev there is a self loop so we need to take for it too.
			//*and not re-adjust par.
			if( prev->right!=prev && prev->left!=prev && prev->par->left) prev->par->left->par=prev->par;
		}

		//*if we are deallocating the root
		if(found->par==0)
			root=prev;
		//*finally copy the pointers and index of found to prev node
		copy_node_ptr(prev,found);
		deallocate_node(found);
	}

	/// erase the node having key 
	/**
	 \returns void
	 \param key is the key to be erase.
	 */ 
	template<class T>
	void erase(const key_type& key,T)
	{
		patricia_node  *cur,*found;
		std::pair<patricia_node*,patricia_node*> temp_pair,check_pair;
		
		if(root==0) return;
		
		assert(root->par==0);
		
		std::cout<< ((root->left!=0)?" \"\" exists":"\"\" does not exists")<<std::endl;
		
		assert( (root->left!=0 && root->left!=root)? root->left->par==root: 1 );

		temp_pair=find_node_prev(root,key,T());
		found=temp_pair.first;
		cur=temp_pair.second;
		if(found==0) return;

		//check_pair=find_node_prev(root,key,rand_tag());
		//assert(check_pair==temp_pair);
		
		erase(found,cur);	
	}

	///deallocate a particular node. Used by erase.
	/**
	 \returns void
	 \param found is the pointer to the node to be erased
	 */
	void inline deallocate_node(patricia_node *&found)
	{
		node_allocator.destroy(found);
		node_allocator.deallocate(found,1);
	}


	/// used by erase(key);
	///copies pointers of two nodes. replacing the "found" node by "to" node.
	/**
	 \returns void
	 \param to is the node to replaced by.
	 \param found is the node to replace.
	 */ 
	void inline copy_node_ptr(patricia_node* to, patricia_node* found)
	{
		if(found->par!=0) {
			//*rewire found->par
			if(found->par->right==found) 
				found->par->right=to;
			else
				found->par->left=to;
		}
		to->par=found->par;

		if(found->right!=found)  
		//*if found is not connected to itself
			to->right=found->right;
		else
			to->right=to;

		if(found->left!=found)  
		//*if found is not connected to itself
			to->left=found->left;
		else
			to->left=to;

		//*rewire the parents of left and right
		if ( found->left && found->left->par==found ) found->left->par=to;
		if ( found->right && found->right->par==found ) found->right->par=to;

		to->index=found->index;
	}
	
	///finds the lower bound corresponding to the key.
	/**
	 	\returns the iterator pointing to the lower bound. 
	 	if there is no lower bound return end()
	 	\param key is the key whose upper bound is to be found
	 */
	template<class T>
	inline iterator lower_bound(const key_type &key,T)
	{
		typedef std::pair<patricia_node*,patricia_node*> node_pair_type;
		typedef std::pair<std::size_t, int> common_type;
		
		std::pair<node_pair_type,common_type> node_pair;
		common_type common;
		patricia_node* next,*cur;
		iterator it;
		
		
		if(root==0) return end();
		if ( Key_traits::begin(key)==Key_traits::end(key) )
		{
			//* Special case for ""
			if(root->left==0) return end();
			else
				return begin();
		}
		
		/// Check whether there keys other that "" at all.
		if(root->right==0)
		{
			 //* Lowerbound has to be ""
			 return begin();
		}
		
		/// Get pair where one needs to insert the node corresponding to the key 
		/// this we "simulate" the insertion and check the element lesser that it.
		node_pair = get_insert_pair(key,T());
		next = node_pair.first.first;
		cur  = node_pair.first.second;
		common = node_pair.second;
		
		switch(common.second)
		{
			case 2:
			//*case where key is found.
				return iterator(next,cur);
			case 0:
			//*case where next->value.key > key. this ensures that all the nodes in the 
			//*subtree also greater
				return --iterator(next,cur);
			case 1:
				//*case where the lower bound is in the subtree of next.
				it=iterator(next,cur);
				//*we just move to the highest value in the subtree.
				it.to_right_most();
				return it;
		}
		assert(false);
	}
	
	///finds the upper bound corresponding to the key.
	/**
	 	\returns the iterator pointing to the upperbound. 
	 	if there is no upper bound return end()
	 	\param key is the key whose upper bound is to be found
	 */
	template<class T>
	inline iterator upper_bound(const key_type &key,T)
	{
		typedef std::pair<patricia_node*,patricia_node*> node_pair_type;
		typedef std::pair<std::size_t, int> common_type;
		
		std::pair<node_pair_type,common_type> node_pair;
		common_type common;
		patricia_node* next,* cur;
		iterator it;
		
		if ( root==0 ) return end();
		if ( Key_traits::begin(key)==Key_traits::end(key) )
			return begin();

		//* Check whether there keys other that "" at all.
		if ( root->right==0 )
		{
			//*explicitly find the upper bound
			if(Key_traits::begin(key)!=Key_traits::end(key) )
				 return end();
			else
				return begin();
		}

		node_pair=get_insert_pair(key,T());
		next=node_pair.first.first;
		cur =node_pair.first.second;
		common=node_pair.second;
		std::cout<<"Upper Bound:SECOND: "<<common.second<<std::endl;
		switch ( common.second )
		{
		  case 2:
			//*key aleardy exists
			return iterator(next,cur);
		  case 1:
		  	//*upper bound is not in the subtree of next.its just greater than the subtree
			return ++iterator(next,cur);
		  case 0:
		  	//*it is in the subtree
			it=iterator(next,cur);
			std::cout<<next->value.first<<"<-"<<cur->value.first<<std::endl;
			//*go to the least element in the subtree.
			it.to_left_most();
			return it;
		}
		assert(false);
	}

	///finds the iterator range corresponding to the prefix (overloaded for random access)
	/**
	 	\returns iterator range
	 	\param key is the key whose prefix range is to be found
	 */
	std::pair<iterator,iterator> prefix_range(const key_type &key,rand_tag)
	{
		std::size_t pos=0;
		std::pair<patricia_node*,patricia_node*> node_pair;
		std::pair<std::size_t,int> common;
		const std::size_t key_size=Key_traits::size(key);
		patricia_node *cur=root,*next=root;
		key_iterator it=Key_traits::begin(key);

		if(root==0) return std::make_pair(end(),end());
		
		//*if its "" then entire patricia is needed
		if(Key_traits::begin(key)==Key_traits::end(key))
			return std::make_pair(begin(),end());
	
		
		//*now key!="" and there is no other key in the patricia
		//*so only option is end(),end() 
		if(root->right==0) return std::make_pair(end(),end());

		//*find node in between which the node was supposed to be inserted
		node_pair=find_node_prev(root,key,iterator_cat(),key_size);

		next=node_pair.first;
		common=get_common_bits(key,next->value.first );
		if ( common.second==2 ) {
			common.second=0;
			//*if key is already found we will need to re adjust this thing
			//*the bit index to be searched for
			common.first=key_size*( bit_width +  1 );
			std::cout<<"FOUND"<<std::endl;
		}

		//*if we did not find a key with entire string as prefix. then then
		//*then we will need to ditch the procedure here.
		if ( common.second!=0 || common.first % ( bit_width +  1 ) != 0)
			return std::make_pair(end(),end());
		std::cout<<"HERE with KEY="<<key<<std::endl;

		//*we start searching for the until we get a key  with index>common.first
		next=cur=root;
		while (next->index < common.first) {
			cur=next;
			pos= cur->index / (bit_width + 1);
			//if ( pos >= key_size ) break;
			next=get_nth_bit ( Key_traits::get_element(it + pos), cur->index % (bit_width + 1) )? 
			cur->right: cur->left;
			if(cur->index >= next->index) break;
		}

		
		iterator right=iterator(next,cur);
		iterator left =iterator(next,cur);
		
		//*find the smallest in the subtree of next
		left.to_left_most();
		
		//*find one greater than greatest in the subtree of next.
		++right;
		return std::make_pair(left,right);
	}

	///finds the iterator range corresponding to the prefix (overloaded for random access)
	/**
	 	\returns iterator range
	 	\param key is the key whose prefix range is to be found
	 */
	template<class T>
	std::pair<iterator,iterator> prefix_range(const key_type &key,T)
	{
		key_iterator it,end_it;
		key_element_type temp_element;
		patricia_node *cur=root,*next=root;
		std::size_t pos=0;
		std::pair<patricia_node*,patricia_node*> node_pair;
		std::pair<std::size_t,int> common;
		bool no_check=false;
		
		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);
		
		//*if its "" then entire patricia is needed
		if(root==0) return std::make_pair(end(),end());		
		if(Key_traits::begin(key)==Key_traits::end(key))
			return std::make_pair(begin(),end());
			
		//*if we did not find a key with entire string as prefix. then then
		//*then we will need to ditch the procedure here.
		if(root->right==0) return std::make_pair(end(),end());
		
		node_pair=find_node_prev(root,key,T());
			
		next=node_pair.first;
		assert(find_node_prev(root,key,iterator_cat()).first==next);
		cur =node_pair.second;
		assert(find_node_prev(root,key,iterator_cat()).second==cur);
		common=get_common_bits(key,next->value.first);
		
		if(common.second==2) 
		{
			common.second=0;
			//*no_check will indicate whether or not to check for next->index+1 > common.first
			no_check=true;
		}
		if ( common.second!=0 || common.first % ( bit_width +  1 ) != 0)
			return std::make_pair(end(),end());

		//*we start searching for the until we get a key  with index>common.first
		it=Key_traits::begin(key);
		cur=root;
		while(it!=end_it)
		{
			if ( (cur->index)%(bit_width+1)==0 )
				next=cur->right;
			else
			{
				temp_element=Key_traits::get_element(it);
				next=get_nth_bit(temp_element,(cur->index)%(bit_width+1))?
					cur->right:cur->left;
			}

			//*fortunately in this case; this should happen only when it==end_it occures.
			assert(next);
			if ( no_check || next->index+1 > common.first ) break;  //insert at cur using next->value->first
			if ( next->index <= cur->index ) break;

			while( pos < (next->index)/(bit_width+1) && it!=end_it )
			{
				++pos;
				++it;
			}
			cur=next;
		}
		iterator right=iterator(next,cur);
		iterator left =iterator(next,cur);

		//*find the smallest in the subtree of next
		left.to_left_most();
		
		//*find one greater than greatest in the subtree of next.
		++right;
		return std::make_pair(left,right);
	}
	
	///used by copy_patricia
	///find the ancestor node in this patricia corresponding to parent node in other patricia
	/**
	 \returns pointer to the ancestor patricia which has corresponding incoming upward link
	 \param this_cur is the node pointer whose upward link is to be found
	 \param other_cur is the node pointer, of the other patricia, which has the upward link.
	 \param par is the node in the other patricia which is the upward link.
	 */
	patricia_node *copy_patricia_find_par(patricia_node* const &this_cur,
	const patricia_node* const &other_cur,const patricia_node * const &par)
	{
		patricia_node *cur=this_cur;
		const patricia_node* o_cur=other_cur;
		if(other_cur==0) return 0;
		while(o_cur!=par)
		{
			cur=cur->par;
			o_cur=o_cur->par;
		}
		return cur;
	}

	///copy other patricia on to this patricia.
	/**
	 \return void
	 \param other_root is the pointer to the root of other patricia to be copied.
	 */
	void copy_patricia(const patricia_node *other_root)
	{
		const patricia_node *const*other_cur,*other_temp,*other_prev;
		
		//*cur is the pointer to the edge.
		//*ie, most of the times, cur=&node->left or cur=&node->right
		patricia_node **cur,*prev;
		
		if(other_root==0)
		{
			this->root=0;
			return;
		}

		cur=&root;
		other_cur=&other_root;
		other_prev=0;
		prev=0;
		while(true)
		{
			//assert(prev!=other_root);
			//*if the current edge in other patricia does not point to 
			//*null and is not the upward edge.
			if( (*other_cur) && (*other_cur)->par==other_prev)
			{
				//*allocate a node in the current of edge of this node.
				(*cur)=node_allocator.allocate(1);
				
				//*copy index, data_type and key_type.
				new(*cur) patricia_node(**other_cur);
				(*cur)->par = prev;
				
				//*get in to the next node and and move to the left edge.
				other_prev=*other_cur;
				other_cur=&(other_prev->left);
				
				//*do the same for the this patricia too.
				prev=*cur;
				cur=&(prev->left);
				//std::cout<<"copy:here1"<<std::endl;
				continue;
			}
			std::cout<<"copy:here2"<<std::endl;
			
			//*when we come here we know that the edge is iether null or point upwards
			//*copy_patricia_find_par returns null is edge points to null.
			//*otherwise it returns the corresponding parent
			(*cur)=copy_patricia_find_par(prev,other_prev,*other_cur);
			
			//*some random assert to check the pointers dont get interchaged by mistake
			assert((*cur)!=other_root);
			
			//*if the edge is the left is the left edge
			if((*other_cur)==other_prev->left)
			{
				//*move towards the right egde
				other_cur=&(other_prev->right);
				//assert(other_prev!=prev);
				cur=&(prev->right);
			}
			else
			{
				//*if already on the right edge one needs to move upward to traverse further.
				other_temp=other_prev->right;
				//*check whether we are on the right edge.
				while(other_temp==other_prev->right)
				{
					other_temp=other_prev;
					//*if right keep moving up
					other_prev=other_prev->par;
					
					//*do the same in other patricia too.
					prev=prev->par;
					if(other_prev==0) 
					{
						//*Finished scanning through the entire patricia.
						//*now return.
						return;
					}
				}
				
				//*finally its not right any more. :)
				//*so adjust both edges towards the right
				other_cur=&other_prev->right;
				cur=&prev->right;
			}
		}
	}
};

	///check the equality of the two patricia nodes
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
