//////////////////////////////////////////////////////////////////////////////
// assign::cref_list_of2.hpp                               					//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_CREF_LIST_OF2_ER_2010_HPP
#define BOOST_ASSIGN_CREF_LIST_OF2_ER_2010_HPP
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/type_traits.hpp>
#include <boost/ref.hpp>
#include <boost/array.hpp>
#include <boost/range.hpp>
#include <boost/assign/list_of.hpp>

namespace boost{
namespace assign{

    // This is as efficient as Boost.Assign's cref_list_of but more 
    // convenient as the number of items must not be specified in advance
    // Usage : vec = cref_list2_of(a)(b)(c)
    // The idea of this class was developed in collaboration with M.P.G.

namespace cref_list_of2_impl{
            
	typedef boost::mpl::void_ top_;

    template<typename T>
    struct ref{
        typedef boost::assign_detail::assign_reference<T> type;
    };
            
    template<typename T,int N>
    struct ref_array{
        typedef boost::array<typename ref<T>::type,N> type;
    };
            
    template<typename L,typename R,int N>
    struct expr;
            
    template<typename E,typename T,int N>
    struct next{
        typedef expr<E,T,N> expr_;
        typedef expr<expr_,T,N+1> type;
    };
            
    template<typename E,typename T,int N>
    struct expr{
        typedef boost::mpl::int_<N> int_n_;
        typedef boost::mpl::int_<1> int_1_;
        typedef typename boost::mpl::equal_to<int_n_,int_1_>::type is_first_;
                
        typedef typename boost::mpl::if_<is_first_,E,E&>::type previous_;
                
        typedef typename ref<T>::type ref_;
        typedef typename ref_array<T,N>::type ref_array_;

        previous_ previous;
        typedef typename next<E,T,N>::type next_;
        ref_ ref;
                
        // public:
        typedef next_ result_type;
                
        expr(T& t):ref(t){} // only for N == 1
        expr(E& p,T& t):previous(p),ref(t){}
                
        next_ operator()(T& t){ return next_(*this,t); }
                
        template<typename T1>
        operator boost::array<T1,N>(){
            typedef boost::array<T1,N> ar_;
            ar_ ar;
            write_to_array(ar,*this);
            return ar;
        }
                
        template<typename C>
        operator C()
        {
            ref_array_ ref_array;
            write_to_array(ref_array,*this);
            return C(boost::begin(ref_array),boost::end(ref_array));
        }
                
        // -------- as container ---- //
                
        typedef ref_ value_type;
        typedef typename boost::range_iterator<ref_array_>::type iterator;
        typedef typename boost::range_iterator<
        	const ref_array_>::type const_iterator;
        typedef typename boost::range_size<ref_array_>::type size_type;
        typedef typename boost::range_difference<
            ref_array_>::type difference_type;
                
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
                
        // private:
        void alloc(){ 
            this->ptr = smart_ptr_(new ref_array_);
            write_to_array(*ptr,*this);		
        }
                
        void alloc_if(){
            if(!ptr){
                return this->alloc();
            }
        }
                
        typedef boost::shared_ptr<ref_array_> smart_ptr_;
        mutable smart_ptr_ ptr;
                
    };
            
    typedef boost::mpl::bool_<false> false_;
    typedef boost::mpl::bool_<true> true_;
            
    template<typename A,typename E,typename T,int N>
    void write_to_array(A& a,expr<E,T,N>& e){
        typedef expr<E,T,N> expr_;
        typedef typename expr_::is_first_ exit_;
        write_to_array(a,e,exit_());
    }
            
    template<typename A,typename E,typename T,int N>
    void write_to_array(A& a,expr<E,T,N>& e,false_ exit){
        a[N-1] = e.ref;
        write_to_array(a,e.previous);
    }
            
    template<typename A,typename E,typename T,int N>
    void write_to_array(A& a,expr<E,T,N>& e,true_ exit){
        a[N-1] = e.ref;
    }
            
    template<typename T>
    struct first{
        typedef cref_list_of2_impl::expr<
        	cref_list_of2_impl::top_,T,1> type;   
    };
            
            
}// cref_list_of2_impl        
        
    template<typename T>
    typename cref_list_of2_impl::first<const T>::type
    cref_list_of2(const T& t){
        typedef typename cref_list_of2_impl::first<const T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename cref_list_of2_impl::first<T>::type
    ref_list_of2(T& t){
        typedef typename cref_list_of2_impl::first<T>::type expr_;
        return expr_(t);
    }
    
}// assign
}// boost

#endif

