//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CPP03_DELAY_CSV_PUT_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_CPP03_DELAY_CSV_PUT_ER_2011_HPP
#include <boost/assign/v2/support/config/limit_csv_arity.hpp>
#include <boost/assign/v2/support/config/limit_tuple_arity.hpp>
#include <boost/assign/v2/support/pp/parameter_list.hpp>
#include <boost/assign/v2/put/do_csv_put.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{
namespace result_of{

    template<typename T>
    struct do_csv_put_elem
        : boost::add_reference<T>
    {};

    template<>
    struct do_csv_put_elem<boost::tuples::null_type>
    {
        typedef boost::tuples::null_type type;
    };

    template<
        typename Options,
        int I,
        BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
            BOOST_ASSIGN_V2_LIMIT_CPP03_TUPLE_ARITY,
            typename T,
            boost::tuples::null_type
        )
    >
    struct do_csv_put
    {
#define BOOST_ASSIGN_V2_MACRO(z, i, data)\
    typename do_csv_put_elem<BOOST_PP_CAT(T,i)>::type\
/**/
        typedef boost::tuple<
            BOOST_PP_ENUM(
                BOOST_ASSIGN_V2_LIMIT_CPP03_TUPLE_ARITY,
                BOOST_ASSIGN_V2_MACRO,
                ~
            )
        > tuple_;
#undef BOOST_ASSIGN_V2_MACRO
        typedef typename result_of::csv_deque<tuple_>::type cont_;
        typedef put_for_each_adapter<
            typename boost::add_const<cont_>::type,
            Options,
            I
        > type;

};
}// result_of

#define BOOST_ASSIGN_V2_DELAY_CSV_PUT_NESTED_ITER(z, N, SeqU)\
    template<\
        typename Options,\
        int I,\
        BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)\
    >\
    typename boost::lazy_enable_if_c<\
        I == BOOST_PP_SEQ_SIZE(SeqU),\
        result_of::do_csv_put<\
            Options,\
            I,\
            BOOST_PP_SEQ_ENUM(SeqU)\
        >\
    >::type\
    do_csv_put\
    (\
        BOOST_ASSIGN_V2_CSV_PARAMETER_LIST(SeqU, N)\
    )\
    {\
        typedef result_of::do_csv_put<\
            Options,\
            I,\
            BOOST_PP_SEQ_ENUM(SeqU)\
        > meta_;\
        typedef typename meta_::tuple_ tuple_;\
        typedef typename meta_::type result_;\
        return result_(\
            Options(),\
            csv_deque<\
                tuple_,\
                I,\
                BOOST_PP_SEQ_ENUM(SeqU)\
            >(\
                BOOST_PP_ENUM_PARAMS(\
                    BOOST_PP_MUL( N, BOOST_PP_SEQ_SIZE(SeqU) ),\
                    _\
                )\
            )\
        );\
    }\
    template<int I, BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)>\
    typename boost::lazy_enable_if_c<\
        I == BOOST_PP_SEQ_SIZE(SeqU),\
        result_of::do_csv_put<\
            empty_list_option_,\
            I,\
            BOOST_PP_SEQ_ENUM(SeqU)\
        >\
    >::type\
    do_csv_put\
    (\
        BOOST_ASSIGN_V2_CSV_PARAMETER_LIST(SeqU, N)\
    )\
    {\
        return do_csv_put<\
            empty_list_option_,\
            I,\
            BOOST_PP_SEQ_ENUM(SeqU)\
        >(\
            BOOST_PP_ENUM_PARAMS(\
                BOOST_PP_MUL( N, BOOST_PP_SEQ_SIZE(SeqU) ),\
                _\
            )\
        );\
    }\
/**/

#define BOOST_ASSIGN_V2_DELAY_CSV_PUT_ITER(r, SeqU)\
    BOOST_PP_REPEAT_FROM_TO(\
        1,\
        BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),\
        BOOST_ASSIGN_V2_DELAY_CSV_PUT_NESTED_ITER,\
        SeqU\
    )\
/**/

#define BOOST_ASSIGN_V2_DELAY_CSV_PUT_CONST_NON_CONST_OVERLOAD(z, I, data)\
    BOOST_PP_SEQ_FOR_EACH_PRODUCT(\
        BOOST_ASSIGN_V2_DELAY_CSV_PUT_ITER,\
        BOOST_ASSIGN_V2_CSV_SEQ1(I)\
    )\
/**/

#define BOOST_ASSIGN_V2_DELAY_CSV_PUT_OVERLOAD(z, I, pos)\
    BOOST_ASSIGN_V2_DELAY_CSV_PUT_ITER(\
        ~,\
        BOOST_ASSIGN_V2_CSV_SEQ2(pos, I)\
    )\
/**/

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_ASSIGN_V2_DELAY_CSV_PUT_CONST_NON_CONST_OVERLOAD,
    ~
)

BOOST_PP_REPEAT_FROM_TO(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_DELAY_CSV_PUT_OVERLOAD,
    0
)

BOOST_PP_REPEAT_FROM_TO(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_DELAY_CSV_PUT_OVERLOAD,
    1
)

#undef BOOST_ASSIGN_V2_DELAY_CSV_PUT_NESTED_ITER
#undef BOOST_ASSIGN_V2_DELAY_CSV_PUT_ITER
#undef BOOST_ASSIGN_V2_DELAY_CSV_PUT_CONST_NON_CONST_OVERLOAD
#undef BOOST_ASSIGN_V2_DELAY_CSV_PUT_OVERLOAD

}// interpreter_aux
using interpreter_aux::do_csv_put;
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CPP03_DELAY_CSV_PUT_ER_2011_HPP
