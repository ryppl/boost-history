//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTIONAL_FUN_ER_2010_HPP
#define BOOST_ASSIGN_V2_OPTIONAL_FUN_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/detail/keyword/ignore.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/interpreter/replace_parameter.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace aux{
namespace result_of{

    template<typename D>
    struct optional_fun{

        typedef aux::replace_fun<D> meta_;

        template<typename F>
        struct apply : ::boost::mpl::apply1<meta_, F>{};

    };

}// result_of

    template<typename F = keyword_aux::ignore>
    struct optional_fun
    {
        optional_fun(){}
        optional_fun(F f) : f_( f ){}

        F const& fun()const{ return this->f_; }

        private:
        F f_;
    };

    template<typename C, typename F, typename Tag, typename D, typename F1>
    typename ::boost::mpl::apply1<result_of::optional_fun<D>, F1>::type
    operator%(
        interpreter_crtp<C, F, Tag, D> const& lhs,
        optional_fun<F1> const& rhs
    )
    {
        typedef result_of::optional_fun<D> meta_;
        typedef typename ::boost::mpl::apply1<meta_, F1>::type result_;
        return result_( lhs.container(), rhs.fun(), lhs.modifier );
    }

    struct keyword_fun{

        template<typename F>
        optional_fun<F> operator=(F const& f)const{
            return optional_fun<F>( f );
        }

    };

}// aux
namespace{
    const aux::keyword_fun _fun = aux::keyword_fun();
}
//[syntax_put_fun_modulo
namespace result_of{

    template<typename D>
    struct optional_fun/*<-*/
        : aux::result_of::optional_fun<D>
    {}/*->*/;

}// result_of
//]


}// v2
}// assign
}// boost

#include <boost/preprocessor/cat.hpp>

#define BOOST_ASSIGN_V2_OPTIONAL_FUN_GENERATE(NAME, FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace aux{\
\
    template<typename T>\
    optional_fun< FUN > NAME()\
    {\
        return ( v2::_fun = FUN() );\
    }\
\
}\
using aux::NAME;\
}\
}\
}\
/**/

#include <boost/assign/v2/detail/functor/constructor.hpp>
BOOST_ASSIGN_V2_OPTIONAL_FUN_GENERATE(constructor, v2::functor_aux::constructor<T>)

#include <boost/assign/v2/detail/functor/new.hpp>
BOOST_ASSIGN_V2_OPTIONAL_FUN_GENERATE(new_ptr, v2::functor_aux::new_<T>)

#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/add_const.hpp>
#define BOOST_ASSIGN_V2_OPTIONAL_FUN_KEYWORD(NAME, EXPR)\
namespace boost{\
namespace assign{\
namespace v2{\
    typedef BOOST_TYPEOF( ( _fun = EXPR ) ) BOOST_PP_CAT(type_of,NAME);\
namespace{\
\
    boost::add_const<BOOST_PP_CAT(type_of,NAME)>::type BOOST_PP_CAT(_,NAME) = ( _fun = EXPR );\
}\
}\
}\
}\

#include <boost/lambda/lambda.hpp>
BOOST_ASSIGN_V2_OPTIONAL_FUN_KEYWORD(identity, ::boost::lambda::_1)

#endif // BOOST_ASSIGN_V2_OPTIONAL_FUN_ER_2010_HPP
