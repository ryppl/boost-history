//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DO_CSV_PUT_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_DO_CSV_PUT_ER_2011_HPP
#include <boost/assign/v2/deque/csv_deque.hpp>
#include <boost/assign/v2/put/do_put.hpp>

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <tuple>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/support/traits/type/add_ref_const.hpp>
#include <boost/assign/v2/support/mpl/variadic_args_to_indices.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_do_csv_put
namespace interpreter_aux{
namespace result_of{

//<-
    template<typename Options, int I>
    struct do_csv_put_helper
    {
        template<typename CastHolder, typename ArgsHolder>
        struct apply{};

        template<typename...Cast, typename... Args>
        struct apply<pack_holder<Cast...>, pack_holder<Args...> >
        {
            typedef pack_holder<
                typename type_traits::add_ref_const<Cast>::type...
            > head_;
            typedef pack_holder<Args...> tail_;
            typedef typename csv_cast<I, head_, tail_>::type cast_;

            typedef typename csv_tuple_head<
                I, pack_holder<>, pack_holder<Args...>
            >::type tuple_;
            typedef typename result_of::csv_deque<tuple_>::type cont_;
            typedef put_for_each_adapter<
                typename boost::add_const<cont_>::type,
                Options,
                I
            > type;

            static type call(Options const& options, Args&&... args)
            {
                return type( options, v2::csv_deque<tuple_, I>(
                    std::forward<Args>( args )...
                ) );
            }
        };
    };

//->
    template<
         typename Options,
         typename CastHolder, typename ArgsHolder,
         int I
    >
    struct do_csv_put/*<-*/
        : do_csv_put_helper<Options, I>:: template apply<
            CastHolder, ArgsHolder
        >
    {}/*->*/;

}// result_of

    template<typename Options, int I, typename ...Cast, typename...Args>
/*<-*/
    typename result_of::do_csv_put<
        Options,
        pack_holder<Cast...>,
        pack_holder<Args...>,
        I
    >::type/*->*/
    /*<-*/BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/
    do_csv_put(Args&&... args)/*<-*/
    {
        typedef result_of::do_csv_put<
            Options,
            pack_holder<Cast...>,
            pack_holder<Args...>,
            I
        > meta_;
        return meta_::call( Options(), std::forward<Args>( args )... );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<int I, typename ...Cast, typename...Args>
/*<-*/
    typename result_of::do_csv_put<
        empty_list_option_,
        pack_holder<Cast...>,
        pack_holder<Args...>,
        I
    >::type/*->*/
    /*<-*/BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/
    do_csv_put(Args&&... args)/*<-*/
    {
        return do_csv_put<empty_list_option_, I, Cast...>(
            std::forward<Args>(args)...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux

using interpreter_aux::do_csv_put;

//]
}// v2
}// assign
}// boost

#else
#include <boost/assign/v2/put/cpp03/do_csv_put.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
#endif // BOOST_ASSIGN_V2_PUT_DO_CSV_PUT_ER_2011_HPP
