//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DELAY_CSV_PUT_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_DELAY_CSV_PUT_ER_2011_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/interpreter/as_arg_list.hpp>
#include <boost/assign/v2/put/csv_put.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/config.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif

namespace boost{
namespace assign{
namespace v2{
//[syntax_delay_csv_put
namespace interpreter_aux{

    template<typename R, typename O = empty_list_option>
    struct delayed_csv_put/*<-*/
        : O, as_arg_list_adapter<R>
    {

        typedef O super1_t;
        typedef as_arg_list_adapter<R> super2_t;

        super1_t const& options()const{ return (*this); }
        super2_t const& arg_list()const{ return (*this); }

        explicit delayed_csv_put(R& r)
            : super2_t( r )
        {}

        delayed_csv_put(O options, R& r)
            : super1_t( options ), super2_t( r )
        {}

    }/*->*/;

    template<typename C, typename R, typename O>
    C& operator|(C& cont, delayed_csv_put<R, O> const& rhs)/*<-*/
    {
        csv_put( cont, rhs.options(), rhs.arg_list() );
        return cont;
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

namespace result_of{    

    template<typename R, typename O = empty_list_option>
    struct delay_csv_put/*<-*/
    {
        typedef delayed_csv_put<R, O> type;
    }/*->*/;

}// result_of

    template<typename R>
    typename result_of::delay_csv_put<R const>::type
    delay_csv_put(R const& range)/*<-*/
    {
        return delayed_csv_put<R const>( range );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename O, typename R>
    typename result_of::delay_csv_put<R const, O>::type
    delay_csv_put(O const& options, R const& range)/*<-*/
    {
        return delayed_csv_put<R const, O>( options, range );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux

    using interpreter_aux::delay_csv_put;

namespace result_of{

    using interpreter_aux::result_of::delay_csv_put;

}// result_of
//]

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_DELAY_CSV_PUT_ER_2011_HPP
