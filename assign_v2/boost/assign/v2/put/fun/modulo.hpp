//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_FUN_MODULO_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_FUN_MODULO_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/detail/keyword/ignore.hpp>
#include <boost/assign/v2/put/adapter/fwd.hpp>
#include <boost/assign/v2/put/adapter/replace_parameter.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{
namespace result_of{

    template<typename D>
    struct modulo_fun{
            
        typedef put_aux::replace_fun<D> meta_;
            
        template<typename F>
        struct apply : ::boost::mpl::apply1<meta_, F>{};
        
    };
            
}// result_of

    template<typename F = keyword_aux::ignore>
    struct modulo_fun
    {
        modulo_fun(){}
        modulo_fun(F f) : f_( f ){}

        F const& fun()const{ return this->f_; }
        
        private:
        F f_;
    };

    template<typename C, typename F, typename Tag, typename D, typename F1>
    typename ::boost::mpl::apply1<result_of::modulo_fun<D>, F1>::type
    operator%(
        adapter_crtp<C, F, Tag, D> const& lhs,
        modulo_fun<F1> const& rhs
    )
    {
        typedef result_of::modulo_fun<D> meta_;
        typedef typename ::boost::mpl::apply1<meta_, F1>::type result_;
        return result_( lhs.container(), rhs.fun(), lhs.modifier );
    }

    struct keyword_fun{
    
        template<typename F>
        modulo_fun<F> operator=(F const& f)const{ 
            return modulo_fun<F>( f ); 
        }
    
    };

}// put_aux
namespace{
    const put_aux::keyword_fun _fun = put_aux::keyword_fun();
}
//[syntax_put_fun_modulo
namespace result_of{

    template<typename D>
    struct modulo_fun 
//<-
        : put_aux::result_of::modulo_fun<D>
//->
    {};

}// result_of
//]


}// v2
}// assign
}// boost

#include <boost/preprocessor/cat.hpp>

#define BOOST_ASSIGN_V2_PUT_FUN_alias(NAME, FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace put_aux{\
\
    template<typename T>\
    modulo_fun< FUN > BOOST_PP_CAT(_,NAME)(){ return v2::_fun = FUN(); }\
\
}\
using put_aux::BOOST_PP_CAT(_,NAME);\
}\
}\
}\
/**/

#include <boost/assign/v2/detail/functor/constructor.hpp>
#define BOOST_ASSIGN_V2_fun functor_aux::constructor<T>
BOOST_ASSIGN_V2_PUT_FUN_alias(constructor, BOOST_ASSIGN_V2_fun)
#undef BOOST_ASSIGN_V2_fun

#include <boost/assign/v2/detail/functor/new.hpp>
#define BOOST_ASSIGN_V2_fun functor_aux::new_<T>
BOOST_ASSIGN_V2_PUT_FUN_alias(new, BOOST_ASSIGN_V2_fun)
#undef BOOST_ASSIGN_V2_fun

#undef BOOST_ASSIGN_V2_PUT_FUN_alias

#include <boost/lambda/lambda.hpp>
#include <boost/typeof/typeof.hpp>
namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

    typedef BOOST_TYPEOF( ( _fun = ::boost::lambda::_1 ) ) keyword_identity;

}// put_aux
namespace {
    
    put_aux::keyword_identity const _identity  = ( _fun = ::boost::lambda::_1 );

}
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_FUN_MODULO_ER_2010_HPP
