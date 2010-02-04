//////////////////////////////////////////////////////////////////////////////
// assign::cref_list_of2.cpp                               					//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_CREF_LIST_OF2_ER_2010_HPP
#define BOOST_ASSIGN_CREF_LIST_OF2_ER_2010_HPP
#include <boost/mpl/empty_base.hpp>
//#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/type_traits.hpp>
#include <boost/ref.hpp>
#include <boost/array.hpp>
#include <boost/range.hpp>
#include <boost/assign/list_of.hpp>

namespace boost{
namespace assign{

// Usage : vec = cref_list2_of(a)(b)(c)
// The idea of this class was developed in collaboration with P.M.
// Complexity of cref_list2_of for size N : A total of N reference_wrappers
// are stored. Another N are constructed at conversion.
//
// TODO perhaps the stored ref_ could be used as a private base;
template<typename T,int N, typename B>
class cref_impl : protected B{
    
    typedef boost::assign_detail::assign_reference<const T> ref_;

	typedef cref_impl<T,N,B> this_;

	typedef typename boost::mpl::plus<
    	boost::mpl::int_<N>,
        boost::mpl::int_<1>
    >::type incr_n_;

    template<typename T1>
    struct next{ typedef cref_impl<T1,incr_n_::value,this_> type; };

	typedef typename next<T>::type next_;

	typedef boost::mpl::bool_<false> false_;
	typedef boost::mpl::bool_<true> true_;

	template<typename T1>
    struct array{ typedef boost::array<T1,N+1> type; };

	// because reference_wrapper has no default constructor, assign_reference
    // is needed
	template<typename T1> 
    struct ref_array 
    	: array<boost::assign_detail::assign_reference<const T1> >{};
        
    typedef typename array<T>::type array_;
    typedef typename ref_array<T>::type ref_array_;

	typedef boost::shared_ptr<ref_array_> smart_ptr_;
    
	typedef typename boost::is_same<
    	boost::mpl::int_<N>,
    	boost::mpl::int_<0>
    >::type exit_impl_;

	protected:

	typedef boost::mpl::bool_<exit_impl_::value> exit_; 

	public:
	typedef next_ result_type;

    typedef ref_ 								  value_type;
    typedef typename boost::range_iterator<
    	ref_array_
    >::type iterator;
    typedef typename boost::range_iterator<
    	const ref_array_
    >::type const_iterator;
    typedef typename boost::range_size<
    	ref_array_
    >::type size_type;
    typedef typename boost::range_difference<
    	ref_array_
    >::type difference_type;

    iterator begin()
    {
        this->alloc_if();
        return boost::begin(*ptr);
    }

    iterator end() 
    {
        this->alloc_if();
        return boost::end(*ptr);
    }
    
    
    size_type size() const
    {
        return ref_array_::size();
    }
    
    bool empty() const
    {
        return !(this->size());
    }
        
	cref_impl(const T& t):ref(t){} 

	cref_impl(const B& b,const T& t):B(b),ref(t){}
        
	next_ operator()(const T& t)const{
        return next_(*this,t);
    }

	template<typename T1>
	operator typename array<T1>::type(){
		typedef typename array<T1>::type ar_;
        ar_ ar;
        this->write_to_array(ar,exit_());
        return ar;
    }
    
	// Requirement: C(begin,end) is a valid constructor
	template<typename C>
	operator C(){
    	// TODO consider instead:
        // C c; c.reserve(N+1)
        // and recursively calling c.push_front(ref)
    
    	typedef typename boost::range_value<C>::type val_;
        ref_array_ ar; 
        this->write_to_array(ar,exit_());
        return C(boost::begin(ar),boost::end(ar));
    }
    
	protected:
    
	template<typename A>
	void write_to_array(A& ar,true_)const{
        ar[N] = this->ref;
    }

	template<typename A>
	void write_to_array(A& ar,false_)const{
        ar[N] = this->ref;
		return forward(*this,ar);
    }

	void alloc(){ 
    	this->ptr = smart_ptr_(new ref_array_);
		this->write_to_array(*ptr,exit_());		
    }

	void alloc_if(){
    	if(!ptr){
        	return this->alloc();
        }
    }

    mutable smart_ptr_ ptr;

	private:
    
	template<typename A>
    static void forward(const B& b,A& ar){
    	typedef typename B::exit_ exit_;
        return b.write_to_array(ar,exit_());
    }
    
	ref_ ref;
};

template<typename T>
cref_impl<T,0,boost::mpl::empty_base>
cref_list_of2(const T& t){
	typedef cref_impl<T,0,boost::mpl::empty_base> res_;
	return res_(t);
}

}// assign
}// boost
             
#endif
