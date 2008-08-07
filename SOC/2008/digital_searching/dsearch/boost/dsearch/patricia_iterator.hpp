#ifndef BOOST_DSEARCH_PATRICIA_ITERATOR_HPP
#define BOOST_DSEARCH_PATRICIA_ITERATOR_HPP
#include<boost/iterator/iterator_facade.hpp>
#include<boost/utility/enable_if.hpp>
#include<boost/type_traits/is_convertible.hpp>

namespace boost{
namespace dsearch{

///iterator for patricia
/**
 \param Value_type is patricia::value_type 
 \param Node_type is patricia::patricia_node
 \param Alloc is the allocator
 */
template<class Value_type, class Node_type, class Alloc>
class patricia_iterator
:public iterator_facade< patricia_iterator<Value_type,Node_type, Alloc>, 
Value_type, bidirectional_traversal_tag >
{
	template<class K,class M,class K_t,class A > friend class patricia;
	protected:
	///empty struct used for avoiding conversion from const_iterator to iterator
	struct enabler {};
	friend class boost::iterator_core_access;
	///Pointer to prev node . usually cur->left or cur->right ==next
	/// if cur ! = NULL.  if cur==NULL then next==root.
	Node_type *cur;
	///Node to which the iterator points to. 
	Node_type *next;
	
	///increments the iterator.
	virtual void increment()
	{
		if (cur)
		{
			//*move up if cur->right===next
			while ( cur && ( cur->right==next || cur->right==0) )
			{
				next=cur;
				cur=cur->par;
			}
			if ( cur == 0) return; //* reached end()
			next=cur->right; //* also cur->right!=0
		}
		else
		{
			//* next == root and now we are moving to the begin()
			cur=next;
			next=(cur->left)?cur->left:cur->right;
		}

		//* go down the least element in the edge
		while ( next && next->index > cur->index )
		{
			cur=next;
			next=cur->left?cur->left:cur->right;
		}
	}
	
	///decrements the iterator.
	virtual void decrement()
	{
		if(cur)
		{
			//*move up if cur->left===next
			while(cur && ( cur->left==next || cur->left==0 ) )
			{
				next=cur;
				cur=cur->par;
			}
			if(cur==0)
				//* reached before begin()
				return;
			//* also cur->left!=0
			next=cur->left;
		}
		else
		{
			//* next == root and now we are moving to the end() - 1
			cur=next;
			next=(cur->right==0)?cur->left:cur->right;
		}

		//* go down the greatest element in the edge
		while ( next && next->index > cur->index )
		{
			cur=next;
			next=cur->right?cur->right:next=cur->left;
		}
	}
	
	///dereferences the iterator.
	Value_type &dereference() const
	{
		assert(cur); //dont reference null;
		return next->value;
	}
	
	///check whether two iterator are equal.
	/**
	 	\returns true if the iterator are equal otherwise false.
	 	\param other is other iterator which is supposed to compared to this.
	 */
	template<class V,class N,class A>
	bool equal(patricia_iterator<V,N,A> const &other) const
	{
	#ifdef PAT_DEBUG
		std::cout<<cur << " " << other.cur << " " << next  << 
		" "<< other.next << std::endl;
	#endif
		if ( cur == other.cur && next == other.next )
			return true;
		return false;
	}

	private:
	///move to the right most node of the edge cur<-->next
	void to_right_most()
	{
		assert(cur);
		while(next->par==cur)
		{
			cur=next;
			next=cur->right?cur->right:cur->left;
		}
	}
	
	private:
	///move to the left most node of the edge cur<-->next
	void to_left_most()
	{
		assert(cur);
		while(next->par==cur)
		{
			cur=next;
			next=cur->left?cur->left:cur->right;
			//std::cout<<"To left most"<<cur->value.first<<"<-"<<next->value.first<<std::endl;
		}
	#ifdef PAT_DEBUG
		std::cout<<"To left most"<<cur->value.first<<"<-"<<next->value.first<<std::endl;
	#endif
	}

	public:
	///default constructor
	patricia_iterator() : cur(0),next(0)
	{
	}

	///use to intialize begin or end.
	/**
	 	\param root is the patricia's root node
	 	\param start specifies start or end
	 */
	patricia_iterator(Node_type *root, bool start)
	{
		if(start)
		{
			if(root==0) 
			{
				std::cout<<"root is zero"<<std::endl;
				cur=0;
				next=0;
				return;
			}
			//*move to the left most wrt root.
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

	///copy constructor for the iterator
	/**
	 \brief is_covertible ensures that const_iterator cannot be converted 
	 to iterator
	 \param other is the iterator to be copied
	 */
	template<class V,class N,class A>
	patricia_iterator ( patricia_iterator<V,N,A> const& other,
				typename enable_if< is_convertible<V*,Value_type*>, 
				enabler >::type = enabler()
		    		) : cur(other.cur),next(other.next)
	{
	}
	
	///initialize the iterator by specifying the edge using found and prev.
	/**
	 \param found is the node whihc the iterator points to
	 \param prev  is node such that prev->left==found or prev->right==found
	 */ 
	patricia_iterator (Node_type* found,Node_type *prev):
	cur(prev),next(found)
	{
	}
};

///reverse iterator for the patricia node
/**
 \param Value_type is patricia::value_type 
 \param Node_type is patricia::patricia_node
 \param Alloc is the allocator
 */
template<class Value_type, class Node_type, class Alloc >
class patricia_reverse_iterator:
public patricia_iterator<Value_type, Node_type, Alloc>
{
	friend class patricia_iterator<Value_type, Node_type, Alloc>;
	///the patricia_iterator super class
	typedef patricia_iterator<Value_type, Node_type, Alloc> forward_type;
	private:
	///empty struct used for conversion from const_iterator to iterator
	struct enabler {};
	
	///decrements to reverse_iterator
	void decrement()
	{
		forward_type::increment();
	}
	
	///increments the reverse_iterator
	void increment()
	{
		forward_type::decrement();
	}

	///check whether two reverse iterator are equal.
	/**
	 	\returns true if the reverse iterator are equal otherwise false.
	 	\param other is other reverse iterator which is supposed to compared to this.
	 */
	template<class V,class N,class A>
	bool equal(patricia_reverse_iterator<V,N,A> const &other) const
	{
		if ( forward_type::cur == other.cur && forward_type::next == other.next )
			return true;
		
		return false;
	}
	
	///dereferences the iterator
	Value_type &dereference() const
	{
		return forward_type::dereference();
	}

	public:
	///default constructor
	patricia_reverse_iterator(): forward_type()
	{
	}

	///initialize the iterator by specifying the edge using found and prev.
	/**
	 \param found is the node whihc the iterator points to
	 \param prev  is node such that prev->left==found or prev->right==found
	 */ 
	patricia_reverse_iterator(Node_type *found,Node_type*prev)
	:forward_type(found,prev)
	{
	}

	///use to intialize begin or end.
	/**
	 	\param root is the patricia's root node
	 	\param start specifies start or end
	 */
	patricia_reverse_iterator(Node_type *root,bool start) : forward_type(root,false)
	{
		if(start && root!=0)
			forward_type::decrement();
	}
	///copy constructor for reverse the iterator
	/**
	 \brief is_covertible ensures that const_reverse_iterator cannot be converted 
	 to reverse_iterator
	 \param other is the iterator to be copied
	 */
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
