//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2009 Neil Groves                                          //
//  Copyright (C) 2010 Manuel Peinado Gallego                               //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_ADAPTOR_ER_2011_HPP
#define BOOST_ASSIGN_V2_CHAIN_ADAPTOR_ER_2011_HPP
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/chain/meta.hpp>
#include <boost/mpl/void.hpp>

// Design:
// - Original design was boost::chain in RangeEx (later joined)
// - ER : lvalue-preserving
// - Finally, MPG proposed a way to compose chains http://gist.github.com/287791

namespace boost{
namespace assign{
namespace v2{
//[syntax_chain_adaptor
namespace chain_aux{/*<-*/

    typedef ::boost::mpl::void_ void_;/*->*/

    template<typename U>
    struct adaptor1/*<-*/
    {
        adaptor1( U& r )
            : ptr( &r )
        {}
        U& reference()const{ return *this->ptr; }
        private:
        mutable U* ptr;
    }/*->*/;

    struct adaptor2/*<-*/
    {

        adaptor2(){}

        template<typename U>
        struct result{
           typedef chain_aux::adaptor1<U> type;
           static type call(U& u){ return type( u ); }
        };

        template<typename R>
        typename result<R>::type
        operator()(R& r)const{
            return result<R>::call ( r );
        }

        template<typename R>
        typename result<R const>::type
        operator()(R const& r)const{
            return result<R const>::call( r );
        }

    }/*->*/;

    template<typename R1, typename U2>
    typename result_of::chain<R1, U2>::type
    operator|(R1 & r1, chain_aux::adaptor1<U2> const & h)/*<-*/
    {
        typedef result_of::chain<R1, U2> caller_;
        return caller_::call( r1, h.reference() );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename R1, typename U2>
    typename result_of::chain<R1 const, U2>::type
    operator|(R1 const & r1, chain_aux::adaptor1<U2> const & h)/*<-*/
    {
        typedef result_of::chain<R1 const, U2> caller_;
        return caller_::call( r1, h.reference() );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// chain_aux
//<-
namespace{
//->

    const chain_aux::adaptor2 _chain/*<-*/ = chain_aux::adaptor2()/*->*/;

//<-
}
//->
namespace result_of{

    using chain_aux::result_of::chain;

}// result_of
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_CHAIN_ADAPTOR_ER_2011_HPP
