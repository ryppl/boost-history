#ifndef BOOST_DSEARCH_PATRICIA_ITERATOR_HPP
#define BOOST_DSEARCH_PATRICIA_ITERATOR_HPP
#include<boost/iterator/iterator_facade.hpp>

namespace boost{
namespace dsearch{
template<class Value_type, class Node_type, class Alloc=std::allocator<char> >
class patricia_iterator
:public iterator_facade< patricia_iterator<Value_type,Node_type, Alloc>, 
Value_type, bidirectional_traversal_tag >
{
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
		while(cur && ( cur->left==next || cur->left==0 ) )
		{
			next=cur;
			cur=cur->par;
		}

		if(cur)
			next=cur->left;
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
	
	template<class T>
	bool equal(T &other ) const
	{
		if ( cur == other.cur && next == other.next )
			return true;
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
};
	
}
}
#endif //BOOST_DSEARCH_PATRICIA_ITERATOR_HPP
