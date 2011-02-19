//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Manuel Peinado Gallego                               //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_CRTP_ER_MPG_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_CRTP_ER_MPG_2010_HPP
#include <iosfwd>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/call_traits.hpp>
#include <boost/operators.hpp>

// This design is an outgrowth of assign_detail::assign_reference<> by TO. 

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
    
    template<typename D, typename T>
    class wrapper_crtp 
    : 
        boost::less_than_comparable1< 
            wrapper_crtp<D,T>, 
            boost::less_than_comparable2<
                wrapper_crtp<D,T>,
                T,
                boost::equality_comparable1<
                    wrapper_crtp<D,T>,
                    boost::equality_comparable2<
                        wrapper_crtp<D,T>,
                        T
                    >
                >
            >
        >
    {

        typedef typename boost::remove_const<T>::type lvalue_;
        typedef typename boost::add_const<T>::type rvalue_;

        public: 
        // protected

        D& derived(){ return static_cast<D&>( *this ); }
        D const & derived()const{ return static_cast<D const&>( *this ); }

        public:

        void operator=(lvalue_& r ){ this->derived().assign( r ); }
        void operator=(rvalue_& r ){ this->derived().assign( r ); }

        void swap( wrapper_crtp& r )
        {
            return this->derived().swap( r );
        }

        operator T&() const
        {
            return this->derived().get();
        }

        bool operator==( const wrapper_crtp& r )const
        {
            return this->derived().get() == r.derived().get();
        }

        bool operator<( const wrapper_crtp& r )const
        {
            return this->derived().get() < r.derived().get();
        }

        bool operator==( typename boost::call_traits<T>::param_type r )const
        {
            return this->derived().get() == r;
        }

        bool operator<( typename boost::call_traits<T>::param_type r )const
        {
            return this->derived().get() < r;
        }

        bool operator>( typename boost::call_traits<T>::param_type r )const
        {
            return this->derived().get() > r;
        }
        
        template<class CharT, class Traits>
        friend std::basic_ostream<CharT,Traits>&
        operator<<(std::basic_ostream<CharT,Traits>& os,
                   const wrapper_crtp& w)
        {
            return (os << w.derived().get() );
        }

    };


    template<typename D,typename T>
    void rebind( wrapper_crtp<D,T>& a, T& r ){ a.rebind( r ); }

    template<typename D,typename T>
    inline void swap( wrapper_crtp<D,T>& l, wrapper_crtp<D,T>& r )
    {
        l.swap( r );
    }

}// ref
}// v2
}// assign
}// boost

#endif
