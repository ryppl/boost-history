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
#include <boost/call_traits.hpp>
#include <boost/operators.hpp>

// This design is an outgrowth of assign_detail::assign_reference<> by TO. The 
// new feature is that the assignment operator and swap member function are 
// customizable and there are two options (for now), identified by MPG : the 
// reference is assigned a new value (copy), or a new address (rebind). 

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

        public: 
        // protected

        D& derived()
        {
            return static_cast<D&>( *this );
        }

        D const & derived()const
        {
            return static_cast<D const&>( *this );
        }

        public:

        void operator=(T& r )
        {
            this->derived().assign( r );
        }

        void swap( wrapper_crtp& r )
        {
            return this->derived().swap( r );
        }

        operator T&() const
        {
            return this->derived().unwrap();
        }

        bool operator==( const wrapper_crtp& r )const
        {
            return this->derived().unwrap() == r.derived().unwrap();
        }

        bool operator<( const wrapper_crtp& r )const
        {
            return this->derived().unwrap() < r.derived().unwrap();
        }

        bool operator==( typename boost::call_traits<T>::param_type r )const
        {
            return this->derived().unwrap() == r;
        }

        bool operator<( typename boost::call_traits<T>::param_type r )const
        {
            return this->derived().unwrap() < r;
        }

        bool operator>( typename boost::call_traits<T>::param_type r )const
        {
            return this->derived().unwrap() > r;
        }
        

    };

	template<typename D,typename T>
	void rebind( wrapper_crtp<D,T>& a, T& r ){ a.rebind( r ); }

    template<typename D,typename T>
    inline void swap( wrapper_crtp<D,T>& l, wrapper_crtp<D,T>& r )
    {
        l.swap( r );
    }

namespace functional{

	template<typename T>
    struct assigner
    {
    	assigner(T& val):value( val ){}
        
        typedef void result_type;
        
        template<typename D>
        void operator()(wrapper_crtp<D,T>& w)const
        {
			w = this->value;
		}        
        private:
        assigner();
        mutable T& value;
    };

}// functional
}// ref
}// v2
}// assign
}// boost

#endif
