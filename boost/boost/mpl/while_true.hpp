//-----------------------------------------------------------------------------
// boost mpl/while_true.hpp header file
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

#ifndef BOOST_MPL_WHILE_TRUE_HPP
#define BOOST_MPL_WHILE_TRUE_HPP

#include "boost/mpl/select_type.hpp"
#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/binary_function.hpp"
#include "boost/mpl/runtime_function.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<typename Loop> struct while_true_iteration;
template<typename Loop> struct while_true_null_iteration;

template< 
      typename Condition
    , typename State
    , typename UnaryFunction
    >
struct while_true_context
{
    typedef Condition condition;
    typedef State state;
    typedef UnaryFunction function;
};

} // namespace detail

template< 
      typename Condition
    , typename State
    , typename UnaryFunction
    >
struct while_true : mpl::detail::while_true_context<
      Condition
    , State
    , UnaryFunction
    >
{
 private:
    typedef while_true self;
    typedef mpl::detail::while_true_context< // MSVC workaround, see below
          Condition
        , State
        , UnaryFunction
        > context_;

    typedef typename mpl::select_type< 
          mpl::unary_function<Condition, State>::value
        , mpl::detail::while_true_iteration<context_> // self??
        , mpl::detail::while_true_null_iteration<self>
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
struct while_true_iteration
{
 private:
    typedef typename mpl::unary_function<
          typename Loop::function
        , typename Loop::state
        > nullary_function_;

    typedef BOOST_MPL_RUNTIME_FUNCTION(nullary_function_)
        loop_statement_;

    typedef while_true< 
          typename Loop::condition
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
struct while_true_null_iteration
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

#endif // BOOST_MPL_WHILE_TRUE_HPP
