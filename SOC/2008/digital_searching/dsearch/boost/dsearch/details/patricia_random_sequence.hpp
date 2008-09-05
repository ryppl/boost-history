//  (C) Copyright Chintan Rao H 2008. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


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
		key_size=Key_traits::size(key);
	
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
		#ifdef PAT_DEBUG
			std::cout<<"FOUND"<<std::endl;
		#endif
	}

	//*if we did not find a key with entire string as prefix. then then
	//*then we will need to ditch the procedure here.
	if ( common.second!=0 || common.first % ( bit_width +  1 ) != 0)
		return std::make_pair(end(),end());
	#ifdef PAT_DEBUG
		std::cout<<"HERE with KEY="<<key<<std::endl;
	#endif

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
		key_size=Key_traits::size(key);

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


