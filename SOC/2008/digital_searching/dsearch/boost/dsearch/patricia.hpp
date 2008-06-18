//CONTAINS REWRITE OF PATRICIA.HPP

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

//replace all key assignement operations with key_copy
//replace all key.size() with key_traits::size();

namespace boost{
namespace dsearch{
template<class Key,class Mapped,class Key_traits,class Alloc=std::allocator<char> >
class patricia{
	private: 
	typedef patricia<Key,Mapped,Key_traits,Alloc> self;
	typedef typename Key_traits::const_iterator key_iterator;
	typedef typename Key_traits::element_type key_element_type;

	enum{
		bit_width=8*sizeof(typename Key_traits::element_type)
	};

	public:
	typedef Key key_type;
	typedef Mapped data_type;
	typedef std::pair<const key_type,data_type> value_type;

	private:
	typedef typename std::iterator_traits<typename Key_traits::const_iterator>::iterator_category iterator_cat;
	typedef typename std::random_access_iterator_tag rand_tag;


	class patricia_node {
		typedef patricia_node self;
		public:
	
		self *par; //avoids the usage of a stack to traverse
		value_type value;
		std::size_t index;
		self *left,*right;//left for zero.. right for one.
	
		patricia_node(): par(0),  index(0), left(0), right(0)
		{
		}
		patricia_node(const key_type& key_,const std::size_t &index_,patricia_node *left_
		,patricia_node *right_,patricia_node *par_) 
		:par(par_),value(make_pair(key_,0)),index(index_),left(left_),right(right_)
		{
		}
		patricia_node(const patricia_node &other)
		:par(0),value(other.value),index(other.index),left(0),right(0)
		{
		}
	};
	typedef typename Alloc::template rebind<patricia_node>::other node_allocator_type;
	node_allocator_type node_allocator;
	patricia_node *root;

	public:
	typedef patricia_iterator<value_type, patricia_node, Alloc> iterator;
	typedef patricia_iterator<const value_type, const patricia_node, Alloc> const_iterator;

	patricia(): root(0)
	{
	}
	
	patricia(const self &other)
	{
		copy_patricia(other.root);
	}

	data_type &operator [](const key_type &k)
	{
		#ifndef FORWARD
			return insert_new_node(k,iterator_cat());
		#else
			return insert_new_node(k,0);
		#endif
	}

	void insert(const value_type &v)
	{
		this->operator [](v.first)=v.second;
	}

	iterator begin()
	{
		return iterator ( root, true );
	}
	
	iterator end()
	{
		return iterator ( root, false );
	}

	bool find(const key_type &k)
	{
		std::pair<std::size_t,int> common;
		patricia_node *node;
		#ifndef FORWARD
			node=find_node(root,k,iterator_cat());
		#else
			node=find_node(root,k,0);
		#endif
		if(node==0) return 0;
		common=get_common_bits(node->value.first,k);
		return (common.second==2);
		return true;
	}

	void erase(const key_type &k)
	{
		#ifndef FORWARD
			erase(k,iterator_cat());
		#else
			erase(k,0);
		#endif
	}

	bool empty() const
	{
		//if(root)
		//	std::cout<<"NOT EMPTY "<<root->value.first<<std::endl;
		return root==0;
	}

	void clear()
	{
		patricia_node *cur,*next;
		if(root==0) return;
		cur=root;
		root=0;
		while(true)
		{
			next=cur->left?cur->left:cur->right;
			if(next==0)
			{
				next=cur->par;
				delete cur;
				if(next==0) return; 
				if(next->left==cur)
					next->left=0;
				else
					next->right=0;
				cur=next;
			}
			else
			if( next->par!=cur ) 
			{
				//entered up link. so mark the thing as zero.
				if(cur->left==next)
					cur->left=0;
				else
					cur->right=0;
			}
			else
			cur=next;
		}
	}
	
	~patricia()
	{
		this->clear();
	}

	private:
	//function for Random Access Iterator
	inline data_type &insert_new_node(const key_type &key,rand_tag)
	{
		key_iterator it,end_it;
		patricia_node *cur=root,*next=root,*temp_node,*old_root=0;
		std::size_t pos=0;
		
		//std::cout<<"in random access"<<std::endl;
		
		std::pair<std::size_t,int> common;

		//std::cout<<"\nInserting \""<<key<<"\""<<std::endl;
		
		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);

		const std::size_t key_size=Key_traits::size(key);
	
		//do some jugglery to ensure that inserting "" first as root wont cause any problems
		//move root to the right position while inserting the second
		//that will take care of the assert(next) in find 
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
					std::cout<<"assigning OLD ROOT"<<std::endl;
					old_root->par=root;
				}
			}
			else
				new(root) patricia_node( key, 0, root, 0, 0 );

			return root->value.second;
		}
		
		if(it==end_it)  //"" inserted
		{
			if(root->left!=0) 
				next=root->left;
			else
			{
				root->left = node_allocator.allocate(1);
				new(root->left) patricia_node(key,0,root->left,0,root);
				return root->left->value.second;
			}
		}

		next=find_node(root,key,iterator_cat(),key_size);

		common=get_common_bits(key,next->value.first );
		if ( common.second==2 ) 
		{
			std::cout<<"The key \""<<key<<"\" already exists"<<std::endl;
			return next->value.second;
		}

		next=cur=root;
		while (next->index < common.first) {
			cur=next;
			pos= cur->index / (bit_width + 1);
			//if ( pos >= key_size ) break;
			next=get_nth_bit ( Key_traits::get_element(it + pos), cur->index % (bit_width + 1) )? 
			cur->right: cur->left;
			if(cur->index >= next->index) break;
		}
		temp_node=node_allocator.allocate(1);
		
		if(cur->left==next) cur->left=temp_node;
		else
		if(cur->right==next) 
			cur->right=temp_node;
		else
			assert(false);

		assert(common.second!=2);
		
		if(common.second) //temp_node should point to inself at 1 ie right
			new (temp_node) patricia_node(key,common.first,next,temp_node,cur);
		else
			new (temp_node) patricia_node(key,common.first,temp_node,next,cur);

		assert(root->par==0);
		if(cur->index < next->index) next->par=temp_node;
			
		assert(root->par==0);
		return temp_node->value.second;
	}

	template<class T>
	inline data_type &insert_new_node(const key_type&key,T)
	{
		key_iterator it,end_it;
			key_element_type temp_element;
		patricia_node *cur=root,*next=root,*temp_node,*old_root=0;
		std::size_t pos=0;
		
		//std::cout<<"in random access"<<std::endl;
		
		std::pair<std::size_t,int> common;

		//std::cout<<"\nInserting \""<<key<<"\""<<std::endl;
		
		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);
	
		//do some jugglery to ensure that inserting "" first as root wont cause any problems
		//move root to the right position while inserting the second
		//that will take care of the assert(next) in find 
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
					std::cout<<"assigning OLD ROOT"<<std::endl;
					old_root->par=root;
				}
			}
			else
				new(root) patricia_node( key, 0, root, 0, 0 );
			return root->value.second;
		}
		
		if(it==end_it)  //"" inserted
		{
			if(root->left!=0) 
				next=root->left;
			else
			{
				root->left = node_allocator.allocate(1);
				new(root->left) patricia_node(key,0,root->left,0,root);
				return root->left->value.second;
			}
		}

		next=find_node(root,key,T());
		
		//find the largerst prefix matching the key and the found key
		//std::cout<<"After find"<<std::endl;
		common=get_common_bits(key,next->value.first);
		
		//key already exists
		if(common.second==2) 
		{
			std::cout<<"The key \""<<key<<"\" already exists"<<std::endl;
			return next->value.second;
		}

		//assert(common.first);wrong assert

		//find the parent and child in between which this thing needs to be added.
		//note: parent can be equal to child
		//cur=parent, next=child
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

			//fortunately in this case; this should happen only when it==end_it occures.
			assert(next);
			if ( next->index+1 > common.first ) break;  //insert at cur using next->value->first
			if ( next->index <= cur->index ) break;

			while( pos < (next->index)/(bit_width+1) && it!=end_it )
			{
				++pos;
				++it;
			}
			cur=next;
		}

		temp_node=node_allocator.allocate(1);
		
		if(cur->left==next) cur->left=temp_node;
		else
		if(cur->right==next) 
			cur->right=temp_node;
		else
			assert(false);

		assert(common.second!=2);
		
		if(common.second) //temp_node should point to inself at 1 ie right
			new (temp_node) patricia_node(key,common.first,next,temp_node,cur);
		else
			new (temp_node) patricia_node(key,common.first,temp_node,next,cur);

		assert(root->par==0);
		if(cur->index < next->index) next->par=temp_node;
			
		assert(root->par==0);
		return temp_node->value.second;
	}

	inline std::size_t get_nth_bit(const key_element_type &element,const int &n) const 
	{
		return n==0? 1 : ( ( element&(1<<(bit_width - n )) )  !=0 );
	}

	//returns inclusive of end bit after each pos and the not(un) common bit in k1;
	//returns not common bit as two if both keys are equal.
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

		while(k1_it!=k1_end && k2_it!=k2_end )
		{
			if( Key_traits::get_element(k1_it) != Key_traits::get_element(k2_it) )
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
			t1=Key_traits::get_element(k1_it);
			t2=Key_traits::get_element(k2_it);
			//TODO:optimize this part!!!
			key_element_type x= (key_element_type(1)<<(bit_width-1));
			while((t1 & x)==( t2 & x))//possibly make this ( t1 &  x) != ( t1 &  x )
			{
				x>>=1;
				++pos;
			}
			uncommon_bit= ((t1 & x)!=0);
			++pos;
		}
		else
			uncommon_bit=(k1_it==k1_end)?((k2_it==k2_end)?2:0):1;

		return std::make_pair<std::size_t,int>(pos,uncommon_bit);
	}

	template<class T>
	patricia_node* find_node(const patricia_node* cur,const key_type &key,T)
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

		while(it!=end_it) {
			if ( (cur->index)%(bit_width+1)==0 )
				next=cur->right;
			else {
				temp_element=Key_traits::get_element(it);
				//std::cout<<"getting "<<(cur->index)%(bit_width+1)<<"th bit of "<<temp_element<<std::endl;
				next=get_nth_bit ( temp_element , (cur->index)%(bit_width+1) ) ?
					cur->right : cur->left;
			}
			if(next==0) return next;
			//std::cout<<(cur->left==next?"going left":"going right")<<std::endl;
			//std::cout<<"next: "<<next->value.first<<std::endl;

			if ( next->index <= cur->index ) break;

			while ( pos < (next->index)/(bit_width+1) && it!=end_it ) {
				++pos;
				++it;
			}
			cur=next;
		}

		if ( it == end_it && pos==(cur->index)/(bit_width+1) ) {
			next=cur->left;
		}
		return next;
	}

	/*key_iterator move_iterator(const key_iterator it,const iterator end_it,const std::size_t &cur_pos,const std::size_t &dest_pos)
	{
		std::size_t i=dest_pos-cur_pos;
		key_iterator ret_it=it;
		while(i){
			if ( ret_it==end_it ) return end_it;
			++ret_it;
			--i;
		}
		return ret_it;
	}*/

	patricia_node* find_node(const patricia_node *cur, const key_type &key,rand_tag,std::size_t key_size=0)
	{
		patricia_node  *next=0;
		key_iterator it, end_it;
		it=Key_traits::begin(key);
		std::size_t pos=0;
		if(key_size==0)
			key_size=key.size();

		if(root==0) return 0;
		
		while ( pos < (cur->index)/(bit_width+1) && it!=end_it ) {
			++pos;
			++it;
		}
		while (true ) {
			pos= cur->index / (bit_width + 1);
			if ( pos >= key_size ) break;
			next=get_nth_bit ( Key_traits::get_element(it + pos), cur->index % (bit_width + 1) )? 
			cur->right: cur->left;
			//std::cout<<"In find: cur="<<cur->value.first
			//<<" going to next \""<<next->value.first<<"\""<<std::endl;
			if(next==0) return 0;
			if(cur->index >= next->index) break;
			cur=next;
		}

		if(pos == key_size ) { 
			next=cur->left; 
		}

		return next;
	}

	std::pair<patricia_node*,patricia_node*>
	find_node_prev(patricia_node *root, const key_type &key, rand_tag,std::size_t key_size=0)
	{
		patricia_node  *cur,*next=0;
		key_iterator it, end_it;
		it=Key_traits::begin(key);
		std::size_t pos=0;
		if(key_size==0)
			key_size=key.size();

		if(root==0) return std::make_pair<patricia_node*,patricia_node*>(0,0);
		
		cur=root;
		
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
		if(key_size==0) cur=next;
		return std::make_pair<patricia_node*,patricia_node*>(next,cur);
	}

	template<class T>
	std::pair<patricia_node*,patricia_node*>
	find_node_prev(patricia_node *root, const key_type &key, T ,std::size_t)
	{
		patricia_node *cur=root,*next;
		std::size_t pos=0;
		key_iterator it,end_it;
		key_element_type temp_element;

		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);

		if(root==0) return std::make_pair(root,root);
		if(it==end_it)
			return std::make_pair(root->left,root->left);
		while ( pos < (cur->index)/(bit_width+1) && it!=end_it ) {
			++pos;
			++it;
		}

		while(it!=end_it) {
			if ( (cur->index)%(bit_width+1)==0 )
				next=cur->right;
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

	template<class T>
	void erase(const key_type& key,T)
	{
		if(root==0) return;
		patricia_node  *cur,*next=0,*found;
		key_iterator it, end_it;
		it=Key_traits::begin(key);
		
		std::size_t key_size=key.size();
		std::pair<patricia_node*,patricia_node*> temp_pair,check_pair;
		
		if(root==0) return;
		assert(root->par==0);
		std::cout<< ((root->left!=0)?" \"\" exists":"\"\" does not exists")<<std::endl;
		
		assert( (root->left!=0 && root->left!=root)? root->left->par==root: 1 );
		
		std::cout<<(root->par==0)<<" root key:"<<root->value.first<<std::endl;
		cur=root;
		next=root;


		temp_pair=find_node_prev(root,key,T(),key_size);
		found=next=temp_pair.first;
		cur=temp_pair.second;
		if(found==0) return;
		check_pair=find_node_prev(root,key,rand_tag(),key_size);
		std::cout<<"KEY=="<<key<<";Next key=="<<temp_pair.first->value.first
		<<";Cur key=="<<temp_pair.second->value.first<<std::endl;

		assert(check_pair==temp_pair);

		std::cout<<"in erase: found "<<found->value.first<<" and prev "<<cur->value.first<<std::endl;

		if ( found-> par == 0 ) {  //root node
			std::cout<<"par"<<std::endl;
			assert(root==found);

			if( (found->right==0 || found->left==0) && found==cur){ //only root or only ""
				deallocate_node(found);		
				root=0;
				return;
			}
			std::cout<<"wrong place"<<std::endl;
			if(cur==found){ //also takes care of "" at the root
				root=(found->right==found)?found->left:found->right;
				if(root) root->par=0;
				deallocate_node(found);
				return;
			}
		}
		else
		if ( found->right==0 || found->left==0 )
			cur=found;

		std::cout<<"here"<<std::endl;
		if ( cur == found ) {		//deleting the leaf node. //this case for root node is handled before
				if ( found->par->right == found )
					found->par->right = (found->right==found)?found->left:found->right;
				else
				if ( found->par->left == found )
					found->par->left = (found->right==found)?found->left:found->right;
				else
					assert(false);
				std::cout<<"cur==found with key="<<key<<std::endl;
				if ( found->right==0 || found->left==0 )
				{
					assert(found->par==root);
					std::cout<<"\"\" node to be erased"<<std::endl;
				}
				deallocate_node(found);	
				return;
		}
		
		temp_pair=find_node_prev(cur,cur->value.first,T(),cur->value.first.size());
		/*check_pair=find_node_prev(cur,cur->value.first,rand_tag(),cur->value.first.size());

		std::cout<<"KEY=="<<cur->value.first<<";Next key=="<<temp_pair.first->value.first
		<<";Cur key=="<<temp_pair.second->value.first<<std::endl;
		std::cout<<"KEY=="<<cur->value.first<<";Next key=="<<check_pair.first->value.first
		<<";Cur key=="<<check_pair.second->value.first<<std::endl;
		assert(check_pair==temp_pair);*/
		
		next=temp_pair.first;
		cur=temp_pair.second;
		assert(next->par);
		
		if(next->par->right==next) {
			next->par->right=(next->right==found)?next->left:next->right;
			if( next->right!=next && next->left!=next && next->par->right ) next->par->right->par=next->par;
		}
		else {
			std::cout<<"far away here"<<std::endl;
			next->par->left=(next->right==found)?next->left:next->right;
			if( next->right!=next && next->left!=next && next->par->left) next->par->left->par=next->par;
		}

		if(found->par==0)
			root=next;
		copy_node_ptr(next,found);
		deallocate_node(found);
	}

	void inline deallocate_node(patricia_node *&found)
	{
		node_allocator.destroy(found);
		node_allocator.deallocate(found,1);
	}

	//used by erase(key);
	void inline copy_node_ptr(patricia_node* to, patricia_node* found)
	{
		if(found->par!=0) {
			if(found->par->right==found) 
				found->par->right=to;
			else
				found->par->left=to;
		}
		to->par=found->par;

		if(found->right!=found)  //if found is not connected to itself
			to->right=found->right;
		else
			to->right=to;

		if(found->left!=found)  //if found is not connected to itself
			to->left=found->left;
		else
			to->left=to;
		if ( found->left && found->left->par==found ) found->left->par=to;
		if ( found->right && found->right->par==found ) found->right->par=to;

		to->index=found->index;
	}

	//used by copy_patricia
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

	void copy_patricia(const patricia_node *other_root)
	{
		const patricia_node *const*other_cur,*other_temp,*other_prev;
		patricia_node **cur,*prev;
		if(other_root==0)
		{
			this->root=0;
			return;
		}

		cur=&root;
		std::cout<<"OTHER ROOT:"<<other_root<<std::endl;
		other_cur=&other_root;
		other_prev=0;
		prev=0;
		while(true)
		{
			assert(prev!=other_root);
			if( (*other_cur) && (*other_cur)->par==other_prev)
			{
				(*cur)=node_allocator.allocate(1);
				new(*cur) patricia_node(**other_cur);
				assert((*cur)!=other_root);
				assert(other_root);
				assert((*cur)->right!=other_root);
				(*cur)->par = prev;
				std::cout<<(**cur).value.first<<" "<<(prev==0?"NULL":prev->value.first)<<std::endl;
				other_prev=*other_cur;
				other_cur=&(other_prev->left);
				prev=*cur;
				cur=&(prev->left);
				std::cout<<"copy:here1"<<std::endl;
				continue;
			}
			std::cout<<"copy:here2"<<std::endl;
			(*cur)=copy_patricia_find_par(prev,other_prev,*other_cur);
			assert((*cur)!=other_root);
			//leaf node!!
			if((*other_cur)==other_prev->left)
			{
				//assert(*cur!=other_root);
				other_cur=&(other_prev->right);
				//assert(other_prev!=prev);
				cur=&(prev->right);
				std::cout<<prev->value.first<<std::endl;
				std::cout<<( (*other_cur)?(*other_cur)->value.first:"HOW NULL" )<<std::endl;
				assert((*cur)!=other_root);
			}
			else
			{
				other_temp=other_prev->right;
				while(other_temp==other_prev->right)
				{
					other_temp=other_prev;
					other_prev=other_prev->par;
					prev=prev->par;
					if(other_prev==0) 
					{
						std::cout<<"ROOT:"<<root<<std::endl;
						return;
					}
				}
				other_cur=&other_prev->right;
				cur=&prev->right;
			}
		}
	}
};

}//namespace dsearch
}//namespace boost

#endif //BOOST_DSEARCH_PATRICIA_HPP
