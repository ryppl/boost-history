//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_FUN_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_FUN_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/assign/v2/put/frame/fwd.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of_modulo{

    template<typename D> struct fun/* Specialize on D(erived) */{
        template<typename F> struct apply{};
    };

}// result_of_modulo
namespace put_aux{

    template<typename F>
    struct modulo_fun
    {
        modulo_fun(const F& f) : value(f){}
        F value;
    };

    template<typename C, typename F, typename Tag, typename D, typename F1>
    typename ::boost::mpl::apply1<result_of_modulo::fun<D>, F1>::type
    operator%(
        crtp<C, F, Tag, D> const& lhs,
        modulo_fun<F1> const& rhs
    )
    {
        typedef result_of_modulo::fun<D> meta_;
        typedef typename ::boost::mpl::apply1<meta_, F1>::type result_;
        return result_( lhs.container(), rhs.value, lhs.modifier );
    }

    struct kwd_fun{

        template<typename F>
        modulo_fun<F> operator=(F const& f)const
        {
            return put_aux::modulo_fun<F>( f );
        }

    };

}// put_aux
namespace{
    const put_aux::kwd_fun _fun = put_aux::kwd_fun();
}
}// v2
}// assign
}// boost

#endif
