//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2009 Neil Groves                                          //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_ADAPTOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_ADAPTOR_ER_2010_HPP
#include <boost/mpl/void.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace chain_aux{

    typedef ::boost::mpl::void_ void_;

    // Tag1 controls conversion. Seet traits/type/meta_convert.hpp

    template<typename U, typename Tag1 = use_default, typename Tag2 = void_>
    struct adaptor1
    {
        adaptor1( U& r ) : value( r ){}
        mutable U& value;
    };

    template<
        typename Tag1 = use_default,
        typename Tag2 = void_
    >
    struct adaptor2
    {
    
        adaptor2(){}
    
        template<typename U>
        struct result{ 
           typedef chain_aux::adaptor1<U, Tag1, Tag2> type; 
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
        
    };
    
}// chain_aux
namespace{
    const chain_aux::adaptor2<> _chain = chain_aux::adaptor2<>();
}
}// v2
}// assign
}// boost

#endif
