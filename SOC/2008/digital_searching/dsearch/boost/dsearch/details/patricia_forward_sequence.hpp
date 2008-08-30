
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

