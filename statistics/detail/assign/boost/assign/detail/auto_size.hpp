//////////////////////////////////////////////////////////////////////////////
// assign::detail::ref_list_of_auto_size_copy_rebind.hpp                    //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_REF_LIST_OF_AUTO_SIZE_COPY_REBIND_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_REF_LIST_OF_AUTO_SIZE_COPY_REBIND_ER_2010_HPP
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>
#include <boost/array.hpp>
#include <boost/range.hpp>
#include <boost/assign/list_of.hpp> // needed for assign_referene
#include <boost/assign/detail/assign_value.hpp>

// This is the implementation behind ref_list_of() and ref_copy_list_of().
//
// The motivation for this class was to improve upon ref_list_of<int>(), by 
// deducing the number of arguments at compile time, and allowing for either 
// of copy or rebind semantics, in case it is used as the lhs of an assignment; 
// the former (copy) being both safer and more common than the latter (rebind).
//    
// Acknowledgement: The idea of this class was developed in collaboration 
// with M.P.G
//
// Revision history:
// Feb 9, 2010 : 
// 	- Added copy semantics.
// 	- The temporary array in the conversion operator is now assigned by calling 
// 	begin() and end() rather than write_to_array() to ensure consistency of 
//  side effect when assigning under rebind semantics. Negligible loss in 
//	performance
// Feb 5, 2010 : First version. rebind semantics.

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
            
	typedef boost::mpl::void_ top_;

	// Rebind semantics
    template<typename T>
    struct ref_bind{
        typedef boost::assign_detail::assign_reference<T> type;
    };

	// Copy semantics
    template<typename T>
    struct ref_value{
        typedef boost::assign_detail::assign_reference_copy<T> type;
    };
            
    template<typename T,int N,template<typename> class Ref>
    struct ref_array{
        typedef boost::array<typename Ref<T>::type,N> type;
    };
            
    template<typename L,typename T,int N,template<typename> class Ref>
    struct expr;
            
    template<typename E,typename T,int N,template<typename> class Ref>
    struct next{
        typedef expr<E,T,N,Ref> expr_;
        typedef expr<expr_,T,N+1,Ref> type;
    };
            
    template<typename E,typename T,int N,template<typename> class Ref>
    class expr{
        typedef boost::mpl::int_<N> int_n_;
        typedef boost::mpl::int_<1> int_1_;
        typedef typename Ref<T>::type ref_;

		public:       
        typedef typename boost::mpl::equal_to<int_n_,int_1_>::type is_first_;
        typedef typename boost::mpl::if_<is_first_,E,E&>::type previous_;
        typedef typename ref_array<T,N,Ref>::type ref_array_;
        typedef typename next<E,T,N,Ref>::type next_;

        previous_ previous;
        ref_ ref;
                                
        expr(T& t):ref(t){} // only for N == 1
        expr(E& p,T& t):previous(p),ref(t){}
                
        typedef next_ result_type;
        next_ operator()(T& t){ return next_(*this,t); }
                
        template<typename T1>
        operator boost::array<T1,N>(){
        	boost::array<T1,N> ar;
			std::copy(	
            	boost::begin(this->ref_array()),
            	boost::end(this->ref_array()),
                boost::begin(ar)
            );
            return ar;
        }
                
        template<typename C>
        operator C()
        {
            return C(
            	boost::begin(this->ref_array()),
                boost::end(this->ref_array())
            );
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
            return boost::begin(this->ref_array());
        }
        iterator end() 
        {
            this->alloc_if();
            return boost::end(this->ref_array());
        }
        size_type size() const
        {
            return ref_array_::size();
        }
        bool empty() const
        {
            return !(this->size());
        }
                
        private:
        void alloc(){ 
            this->ptr = smart_ptr_(new ref_array_);
            write_to_array(*this->ptr,*this);		
        }
                
        void alloc_if(){
            if(!this->ptr){
                return this->alloc();
            }
        }

		ref_array_& ref_array(){ 
        	this->alloc_if();
            return (*this->ptr);
        }
                
        typedef boost::shared_ptr<ref_array_> smart_ptr_;
        smart_ptr_ ptr;
                
    };
            
    typedef boost::mpl::bool_<false> false_;
    typedef boost::mpl::bool_<true> true_;
            
    template<typename A,typename E,typename T,int N,
    	template<typename> class Ref>
    void write_to_array(A& a,expr<E,T,N,Ref>& e){
        typedef expr<E,T,N,Ref> expr_;
        typedef typename expr_::is_first_ exit_;
        write_to_array(a,e,exit_());
    }
            
    template<typename A,typename E,typename T,int N,
    	template<typename> class Ref>
    void write_to_array(A& a,expr<E,T,N,Ref>& e,false_ /*exit*/){
        a[N-1] = e.ref;
        write_to_array(a,e.previous);
    }
            
    template<typename A,typename E,typename T,int N,
    	template<typename> class Ref>
    void write_to_array(A& a,expr<E,T,N,Ref>& e,true_ /*exit*/){
        a[N-1] = e.ref;
    }
            
    template<typename T>
    struct copy_first{
        typedef detail::auto_size::expr<
        	detail::auto_size::top_,T,1,ref_value> type;   
    };

    template<typename T>
    struct rebind_first{
        typedef detail::auto_size::expr<
        	detail::auto_size::top_,T,1,ref_bind> type;   
    };
            
}// auto_size  
}// detail      
}// assign
}// boost

#endif

