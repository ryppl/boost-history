//////////////////////////////////////////////////////////////////////////////
// assign::cref_list_of.hpp                               					//
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
#include <boost/array.hpp>
#include <boost/range.hpp>
#include <boost/assign/detail/assign_value.hpp>
#include <boost/assign/list_of.hpp> // optionally needed for assign_referene

namespace boost{
namespace assign{

    // This is a variation on Boost.Assign's ref_list_of that does not require
    // the number of items to be specified in advance and uses copy semantics
    // for the assignment operator.
    // Usage 1: 
    // 	std::vector<T> vec = cref_list_of(a)(b)(c);
    // Usage 2: 
    // 	boost::fill( ref_list_of(a)(b)(c) , 0); 
	//
    // For rebind semantics use ref_rebind_list_of(a)(b)(c)
	//    
    // Acknowledgement: The idea of this class was developed in collaboration 
    // with M.P.G

namespace cref_list_of_impl{
            
	typedef boost::mpl::void_ top_;

    template<typename T>
    struct ref_bind{
        typedef boost::assign_detail::assign_reference<T> type;
    };

	// Copy semantics
    template<typename T>
    struct ref_value{
        typedef boost::assign_detail::assign_value<T> type;
    };
            
	// Rebind semantics
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
            
    template<typename E,typename T,int N,template<typename> class Ref = ref_value>
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
            return boost::begin(*this->ptr);
        }
        iterator end() 
        {
            this->alloc_if();
            return boost::end(*this->ptr);
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
                
        typedef boost::shared_ptr<ref_array_> smart_ptr_;
        smart_ptr_ ptr;
                
    };
            
    typedef boost::mpl::bool_<false> false_;
    typedef boost::mpl::bool_<true> true_;
            
    template<typename A,typename E,typename T,int N,template<typename> class Ref>
    void write_to_array(A& a,expr<E,T,N,Ref>& e){
        typedef expr<E,T,N,Ref> expr_;
        typedef typename expr_::is_first_ exit_;
        write_to_array(a,e,exit_());
    }
            
    template<typename A,typename E,typename T,int N,template<typename> class Ref>
    void write_to_array(A& a,expr<E,T,N,Ref>& e,false_ /*exit*/){
        a[N-1] = e.ref;
        write_to_array(a,e.previous);
    }
            
    template<typename A,typename E,typename T,int N,template<typename> class Ref>
    void write_to_array(A& a,expr<E,T,N,Ref>& e,true_ /*exit*/){
        a[N-1] = e.ref;
    }
            
    template<typename T>
    struct first{
        typedef cref_list_of_impl::expr<
        	cref_list_of_impl::top_,T,1> type;   
    };

    template<typename T>
    struct bind_first{
        typedef cref_list_of_impl::expr<
        	cref_list_of_impl::top_,T,1,ref_bind> type;   
    };

            
}// cref_list_of_impl        
        
    template<typename T>
    typename cref_list_of_impl::first<const T>::type
    cref_list_of(const T& t){
        typedef typename cref_list_of_impl::first<const T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename cref_list_of_impl::first<T>::type
    ref_list_of(T& t){
        typedef typename cref_list_of_impl::first<T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename cref_list_of_impl::bind_first<const T>::type
    cref_rebind_list_of(const T& t){
        typedef typename cref_list_of_impl::bind_first<const T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename cref_list_of_impl::bind_first<T>::type
    ref_rebind_list_of(T& t){
        typedef typename cref_list_of_impl::bind_first<T>::type expr_;
        return expr_(t);
    }

    
}// assign
}// boost

#endif
