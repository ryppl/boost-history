//////////////////////////////////////////////////////////////////////////////
// assign::detail::array_interface.hpp                                      //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_INTERFACE_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_INTERFACE_ER_2010_HPP
#include <algorithm>
#include <boost/mpl/bool.hpp>
#include <boost/array.hpp>
#include <boost/range.hpp>
#include <boost/assign/auto_size/detail/has_copy_semantics.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
                   
    template<typename T,int N,template<typename> class Ref>
    struct ref_array{
        typedef boost::array<typename Ref<T>::type,N> type;
    };

	// Requirements:
    // D must implement:
    // 	const ref_array_& ref_array_impl()const
    // 	ref_array& ref_array_impl()
    template<typename T,int N,template<typename> class Ref,typename D>
    struct array_interface{
        typedef typename Ref<T>::type ref_;
        typedef typename ref_array<T,N,Ref>::type ref_array_;

        typedef ref_ value_type;
        typedef typename 
        	boost::range_iterator<ref_array_>::type iterator;
        typedef typename boost::range_iterator<
        	const ref_array_>::type const_iterator;
        typedef typename 
        	boost::range_size<ref_array_>::type size_type;
        typedef typename boost::range_difference<
            ref_array_>::type difference_type;
                
        iterator begin()
        {
            return boost::begin(this->ref_array());
        }
        iterator end() 
        {
            return boost::end(this->ref_array());
        }

        const_iterator begin()const
        {
            return boost::begin(this->ref_array());
        }
        const_iterator end()const 
        {
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
                
        typedef typename ref_array_::reference reference;
        typedef typename 
        	ref_array_::const_reference const_reference;

        reference operator[](size_type i){ return (this->ref_array())[i]; }
        const_reference operator[](size_type i)const{ 
             return (this->array())[i]; }
			
        reference front(){ return (this->ref_array()).front(); }
        const_reference front() const{ return (this->ref_array()).front(); }
        reference back(){ return (this->ref_array()).back(); }
        const_reference back() const{ return (this->ref_array()).back(); }
        
        void swap(ref_array_& other){ return (this->ref_array()).swap(other); }
        void assign(const T& val){ 
        	typedef has_copy_semantics<ref_> pred_;
            return this->assign(val,pred_());
        }

        template<typename T1>
        operator boost::array<T1,N>()const{
            boost::array<T1,N> ar;
            std::copy(	
            	boost::begin(this->ref_array()),
            	boost::end(this->ref_array()),
                boost::begin(ar)
            );
            return ar;
        }

        template<typename C>
        operator C ()const
        {
            return C(
            	boost::begin(this->ref_array()),
                boost::end(this->ref_array())
            );
        }

        private:
		typedef boost::mpl::bool_<false> false_;
		typedef boost::mpl::bool_<true> true_;

        void assign(const T& val,true_ /*copy semantics*/){ 
        	// Force copy semantics. Suggested by M.P.G on Feb 28th, 2010.
           	ref_array_& ra = this->ref_array();
           	std::fill(ra.begin(), ra.end(), val);
        }

        void assign(const T& val,false_ /*copy semantics*/){ 
           	return this->ref_array().assign(val);
        }
		
        ref_array_& ref_array(){ 
			return static_cast<D&>(*this).ref_array_impl();
        }

        const ref_array_& ref_array()const{ 
			return static_cast<const D&>(*this).ref_array_impl();
        }

    };

}// auto_size  
}// detail      
}// assign
}// boost

#endif
