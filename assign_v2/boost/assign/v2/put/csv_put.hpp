//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/interpreter/csv.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/option/data.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif

namespace boost{
namespace assign{
namespace v2{
//[syntax_csv_put
namespace interpreter_aux{

    template<typename F>
    struct csv_ready/*<-*/
        : ::boost::mpl::true_
    {}/*->*/;

    template<typename T, typename K, typename M>
    struct csv_ready<
        functor_aux::pair<T, K, M>
    > /*<-*/
        : ::boost::mpl::false_
    {}/*->*/;

namespace result_of{

    template<typename C, typename F, typename MTag, typename DTag>
    struct if_csv_ready/*<-*/ 
    	: ::boost::mpl::identity<
        	put_interpreter<C, F, MTag, DTag>
    	>
    {}/*->*/;

    template<typename C, typename F, typename MTag, typename DTag>
    struct else_csv_ready/*<-*/
    	: result_of::option_data<
        	put_interpreter<C, F, MTag, DTag>, C, value_
    	>
    {}/*->*/;

    template<typename C, typename F, typename MTag, typename DTag>
    struct make_csv_ready/*<-*/
    	: ::boost::mpl::eval_if<
        	csv_ready<F>,
        	if_csv_ready<C, F, MTag, DTag>,
        	else_csv_ready<C, F, MTag, DTag>
    	>
    {}/*->*/;

}// result_of

/*<-*/
    template<typename C, typename F, typename MTag, typename DTag>
    typename result_of::if_csv_ready<C, F, MTag, DTag>::type
    make_csv_ready( 
        put_interpreter<C, F, MTag, DTag> const& interpreter, 
        boost::mpl::true_ suitable
    )
    {
        return interpreter;
    }

    template<typename C, typename F, typename MTag, typename DTag>
    typename result_of::else_csv_ready<C, F, MTag, DTag>::type
    make_csv_ready( 
        put_interpreter<C, F, MTag, DTag> const& interpreter, 
        boost::mpl::false_ suitable
    )
    {
        return interpreter % ( _data = _value );
    }
/*->*/
    template<typename C, typename F, typename MTag, typename DTag>
    typename result_of::make_csv_ready<C, F, MTag, DTag>::type
    make_csv_ready( put_interpreter<C, F, MTag, DTag> const& interpreter)
    /*->*/{
        return make_csv_ready( 
            interpreter, 
            typename csv_ready<F>::type() 
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

    template<typename C, typename O, bool is, typename Args>
    typename boost::enable_if< 
        is_option_crtp<O> 
    >::type csv_put( C& cont, option_crtp<O, is> const& crtp, Args&&... args )
    /*<-*/
    {
        O const& options = static_cast<O const&>( crtp );
        csv(
            make_csv_ready( 
                put( cont ) % options
            ),
              std::forward<Args>( args )...  
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename C, typename Args>
    typename boost::disable_if< 
        is_option_crtp<O> 
    >::type csv_put( C& cont, Args&&... args)/*<-*/
    {
        csv(
            make_csv_ready( put( cont ) ),
              std::forward<Args>( args )...  
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

}// interpreter_aux
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
using interpreter_aux::csv_put;
//<-
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
//]

}// v2
}// assign
}// boost

#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/put/cpp03/csv_put.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2010_HPP
