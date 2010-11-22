//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_ANON_CONT_ER_2010_HPP
#define BOOST_ASSIGN_V2_ANON_CONT_ER_2010_HPP
#include <deque>
#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/range/size_type.hpp>
#include <boost/range/iterator_range.hpp> //iterator_range_detail::
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/put/range/range.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>
#include <boost/assign/v2/put/generic/crtp.hpp>
#include <boost/assign/v2/detail/relational_op/relational_op.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace anon_aux{

    template<typename T,typename F,typename Tag> class cont;

}// anon_aux
namespace result_of_modulo{

    template<typename T,typename F,typename Tag>
    struct new_fun<anon_aux::cont<T,F,Tag> >
    {

    	template<typename F1>
        struct apply{ typedef anon_aux::cont<T, F1, Tag> type; };

    };

    template<typename T,typename F,typename Tag>
    struct new_modifier<anon_aux::cont<T,F,Tag> >
    {

    	template<typename NewTag>
        struct apply{ typedef anon_aux::cont<T, F, NewTag> type; };

    };

}//result_of_modulo
namespace anon_aux{

    template<typename T>
    struct impl{ typedef std::deque<T> type; };

    template<typename T,typename F,typename Tag>
    struct cont_modifier_traits
    { };


    template<typename T,typename F,typename Tag>
    class cont :
    	public relational_op_aux::crtp< cont<T,F,Tag> >,
        public put_aux::crtp<
        	typename anon_aux::impl<T>::type, F, Tag,
            cont<T,F,Tag>,
            cont_modifier_traits<T,F,Tag>
        >
    {
        typedef typename anon_aux::impl<T>::type impl_;
        typedef cont_modifier_traits<T,F,Tag> modifier_traits_;
        typedef put_aux::crtp<
            impl_, F, Tag, cont, modifier_traits_> put_crtp_;

        typedef put_aux::modifier<Tag> modifier_;

    	public:

    	typedef T value_type;
        typedef typename boost::range_size<impl_>::type size_type;
        typedef typename boost::range_iterator<impl_>::type iterator;
        typedef typename boost::range_iterator<
        	const impl_>::type const_iterator;

        // Construct
        cont(){}
        explicit cont(const F& f) : put_crtp_( f ){}
        explicit cont(impl_ const& v, F const& f): put_crtp_( f ), impl( v )
        {
        	// Required by crtp when Tag or F is modified.
        }

        explicit cont( impl_ const& v, F const& f, modifier_ const& m )
            : put_crtp_( f, m ), impl( v )
        {
        	// Required by crtp when Tag or F is modified.
        }

        // Deque interface
        iterator begin(){
            return boost::begin( this->impl );
        }
        iterator end(){
            return boost::end( this->impl );
        }
        const_iterator begin()const{
            return boost::begin( this->impl );
        }
        const_iterator end()const{
        	return boost::end( this->impl );
        }

        typedef typename impl_::reference reference;
        typedef typename impl_::const_reference const_reference;
        typedef typename impl_::difference_type difference_type;

        size_type size()const{
            return this->unwrap().size();
        }
        size_type max_size()const{
            return this->unwrap().max_size();
        }
        bool empty()const{
            return this->unwrap().empty();
        }
        reference operator[](size_type n){
            return this->unwrap()[n];
        }
        const_reference operator[](size_type n)const{
            return this->unwrap()[n];
        }
        reference front(){
            return this->unwrap().front();
        }
        const_reference front()const{
            return this->unwrap().front();
        }
		reference back(){
        	return this->unwrap().back();
        }
        const_reference back()const{
            return this->unwrap().back();
        }
        void pop_front(){
            this->unwrap().pop_front();
        }
        void pop_back(){
            this->unwrap().pop_back();
        }
        void swap(cont& that){
            this->unwrap().swap( that.unwrap() );
        }

        // Note : the modifiers such as push_back() are ommitted as they
        // accessible through the put interface.

        // Convert

        BOOST_ASSIGN_V2_CONVERT_CONVERT_MF
        BOOST_ASSIGN_V2_CONVERT_OPERATOR_MF

       impl_& unwrap()const{ return this->impl; }

        // Relational op

        template<typename R>
        bool equal_to(const R& r)const{
            return ::boost::iterator_range_detail::equal(
            (*this), r );
        }

        template<typename R>
        bool less_than(const R& r)const{
            return ::boost::iterator_range_detail::less_than(
            (*this), r );
        }

        protected:
        mutable impl_ impl;

    };

}// anon_aux
}// v2
}// assign
}// boost

#endif
