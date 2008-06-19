#ifndef BOOST_DSEARCH_PATRICIA_ITERATOR_HPP
#define BOOST_DSEARCH_PATRICIA_ITERATOR_HPP
#include<boost/iterator/iterator_facade.hpp>
#include<boost/utility/enable_if.hpp>
#include<boost/type_traits/is_convertible.hpp>

namespace boost{
namespace dsearch{
template<class Value_type, class Node_type, class Alloc>
class patricia_iterator
:public iterator_facade< patricia_iterator<Value_type,Node_type, Alloc>, 
Value_type, bidirectional_traversal_tag >
{
	protected:
	struct enabler {};
	friend class boost::iterator_core_access;
	Node_type *cur;
	Node_type *next;
	
	void increment()
	{
		if (cur == 0 ) return;
		while ( cur && ( cur->right==next || cur->right==0) )
		{
			next=cur;
			cur=cur->par;
		}
		if ( cur == 0) return; //reached end()
		next=cur->right; //cur->right!=0
		while ( next && next->index > cur->index )
		{
			cur=next;
			next=cur->left;
			if ( next == 0 ) next=cur->right;
		}
	}

	void decrement()
	{
		if(cur)
		{
			while(cur && ( cur->left==next || cur->left==0 ) )
			{
				next=cur;
				cur=cur->par;
			}
			if(cur==0)
				return;
			next=cur->left;
		}
		else
		{
			cur=next;
			next=(cur->right==0)?cur->left:cur->right;
		}

		while ( next && next->index > cur->index )
		{
			cur=next;
			next=cur->right;
			if ( next == 0 ) next=cur->left;
		}
	}
	
	Value_type &dereference() const
	{
		assert(cur); //dont reference null;
		return next->value;
	}
	
	template<class V,class N,class A>
	bool equal(patricia_iterator<V,N,A> const &other,
	typename enable_if< is_convertible<V*,Value_type*>, 
				enabler >::type = enabler()
	) const
	{
		
		if ( cur == other.cur && next == other.next )
			return true;
		//std::cout<<"In equal"<<next->value.first << "!=" << other.next->value.first<<std::endl;
		return false;
	}

	public:
	patricia_iterator() : cur(0),next(0)
	{
	}

	patricia_iterator(Node_type *root, bool start)
	{
		if(start)
		{
			if(root==0) 
			{
				cur=0;
				root=0;
				return;
			}
			cur=root;
			next= (cur->left)?cur->left:cur->right;
			assert(next!=0);
			while ( cur->index < next->index )
			{
				cur=next;
				next=cur->left?cur->left:cur->right;
			}
		}
		else
		{
			next=root;
			cur=0;
		}
		return;
	}
	
	template<class V,class N,class A>
	patricia_iterator ( patricia_iterator<V,N,A> const& other,
				typename enable_if< is_convertible<V*,Value_type*>, 
				enabler >::type = enabler()
		    		) : cur(other.cur),next(other.next)
	{
	}
	
	patricia_iterator (Node_type* found,Node_type *prev):
	cur(prev),next(found)
	{
	}
};

template<class Value_type, class Node_type, class Alloc >
class patricia_reverse_iterator:
public patricia_iterator<Value_type, Node_type, Alloc>
{
	typedef patricia_iterator<Value_type, Node_type, Alloc> forward_type;
	private:
	struct enabler {};
	void decrement()
	{
		forward_type::increment();
	}
	
	void increment()
	{
		forward_type::decrement();
	}
	
	template<class V,class N,class A>
	bool equal(patricia_reverse_iterator<V,N,A> const &other,
	typename enable_if< is_convertible<V*,Value_type*>, 
				enabler >::type = enabler()
	) const
	{
		if ( forward_type::cur == other.cur && forward_type::next == other.next )
			return true;
		return false;
	}
	
	Value_type &dereference() const
	{
		return forward_type::dereference();
	}
	public:
	patricia_reverse_iterator(): forward_type()
	{
	}
	patricia_reverse_iterator(Node_type *found,Node_type*prev)
	: forward_type(found,prev)
	{
	}
	
	patricia_reverse_iterator(Node_type *root,bool start) 
	{
		forward_type::cur=0;
		forward_type::next=root;
		if(start)
		{
			decrement();
		}
	}
	template<class V,class N,class A>
	patricia_reverse_iterator ( patricia_reverse_iterator<V,N,A> const& other,
				typename enable_if< is_convertible<V*,Value_type*>, 
				enabler >::type = enabler()
		    		) 
	: forward_type(other.next,other.cur)
	{
	}
};

}
}
#endif //BOOST_DSEARCH_PATRICIA_ITERATOR_HPP
