#ifndef BOOST_DSEARCH_PATRICIA_HPP
#define BOOST_DSEARCH_PATRICIA_HPP

#include<algorithm>
#include<memory>
#include<assert.h>

namespace boost{
namespace dsearch{
template<class Key,class Mapped,class Key_traits,class Alloc=allocator<char>>
class patricia{
	private: 
	typedef typename Key_traits::const_iterator key_iterator;
	typedef typename Key_traits::element_type key_element_type;

	enum{
		bit_width=sizeof(Key_traits::element_type)*8
	};
	template<class Key,class Mapped>
	class patricia_node {
		typedef typename patricia_node<Key,Mapped> self;
		public:
		typedef Key key_type;
		typedef Mapped data_type;
		typedef std::pair<key_type,data_type> value_type;
	
		self *par; //avoids the usage of a stack to traverse
		value_type value;
		std::size_t index;
		self *left,*right;//left for zero.. right for one.
	
		patricia_node(): par(0), left(0), right(0) index(0);
		{
		}
	};
	typedef typename Alloc::template rebind<patricia_node>::other node_allocator_type;
	node_allocator_type node_allocator;

	public:
	typedef Key key_type;
	typedef Mapped data_type;
	typedef std::pair<key_type,data_type> value_type;

	
	patricia_node *root;

	patricia(): root(0)
	{
	}
	
	void insert(const value_type &v)
	{
	}

	private:
	inline data_type &insert_new_node(const key_type&key)
	{
		key_iterator it,end_it;
		pat_node *cur=root,*next=root,*temp_node;
		key_element_type temp_element;
		int pos=0;
		std::pair<std::size_t,bool> common;

		it=Key_traits::begin(key);
		end_it=Key_traits::end(key);

		if(root==0)
		{
			root=node_allocator.allocate(1);
			new(root) patricia_node();
			root->index=0;		    //(root->index)%(bit_width+1)==0
			root->value->first=key;
			if(it!=end_it)
				root->left=root;
			else
				root->right=root;  //add to right if the first node added is "";
			return root->value->second;
		}
		//do some jugglery to ensure that inserting "" first wont cause any problems
		//perhaps move root to right position after doing the job.
		//that will take of the assert(next) in find 

		while(it!=end_it)
		{
			if((cur->index)%(bit_width+1)==0)
				next=cur->left;
			else
			{
				temp_element=Key_traits::get_element(it);
				next=get_nth_bit(temp_element,(cur->index)%(bitwidth+1))?
					cur->right:cur->left;
			}

			//fortunately in this case; this should happen only when it==end_it occures.
			assert(next);

			if ( next->index <= cur->index ) break;

			if( pos < (next->index)/(bit_width+1) )
			{
				++pos;
				++it;
			}
			cur=next;
		}
		common=get_common_bits(key,next->value.first);
		if(common.second==2) //key already exists
			return next->value.second;

		assert(common.first);
		it=Key_traits::begin(key);
		cur=root;
		while(it!=end_it)
		{
			if((cur->index)%(bit_width+1)==0)
				next=cur->left;
			else
			{
				temp_element=Key_traits::get_element(it);
				next=get_nth_bit(temp_element,(cur->index)%(bitwidth+1))?
					cur->right:cur->left;
			}

			//fortunately in this case; this should happen only when it==end_it occures.
			assert(next);
			if ( next->index > common.first ) break;  //insert at cur using next->value->first
			if ( next->index <= cur->index ) break;

			if( pos < (next->index)/(bit_width+1) )
			{
				++pos;
				++it;
			}
			cur=next;
		}

		temp_node=node_allocator.allocate(1);
		new (temp_node) patricia_node();
		temp->value.first=key;
		
		//recalculate common;
		common=get_common_bits ( temp->value.first, next->key );
		
		if(cur->left==next) cur->left=temp_node;
		else
			if(cur->right==next) cur->right=temp_node;
		else
			assert(false);

		temp_node->index=common.first+1;
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
		return element && ( 1<<(n-1) );
	}
	
	//returns inclusive of end bit after each pos and the not(un) common bit in k1;
	//returns not common bit as two if both keys are equal.
	inline std::pair<std::size_t,int> get_common_bits(const key_type &k1,const key &k2) const 
	{
		key_iterator k1_it,k2_it,k1_end,k2_end;
		//std::pair<std::size_t,bool> ret_type;
		int uncommon_bit;
		key_element_type t1,t2;
		bool unequal=false;
		std::size_t pos=0;

		k1_it=k1.begin();
		k2_it=k2.begin();
		k1_end=k1.end();
		k2_end=k2.end();

		if(k1_it!=k1_end && k2_it!=k2_end )
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
			t1=Key_traits::get_element(k1_it)
			t2=Key_traits::get_element(k2_it) 
			while(t1%2==t2%2)
			{
				t1/=2;
				t2/=2;
				++pos;
			}
			uncommon_bit=t1%2;
		}
		else
			uncommon_bit=(k1==k1_end)?((k2==k2_end)?2:1):0;

		return make_pair(pos,uncommon_bit);
	}
};


}//namespace dsearch
}//namespace boost

#endif //BOOST_DSEARCH_PATRICIA_HPP

