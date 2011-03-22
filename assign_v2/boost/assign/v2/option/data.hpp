//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_DATA_ER_2010_HPP
#define BOOST_ASSIGN_V2_OPTION_DATA_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/ignore.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/interpreter/replace_parameter.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_option_data
namespace aux{
namespace result_of{

    template<typename D>
    struct /*<<Metafunction class>>*/option_data_generator/*<-*/{

        typedef aux::replace_fun<D> meta_;

        template<typename F>
        struct apply : ::boost::mpl::apply1<meta_, F>{};

    };

}// result_of

    template<typename F/*<-*/= keyword_aux::ignore/*->*/>
    struct option_data_generator/*<-*/
    {
        option_data_generator(){}
        option_data_generator(F f) : f_( f ){}

        F const& fun()const{ return this->f_; }

        private:
        F f_;
    }/*->*/;

    template<typename C, typename F, typename Tag, typename D, typename F1>
    typename ::boost::mpl::apply1<result_of::option_data_generator<D>, F1>::type
    operator%/*<<Overrides data generator>>*/(
        interpreter_crtp<C, F, Tag, D> const& lhs,
        option_data_generator<F1> const& rhs
    )/*<-*/
    {
        typedef result_of::option_data_generator<D> meta_;
        typedef typename ::boost::mpl::apply1<meta_, F1>::type result_;
        return result_( lhs.container(), rhs.fun(), lhs.modifier );
    }/*<-*/BOOST_ASSIGN_V2_IGNORE(/*<-*/;/*->*/)/*->*/

    struct keyword_data_generator
    /*<-*/{

        template<typename F>
        option_data_generator<F> operator=(F const& f)const{
            return option_data_generator<F>( f );
        }

    }/*->*/;

}// aux
namespace{
    const aux::keyword_data_generator _data/*<-*/ 
        = aux::keyword_data_generator()/*->*/;
}
namespace result_of{

    template<typename D>
    struct /*<<Metafunction class>>*/option_data_generator/*<-*/
        : aux::result_of::option_data_generator<D>
    {}/*->*/;

}// result_of
//]


}// v2
}// assign
}// boost

#include <boost/preprocessor/cat.hpp>

#define BOOST_ASSIGN_V2_OPTION_DATA_GENERATE(NAME, FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace aux{\
\
    template<typename T>\
    option_data_generator< FUN > NAME()\
    {\
        return ( v2::_data = FUN() );\
    }\
\
}\
using aux::NAME;\
}\
}\
}\
/**/

#include <boost/assign/v2/detail/functor/constructor.hpp>
BOOST_ASSIGN_V2_OPTION_DATA_GENERATE(constructor, v2::functor_aux::constructor<T>)

#include <boost/assign/v2/detail/functor/new.hpp>
BOOST_ASSIGN_V2_OPTION_DATA_GENERATE(new_ptr, v2::functor_aux::new_<T>)

#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/add_const.hpp>
#define BOOST_ASSIGN_V2_OPTION_DATA_KEYWORD(NAME, EXPR)\
namespace boost{\
namespace assign{\
namespace v2{\
    typedef BOOST_TYPEOF( ( _data = EXPR ) ) BOOST_PP_CAT(type_of,NAME);\
namespace{\
\
    boost::add_const<BOOST_PP_CAT(type_of,NAME)>::type BOOST_PP_CAT(_,NAME) = ( _data = EXPR );\
}\
}\
}\
}\

#include <boost/lambda/lambda.hpp>
BOOST_ASSIGN_V2_OPTION_DATA_KEYWORD(identity, ::boost::lambda::_1)

#endif // BOOST_ASSIGN_V2_OPTION_DATA_ER_2010_HPP
