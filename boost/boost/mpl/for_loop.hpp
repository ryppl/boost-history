//-----------------------------------------------------------------------------
// boost mpl/for_loop.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_FOR_LOOP_HPP
#define BOOST_MPL_FOR_LOOP_HPP

#include "boost/mpl/select_type.hpp"
#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/binary_function.hpp"
#include "boost/mpl/runtime_function.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<typename Loop> struct for_loop_iteration;
template<typename Loop> struct for_loop_null_iteration;

template< 
      typename Variable
    , typename Condition
    , typename Expression
    , typename State
    , typename BinaryFunction
    >
struct for_loop_context
{
    typedef Variable variable;
    typedef Condition condition;
    typedef Expression expression;
    typedef State state;
    typedef BinaryFunction function;
};

} // namespace detail

template< 
      typename Variable
    , typename Condition
    , typename Expression
    , typename State
    , typename BinaryFunction
    >
struct for_loop : mpl::detail::for_loop_context<
      Variable
    , Condition
    , Expression
    , State
    , BinaryFunction
    >
{
 private:
    typedef for_loop self;
    typedef mpl::detail::for_loop_context< // MSVC workaround, see below
          Variable
        , Condition
        , Expression
        , State
        , BinaryFunction
        > context_;

    typedef typename mpl::select_type_t< 
          typename mpl::unary_function<Condition, Variable>::type
        , mpl::detail::for_loop_iteration<context_> // self??
        , mpl::detail::for_loop_null_iteration<self>
        >::type next_iteration_;
 public:
    typedef typename next_iteration_::executed executed;

    template<typename T>
    static void inline execute(T t)
    {
        next_iteration_::execute(t);
    }
    
    static void inline execute()
    {
        next_iteration_::execute();
    }
};

namespace detail {

template<typename Loop>
struct for_loop_iteration
{
 private:
    typedef typename mpl::binary_function<
          typename Loop::function
        , typename Loop::state
        , typename Loop::variable
        > nullary_function_;

    typedef BOOST_MPL_RUNTIME_FUNCTION(nullary_function_)
        loop_statement_;

    typedef typename mpl::unary_function<
          typename Loop::expression
        , typename Loop::variable
        >::type next_variable_;

    typedef for_loop< 
          next_variable_
        , typename Loop::condition
        , typename Loop::expression
        , typename nullary_function_::type
        , typename Loop::function
        > next_iteration_;

 public:
    typedef typename next_iteration_::executed executed;

    template<typename T>
    static void inline execute(T t)
    {
        loop_statement_::execute(t);
        next_iteration_::execute(t);
    }
        
    static void inline execute()
    {
        loop_statement_::execute();
        next_iteration_::execute();
    }
};

template<typename Loop>
struct for_loop_null_iteration
{
    typedef Loop executed;
    
    template<typename T>
    static void inline execute(T t)
    {}
    
    static void inline execute()
    {}
};

} // namespace detail

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_FOR_LOOP_HPP
