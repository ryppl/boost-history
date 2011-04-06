//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DELAYED_CSV_PUT_HPP_ER_2010
#define BOOST_ASSIGN_V2_PUT_DELAYED_CSV_PUT_HPP_ER_2010
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/keyword.hpp>
#include <boost/assign/v2/detail/traits.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/assign/v2/ref/array/as_arg_list.hpp>
#include <boost/assign/v2/ref/array/csv_array.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector/vector0.hpp>
#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/assign/v2/ref/wrapper/cpp0x.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
//[syntax_pipe_csv_put
namespace interpreter_aux{

    typedef ref::array_aux::size_type arg_list_size_type;

    template<arg_list_size_type N, typename U, typename Os = empty_list_option>
    struct delayed_csv_put_holder/*<-*/
    {
        typedef typename v2::ref::nth_result_of::csv_array<
            N, U
        >::type arg_list_type; //notice it's arg, not args

        delayed_csv_put_holder(){}
        delayed_csv_put_holder(arg_list_type const& a)
            : arg_list_( a )
        {}
        delayed_csv_put_holder(Os options, arg_list_type const& a)
            : options_( options ), arg_list_( a )
        {}

        arg_list_type const& arg_list() const
        {
            return this->arg_list_cont_;
        }

        Os const& options() const
        {
            return this->options_;
        }

        protected:
        Os options_;
        arg_list_type arg_list_;

    }/*->*/;
    
#if BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace result_of{

    template<arg_list_size_type N, typename U, typename Os = empty_list_option>
    struct delayed_csv_put
    {
        typedef delayed_csv_put_holder<
            typename ref::wrapper_param<U>::type, N, Os
        > type;
    };

}// result_of

    // Explicit U

    template<typename U, typename...Args>
    typename result_of::delayed_csv_put<U, size_of...(Args)>::type
    delayed_csv_put(Args&&... args)
    {
        return typename result_of::delayed_csv_put<U, size_of...(Args)>::type(
            ref::csv_array<U>( std::forward<Args>( args )... )
        );
    }

    template<typename U, typename O, bool is, typename...Args>
    typename result_of::delayed_csv_put<U, size_of...(Args), O>::type
    delayed_csv_put(option_crtp<O, is> const& crtp, Args&&... args)
    {
        typedef O const& options_;
        options_ options = static_cast<options_>( crtp );
        return typename result_of::delayed_csv_put<
            U, size_of...(Args), O
        >::type(
            options,
            ref::csv_array<U>( std::forward<Args>( args )... )
        );
    }

    // Implicit U

    template<typename U, typename...Args>
    typename result_of::delayed_csv_put<U, 1 + size_of...(Args)>::type
    delayed_csv_put(U&& u, Args&&... args)
    {
        return delayed_csv_put<U>( 
            std::forward<U>( u ), 
            std::forward<Args>( args ) ... 
        );
    }

    template<typename U, typename O, bool is, typename...Args>
    typename result_of::delayed_csv_put<U, 1 + size_of...(Args), O>::type
    delayed_csv_put(option_crtp<O, is> const& crtp, U&& u, Args&&... args)
    {
        return delayed_csv_put<U>( 
            crtp,  
            std::forward<U>( u ), 
            std::forward<Args>( args ) ... 
        );
    }
    
#else

    template<typename U>
    typename result_of::delayed_csv_put<U, 0>::type
    delayed_csv_put()
    {
        return typename result_of::delayed_csv_put<U, 0>::type(
            ref::csv_array<U>();
        );
    }

#define BOOST_ASSIGN_V2_MACRO2(z, N, U)\
    template<typename T, arg_list_size_type N>\
    typename result_of::delayed_csv_put<U, N>::type\
    delayed_csv_put( BOOST_PP_ENUM_PARAMS(N, U &_) )\
    {\
        return typename result_of::delayed_csv_put<U, N>::type(\
            ref::csv_array<U>( BOOST_PP_ENUM_PARAMS(N, _) )\
        );\
    }\
    template<typename T, typename O, bool is, arg_list_size_type N>\
    typename result_of::delayed_csv_put<U, N>::type\
    delayed_csv_put(\
        option_crtp<O, is> const& crtp,\
        BOOST_PP_ENUM_PARAMS(N, U &_)\
    )\
    {\
        typedef O const& options_;\
        options_ options = static_cast<options_>( crtp );\
        return typename result_of::delayed_csv_put<U, N>::type(\
            ref::csv_array<U>( BOOST_PP_ENUM_PARAMS(N, _) )\
        );\
    }\
/**/

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    T
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    T const
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

    }/*->*/;

    template<typename C, arg_list_size_type N, typename U, typename Os>
    C& operator|(C& cont, delayed_csv_put_holder<N, U, Os> const& rhs)/*<-*/
    {
        v2::ref::as_arg_list( 
            make_csv_ready( put( cont ) % rhs.options() ), 
            rhs.arg_list() 
        );
        return cont;

    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux 
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_DELAYED_CSV_PUT_HPP_ER_2010

