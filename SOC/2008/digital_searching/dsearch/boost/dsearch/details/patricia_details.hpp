//  (C) Copyright Chintan Rao H 2008. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

///Has a pair of node pointers. usually has firt element as node found
///and second element as the node from which the upward edge goes.
typedef std::pair<patricia_node*,patricia_node*> node_pair_type;

///find element points to point at which the bits become uncommon between two keys
///and second element as uncommon bit.
///second element=2 if two strings are equal.
typedef std::pair<std::size_t,int> common_type;


#include "patricia_random_sequence.hpp" 
#include "patricia_forward_sequence.hpp" 

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
		#ifdef PAT_DEBUG
			std::cout<<"The key \""<<key<<"\" already exists"<<std::endl;
		#endif
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



 ///erases the found node.
/**
 \returns void
 \param found is the node to be erase.
 \param prev is the node with the uplink.
 */
void erase(patricia_node*found,patricia_node *prev)
{
	if(root==0 || found==0) return;
	#ifdef PAT_DEBUG	
		std::cout<<"in erase: found "<<found->value.first<<" and prev "<<prev->value.first<<std::endl;
	#endif		
	if ( found-> par == 0 ) {  
		//*==> found=root node 
	#ifdef PAT_DEBUG	
		std::cout<<"par"<<std::endl;
	#endif		
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
	#ifdef PAT_DEBUG	
		std::cout<<"here"<<std::endl;
	#endif
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
				#ifdef PAT_DEBUG
					std::cout<<"\"\" node to be erased"<<std::endl;
				#endif
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
	
	#ifdef PAT_DEBUG
		std::cout<< ((root->left!=0)?" \"\" exists":"\"\" does not exists")<<std::endl;
	#endif
	
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
	#ifdef PAT_DEBUG
		std::cout<<"Upper Bound:SECOND: "<<common.second<<std::endl;
	#endif
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
		#ifdef PAT_DEBUG
			std::cout<<next->value.first<<"<-"<<cur->value.first<<std::endl;
		#endif
		//*go to the least element in the subtree.
		it.to_left_most();
		return it;
	}
	assert(false);
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
		
		#ifdef PAT_DEBUG
			std::cout<<"copy:here2"<<std::endl;
		#endif
		
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

void pat_clear()
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

std::size_t pat_size() const 
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
		#ifdef PAT_DEBUG
			std::cout<<cur->value.first<<std::endl;
		#endif
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
