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
#include <boost/call_traits.hpp>
#include <boost/operators.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/utility/enable_if.hpp>

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

        typedef typename boost::remove_const<T>::type non_const;
        typedef typename boost::add_const<T>::type const_;

        public:

        D& derived(){ return static_cast<D&>( *this ); }
        D const & derived()const{ return static_cast<D const&>( *this ); }

        public:

        void operator=(non_const& t ){ this->derived().assign( t ); }
        void operator=(const_& t ){ this->derived().assign( t ); }

        void swap( wrapper_crtp& that )
        {
            return this->derived().swap( that );
        }

        operator T&() const
        {
            return this->derived().get();
        }

        bool operator==( const wrapper_crtp& that )const
        {
            return this->derived().get() == that.derived().get();
        }

        bool operator<( const wrapper_crtp& that )const
        {
            return this->derived().get() < that.derived().get();
        }

        bool operator==( typename boost::call_traits<T>::param_type t )const
        {
            return this->derived().get() == t;
        }

        bool operator<( typename boost::call_traits<T>::param_type t )const
        {
            return this->derived().get() < t;
        }

        bool operator>( typename boost::call_traits<T>::param_type t )const
        {
            return this->derived().get() > t;
        }

        template<class CharT, class Traits>
        friend std::basic_ostream<CharT,Traits>&
        operator<<(std::basic_ostream<CharT,Traits>& os,
                   const wrapper_crtp& w)
        {
            return (os << w.derived().get() );
        }

    };


    template<typename D, typename T>
    void rebind( wrapper_crtp<D,T>& a, T& t ){ a.rebind( t ); }

    template<typename D, typename T>
    inline void swap( wrapper_crtp<D,T>& l, wrapper_crtp<D,T>& r )
    {
        l.swap( r );
    }

}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_WRAPPER_CRTP_ER_MPG_2010_HPP
