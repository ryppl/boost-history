// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_REBIND_HPP_INCLUDED
#define BOOST_IDL_DETAIL_REBIND_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp> // BOOST_MSVC
#include <boost/detail/workaround.hpp>
#include <boost/interfaces/advice_categories.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/interfaces/detail/enable_by_arity.hpp>

namespace boost { namespace interfaces { namespace detail {
                    
//------------------Primary template------------------------------------------//

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1310)
# pragma warning(push)
//# pragma warning(disable)
#endif

template<typename Advice1, typename Advice2, typename Enabler = void>
struct rebind_advice {
    struct type : Advice1, Advice2 { };
    typedef Advice1 first_type;
    typedef Advice2 second_type;
};

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1310)
# pragma warning(pop)
#endif

//------------------Simple sepcializations------------------------------------//

template<typename Advice>
struct rebind_advice<Advice, Advice> {
    typedef Advice type;
};

template<typename Advice>
struct rebind_advice<Advice, empty_base> {
    typedef Advice type;
};

template<typename Advice>
struct rebind_advice<empty_base, Advice> {
    typedef Advice type;
};

//------------------Specializations for unary Advice1-------------------------//

template< template<typename> class Advice1, 
          typename T, typename Advice2 >
struct rebind_advice< Advice1<T>, Advice2, 
                      typename 
                      enable_by_arity<
                           Advice1<T>, 1
                      >::type > 
{
    typedef typename rebind_advice<T, Advice2>::type  result;
    typedef Advice1<result>                           type;
};

template< template<typename> class Advice1, typename Advice2 >
struct rebind_advice< Advice1<empty_base>, Advice2, 
                      typename 
                      enable_by_arity<
                           Advice1<empty_base>, 1
                      >::type > 
{
    typedef Advice1<Advice2> type;
};

//------------------Specializations for binary Advice1------------------------//

template< template<typename, typename> class Advice1, 
          typename T1, typename T2, typename Advice2 >
struct rebind_advice< Advice1<T1, T2>, Advice2, 
                      typename 
                      enable_by_arity<
                          Advice1<T1, T2>, 2
                      >::type > 
{
    typedef typename rebind_advice<T2, Advice2>::type  result;
    typedef Advice1<T1, result>                        type;
};

template< template<typename, typename> class Advice1, 
          typename T1, typename Advice2 >
struct rebind_advice< Advice1<T1, empty_base>, Advice2, 
                      typename 
                      enable_by_arity<
                          Advice1<T1, empty_base>, 2
                      >::type > 
{
    typedef Advice1<T1, Advice2> type;
};

//------------------Specializations for ternary Advice1-----------------------//

template< template<typename, typename, typename> class Advice1, 
          typename T1, typename T2, typename T3, typename Advice2 >
struct rebind_advice< Advice1<T1, T2, T3>, Advice2, 
                      typename 
                      enable_by_arity<
                          Advice1<T1, T2, T3>, 3
                      >::type > 
{
    typedef typename rebind_advice<T3, Advice2>::type  result;
    typedef Advice1<T1, T2, result>                    type;
};

template< template<typename, typename, typename> class Advice1, 
          typename T1, typename T2, typename Advice2 >
struct rebind_advice< Advice1<T1, T2, empty_base>, Advice2, 
                      typename 
                      enable_by_arity<
                          Advice1<T1, T2, empty_base>, 3
                      >::type > 
{
    typedef Advice1<T1, T2, Advice2> type;
};


//------------------Specializations for 4-ary Advice1-------------------------//

template< template<typename, typename, typename, typename> class Advice1, 
          typename T1, typename T2, typename T3, typename T4, typename Advice2 >
struct rebind_advice< Advice1<T1, T2, T3, T4>, Advice2, 
                      typename 
                      enable_by_arity<
                          Advice1<T1, T2, T3, T4>, 4
                      >::type > 
{
    typedef typename rebind_advice<T4, Advice2>::type  result;
    typedef Advice1<T1, T2, T3, result>                type;
};

template< template<typename, typename, typename, typename> class Advice1, 
          typename T1, typename T2, typename T3, typename Advice2 >
struct rebind_advice< Advice1<T1, T2, T3, empty_base>, Advice2, 
                      typename 
                      enable_by_arity<
                          Advice1<T1, T2, T3, empty_base>, 4
                      >::type > 
{
    typedef Advice1<T1, T2, T3, Advice2> type;
};

//------------------Specializations for 5-ary Advice1-------------------------//


template< template<typename, typename, typename, typename, typename> class Advice1, 
          typename T1, typename T2, typename T3, typename T4, 
          typename T5, typename Advice2 >
struct rebind_advice< Advice1<T1, T2, T3, T4, T5>, Advice2, 
                      typename 
                      enable_by_arity<
                          Advice1<T1, T2, T3, T4, T5>, 5
                      >::type > 
{
    typedef typename rebind_advice<T5, Advice2>::type  result;
    typedef Advice1<T1, T2, T3, T4, result>            type;
};

template< template<typename, typename, typename, typename, typename> class Advice1, 
          typename T1, typename T2, typename T3, typename T4, typename Advice2 >
struct rebind_advice< Advice1<T1, T2, T3, T4, empty_base>, Advice2, 
                      typename 
                      enable_by_arity<
                          Advice1<T1, T2, T3, T4, empty_base>, 5
                      >::type > 
{
    typedef Advice1<T1, T2, T3, T4, Advice2> type;
};

} } } // End namespace detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_REBIND_HPP_INCLUDED
