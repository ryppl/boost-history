//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_SPEED_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_SPEED_ER_2010_HPP
#include <cstddef>
#include <deque>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <boost/array.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/put.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/pipe/csv_put.hpp>
#include <boost/assign/v2/ref/array/array.hpp>
#include <boost/assign/v2/ref/array/csv_array.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/typeof/typeof.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace speed_aux{

// --------------- FUNCTOR -------------- //

struct exit_empty{};
template<std::size_t N> struct exit_assert_size{};

template<typename F> void eval_functor(F const& f, exit_empty){}

template<typename F, std::size_t N>
void eval_functor(F const& f, exit_assert_size<N>)
{
    assert( f.size() == N );
}

template<typename F, typename Exit, typename T, typename... Args>
void eval_functor(F&& f, Exit, T&& t, Args&&... args)
{
    eval_functor(
        std::forward<F>( f )(
            std::forward<T>( t )
        ),
        Exit(),
        std::forward<Args>( args )...
    );
}

// ********* ASSIGN-CONTAINER *********** //

// STL-PUSH_BACK

template<typename C>
void stl_push_back_impl(C& cont){}

template<typename C, typename T, typename... Args>
void stl_push_back_impl(C& cont, T&& t, Args&&... args)
{
    cont.push_back( std::forward<T>( t ) );
    stl_push_back_impl( cont, std::forward<Args>( args )... );
}

template<typename Timer, typename E, typename C, typename... Args>
void stl_push_back(std::size_t n, E& elapsed, C& cont, Args&&... args)
{
    Timer timer;
    for(std::size_t i = 0; i < n; i++)
    {
        cont.clear();
        stl_push_back_impl( cont, std::forward<Args>( args )... );
    }
    elapsed = timer.elapsed();
    assert( cont.size() == sizeof...(Args) );
}


// V1-PUSH_BACK

template<typename Timer, typename E, typename C, typename... Args>
void v1_push_back(std::size_t n, E& elapsed, C& cont, Args&&... args)
{
    cont.clear();
    Timer timer;
    for(std::size_t i = 0; i < n; i++)
    {
        cont.clear();
        BOOST_AUTO(functor, assign::push_back( cont ) );
        eval_functor(
            functor, // requires lvalue
            exit_empty(),
            std::forward<Args>( args )...
        );
    }
    elapsed = timer.elapsed();
    assert( cont.size() == sizeof...(Args) );
}

// V2-PUT

template<typename Timer, typename E, typename C, typename... Args>
void v2_put(std::size_t n, E& elapsed, C& cont, Args&&... args)
{
    Timer timer;
    for(std::size_t i = 0; i < n; i++){
        cont.clear();
        eval_functor( v2::put( cont ), exit_empty(), std::forward<Args>( args )... );
    }
    assert( cont.size() == sizeof...(Args) );
    elapsed = timer.elapsed();
}

// V2-PIPE_PUT
/*
template<typename C, typename F>
void v2_pipe_put_impl(C& cont, F const& f){ cont | f; }

template<typename C, typename F, typename T, typename...Args>
void v2_pipe_put_impl(C & cont, F const& f, T&& t, Args&&...args)
{
    v2_pipe_put_impl( cont, f( std::forward<T>( t ) ), std::forward<Args>( args )... );
}

template<typename Timer, typename E, typename C, typename... Args>
void v2_pipe_put(std::size_t n, E& elapsed, C& cont, Args&&... args)
{
    Timer timer;
    for(std::size_t i = 0; i < n; i++)
    {
        cont.clear();
        v2_pipe_put_impl(
            cont,
            v2::_put,
            std::forward<Args>( args )...
        );
    }
    elapsed = timer.elapsed();
    assert( cont.size() == sizeof...(Args) );
}
*/

// V2-PIPE-CSV_PUT

template<typename Timer, typename E, typename C, typename... Args>
void v2_pipe_csv_put(std::size_t n, E& elapsed, C& cont, Args&&... args)
{
    Timer timer;
    for(std::size_t i = 0; i < n; i++)
    {
        cont.clear();
        cont | v2::_csv_put( std::forward<Args>( args )... );
    }
    elapsed = timer.elapsed();
    assert( cont.size() == sizeof...( Args ) );
}

// ********* GENERATE-CONTAINER *********** //

// V1-LIST-OF

template<typename Timer, typename E, typename T, typename... Args>
void v1_list_of(std::size_t n, E& elapsed, T&& t, Args&&... args)
{
#define MACRO(Exit)\
    eval_functor(\
        assign::list_of( std::forward<T>( t ) ),\
        Exit(),\
        std::forward<Args>( args )...\
    );\
/**/
    Timer timer;
    for(std::size_t i = 0; i < n; i++){ MACRO( exit_empty ) }
    elapsed = timer.elapsed();
    MACRO( exit_assert_size< 1 + sizeof...(Args)> )

#undef MACRO
}

// V1-CREF-LIST-OF

template<typename Timer, typename E, typename T, typename... Args>
void v1_cref_list_of(std::size_t n, E& elapsed, T&& t, Args&&... args)
{
#define MACRO(Exit)\
    eval_functor(\
        assign::cref_list_of<1 + sizeof...(Args)>( std::forward<T>( t ) ),\
        Exit(),\
        std::forward<Args>( args )...\
    );\
/**/
    Timer timer;
    for(std::size_t i = 0; i < n; i++){ MACRO( exit_empty ) }
    elapsed = timer.elapsed();
    MACRO( exit_assert_size<1 + sizeof...(Args)> )
#undef MACRO

}

// V2-DEQUE

template<typename Timer, typename E, typename T, typename... Args>
void v2_deque(std::size_t n, E& elapsed, T&& t, Args&&... args)
{
    typedef typename boost::remove_cv<
        typename boost::remove_reference<T>::type
    >::type t_;

#define MACRO(Exit)\
    eval_functor(\
        v2::deque<t_>( std::forward<T>( t ) ),\
        Exit(),\
        std::forward<Args>( args )...\
    );\
/**/
    Timer timer;
    for(std::size_t i = 0; i < n; i++){ MACRO( exit_empty ) }
    elapsed = timer.elapsed();
    MACRO( exit_assert_size<1 + sizeof...(Args)> )

#undef MACRO

}

// V2-CSV_DEQUE

template<typename Timer, typename E, typename... Args>
void v2_csv_deque(std::size_t n, E& elapsed, Args&&... args)
{
#define MACRO v2::csv_deque( std::forward<Args>( args )... )
    Timer timer;
    for(std::size_t i = 0; i < n; i++){ MACRO; }
    elapsed = timer.elapsed();
    assert( MACRO .size() == sizeof...(Args) );
#undef MACRO
}

// V2-REF-ARRAY

template<typename Timer, typename E, typename T, typename... Args>
void v2_ref_array(std::size_t n, E& elapsed, T&& t, Args&&... args)
{
#define MACRO(Exit)\
    eval_functor(\
        v2::ref::array( std::forward<T>( t ) ),\
        Exit(),\
        std::forward<Args>( args )...\
    );\
/**/
    Timer timer;
    for(std::size_t i = 0; i < n; i++){ MACRO(exit_empty) }
    elapsed = timer.elapsed();
    MACRO( exit_assert_size<1 + sizeof...(Args)> )
#undef MACRO
}

// V2-REF_CSV_ARRAY

template<typename Timer, typename E, typename... Args>
void v2_ref_csv_array(std::size_t n, E& elapsed, Args&&... args)
{
#define MACRO v2::ref::csv_array( std::forward<Args>( args )... )
    Timer timer;
    for(std::size_t i = 0; i < n; i++){ MACRO; }
    elapsed = timer.elapsed();
    assert( MACRO.size() == sizeof...(Args) );
#undef MACRO
}

}// speed_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
#endif // BOOST_ASSIGN_V2_DETAIL_SPEED_ER_2010_HPP
