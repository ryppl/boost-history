//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2011_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/interpreter/as_arg_list.hpp>
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
/*<-*/
    template<typename F>
    struct csv_ready
        : ::boost::mpl::true_
    {};

    template<typename T, typename K, typename M>
    struct csv_ready<
        functor_aux::pair<T, K, M>
    >
        : ::boost::mpl::false_
    {};

namespace result_of{

    template<typename C, typename F, typename MTag, typename DTag>
    struct if_csv_ready
        : ::boost::mpl::identity<
            put_interpreter<C, F, MTag, DTag>
        >
    {};

    template<typename C, typename F, typename MTag, typename DTag>
    struct else_csv_ready
        : result_of::option_data<
            put_interpreter<C, F, MTag, DTag>, C, value_
        >
    {};

    template<typename C, typename F, typename MTag, typename DTag>
    struct make_csv_ready
        : ::boost::mpl::eval_if<
            csv_ready<F>,
            if_csv_ready<C, F, MTag, DTag>,
            else_csv_ready<C, F, MTag, DTag>
        >
    {};

}// result_of

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

    template<typename C, typename F, typename MTag, typename DTag>
    typename result_of::make_csv_ready<C, F, MTag, DTag>::type
    make_csv_ready( put_interpreter<C, F, MTag, DTag> const& interpreter)
    {
        return make_csv_ready(
            interpreter,
            typename csv_ready<F>::type()
        );
    }

/*->*/
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

    template<typename C, typename O, typename... Args>
    /*<-*/typename boost::enable_if<
        is_list_option<O>
    >::type BOOST_ASSIGN_V2_IGNORE(/*->*/void /*<-*/)
    /*->*/csv_put(
        C& cont,
        O const& option,
        Args&&... args
    )/*<-*/
    {
        csv(
            make_csv_ready(
                put( cont ) % option
            ),
              std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename C, typename T, typename... Args>
    /*<-*/typename boost::disable_if<
        is_list_option_cpp0x<T>
    >::type BOOST_ASSIGN_V2_IGNORE(/*->*/void /*<-*/)
    /*->*/csv_put( C& cont, T&& t, Args&&... args)/*<-*/
    {
        csv(
            make_csv_ready( put( cont ) ),
              std::forward<T>( t ),
              std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename C>
    void csv_put( C& cont )/*<-*/
    {
        csv(
            make_csv_ready( put( cont ) )
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
    template<typename R, typename O = empty_list_option>
    struct delayed_csv_put/*<-*/
        : O, as_arg_list_adapter<R>
    {

        typedef O super1_t;
        typedef as_arg_list_adapter<R> super2_t;

        explicit delayed_csv_put(R& r)
            : super2_t( r )
        {}

        delayed_csv_put(O options, R& r)
            : super1_t( options ), super2_t( r )
        {}

        template<typename C>
        C& apply(C& cont)const
        {
            make_csv_ready(
                put( cont ) % static_cast<super1_t const&>( *this )
            )(
                static_cast<super2_t const&>( *this )
            );

            return cont;
        }


    }/*->*/;

    template<typename C, typename R, typename O>
    C& operator|(C& cont, delayed_csv_put<R, O> const& rhs)/*<-*/
    {
        return rhs.apply( cont );

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
    delay_csv_put(option_crtp<O> const& options, R const& range)/*<-*/
    {
        return delayed_csv_put<R const, O>(
            static_cast<O const&>( options ), range
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
    using interpreter_aux::csv_put;
//<-
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
    using interpreter_aux::delay_csv_put;

namespace result_of{

    using interpreter_aux::result_of::delay_csv_put;

}// result_of
//]

}// v2
}// assign
}// boost

#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/put/cpp03/csv_put.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2011_HPP
