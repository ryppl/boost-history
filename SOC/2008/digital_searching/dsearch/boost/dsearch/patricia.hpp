#ifndef BOOST_DSEARCH_PATRICIA_HPP
#define BOOST_DSEARCH_PATRICIA_HPP

#include<algorithm>
#include<memory>
#include<iostream>
#include<assert.h>

namespace boost{
namespace dsearch{
template<class Key,class Mapped,class Key_traits,class Alloc=std::allocator<char> >
class patricia{
	private: 
	typedef typename Key_traits::const_iterator key_iterator;
	typedef typename Key_traits::element_type key_element_type;

	enum{
		bit_width=sizeof(typename Key_traits::element_type)*8
	};

	public:
	typedef Key key_type;
	typedef Mapped data_type;
	typedef std::pair<key_type,data_type> value_type;

	private:
	//template<class Key,class Mapped>
	class patricia_node {
		typedef patricia_node self;
		public:
		//typedef Key key_type;
		//typedef Mapped data_type;
		typedef std::pair<key_type,data_type> value_type;
	
		self *par; //avoids the usage of a stack to traverse
		value_type value;
		std::size_t index;
		self *left,*right;//left for zero.. right for one.
	
		patricia_node(): par(0), left(0), right(0), index(0)
		{
		}
	};
	typedef typename Alloc::template rebind<patricia_node>::other node_allocator_type;
	node_allocator_type node_allocator;

	patricia_node *root;

	public:

	patricia(): root(0)
	{
	}
	
	void insert(const value_type &v)
	{
		insert_new_node(v.first)=v.second;
	}
		
	bool find(const key_type &k)
	{
		std::pair<std::size_t,int> common;
		patricia_node *node;
		node=find_node(k);
		if(node==0) return 0;
		common=get_common_bits(node->value.first,k);
		return (common.second==2);
		return true;
	}

	private:
	inline data_type &insert_new_node(const key_type&key)
	{
		key_iterator it,end_it;
		patricia_node *cur=root,*next=root,*temp_node,*old_root=0;
		key_element_type temp_element;
		std::size_t pos=0;
		std::pair<std::size_t,int> common;

		//std::cout<<"\nInserting \""<<key<<"\""<<std::endl;

		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);
	
		//do some jugglery to ensure that inserting "" first as root wont cause any problems
		//move root to the right position while inserting the second
		//that will take care of the assert(next) in find 
		old_root=0;
		if(root!=0 && root->right==root)
		{
			if(it==end_it) return root->value.second;
			old_root=root;
			root=0;
		}

		if(root==0)
		{
			root=node_allocator.allocate(1);
			new(root) patricia_node();
			root->index=0;		   //(root->index)%(bit_width+1)==0
			root->value.first=key;

			if(it!=end_it)
			{
				root->left=root;
				root->right=old_root;
			}
			else
				root->right=root;  //add to right if the first node added is "";
		
			return root->value.second;
		}
		if(it==end_it)  //"" inserted
		{
			if(root->right!=0) 
				next=root->right;
			else
			{
				root->right = node_allocator.allocate(1);
				new(root->right) patricia_node();
				root->right->par=root;
				root->right->index=0;
				root->right->right=root->right;
				root->right->value.first=key;
				return root->right->value.second;
			}
		}

		next=find_node(key);
		
		//find the largerst prefix matching the key and the found key
		//std::cout<<"After find"<<std::endl;
		common=get_common_bits(key,next->value.first);
		
		//key already exists
		if(common.second==2) 
		{
			//std::cout<<"The key \""<<key<<"\" already exists"<<std::endl;
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
				next=cur->left;
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
		new (temp_node) patricia_node();
		temp_node->value.first=key;
		
		//recalculate common;
		//std::cout<<"After second find"<<std::endl;
		common=get_common_bits ( temp_node->value.first, next->value.first );

		//find where next was and insert the new node there		
		if(cur->left==next) cur->left=temp_node;
		else
			if(cur->right==next) cur->right=temp_node;
		else
			assert(false);

		assert(common.second!=2);
		//std::cout<<"common: "<<common.first<<" uncommon "<<common.second<<std::endl;
		temp_node->index=common.first;
		temp_node->par=cur;
		
		if(common.second) //temp_node should point to inself at 1 ie right
		{
			temp_node->right=temp_node;
			temp_node->left=next;
		}
		else
		{
			temp_node->right=next;
			temp_node->left=temp_node;
		}

		return temp_node->value.second;
	}

	inline std::size_t get_nth_bit(const key_element_type &element,const int &n) const 
	{
		return n==0? 0 : ( element & (1<<(n-1)) );
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
			while(t1%2==t2%2)
			{
				t1/=2;
				t2/=2;
				++pos;
			}
			uncommon_bit=t1%2;
			++pos;
		}
		else
			uncommon_bit=(k1_it==k1_end)?((k2_it==k2_end)?2:1):0;

		//we dont know anything yet about the last bit. ie len+1 th bit
		//if(pos%(bit_width+1)!=0) pos++;	

		return std::make_pair<std::size_t,int>(pos,uncommon_bit);
	}

	patricia_node* find_node(const key_type &key)
	{
		patricia_node *cur=root,*next;
		std::size_t pos=0;
		key_iterator it,end_it;
		key_element_type temp_element;

		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);

		if(root==0) return 0;
		if(it==end_it)
			return root->right;

		while(it!=end_it)
		{
			if ( (cur->index)%(bit_width+1)==0 )
				next=cur->left;
			else
			{
				temp_element=Key_traits::get_element(it);
				//std::cout<<"getting "<<(cur->index)%(bit_width+1)<<"th bit of "<<temp_element<<std::endl;
				next=get_nth_bit ( temp_element , (cur->index)%(bit_width+1) ) ?
					cur->right : cur->left;
			}
			if(next==0) return next;
			//std::cout<<(cur->left==next?"going left":"going right")<<std::endl;
			//std::cout<<"next: "<<next->value.first<<std::endl;

			if ( next->index <= cur->index ) break;

			while ( pos < (next->index)/(bit_width+1) && it!=end_it )
			{
				++pos;
				++it;
			}
			cur=next;
		}
		if ( it == end_it && pos==(cur->index)/(bit_width+1) )
		{
//			cur=next;
			next=cur->right;
		}
		return next;
	}
};


}//namespace dsearch
}//namespace boost

#endif //BOOST_DSEARCH_PATRICIA_HPP

