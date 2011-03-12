//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_FUN_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_FUN_MAKE_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/ignore.hpp>
#include <boost/assign/v2/put/frame/fwd.hpp>
#include <boost/assign/v2/put/modulo/fun/meta.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{
    
    template<typename F = keyword_aux::ignore>
    struct modulo_fun
    {
        modulo_fun(){}
        modulo_fun(F f) : f_( f ){}

        F const& fun()const{ return this->f_; }
        
        private:
        F f_;
    };

    struct keyword_fun{
    
        template<typename F>
        modulo_fun<F> operator=(F const& f)const{ 
            return modulo_fun<F>( f ); 
        }
    
    };

    template<typename C, typename F, typename Tag, typename D, typename F1>
    typename ::boost::mpl::apply1<result_of::modulo_fun<D>, F1>::type
    operator%(
        crtp<C, F, Tag, D> const& lhs,
        modulo_fun<F1> const& rhs
    )
    {
        typedef result_of::modulo_fun<D> meta_;
        typedef typename ::boost::mpl::apply1<meta_, F1>::type result_;
        return result_( lhs.container(), rhs.fun(), lhs.modifier );
    }


}// put_aux
namespace{
    const put_aux::keyword_fun _fun = put_aux::keyword_fun();
}
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_MODULO_FUN_MAKE_ER_2010_HPP
