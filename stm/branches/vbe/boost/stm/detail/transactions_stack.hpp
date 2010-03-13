//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_TRANSACTIONS_STACK__HPP
#define BOOST_STM_TRANSACTIONS_STACK__HPP

#include <stack>
#include <boost/stm/detail/config.hpp>

namespace boost { namespace stm {

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class transaction;

namespace detail {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct transactions_stack {
#ifdef BOOST_STM_USE_STACK
    typedef std::stack<transaction*> cont_type;
    cont_type stack_;
    transactions_stack() {
        // the stack at least one element (0) so we can always call to top, i.e. current transaction is 0
        stack_.push(0);
    }
    void push(transaction* ptr) {stack_.push(ptr);}
    void pop() {stack_.pop();}
    std::size_t size() {return stack_.size();}
    transaction* top() {return stack_.top();}
#else
    transaction* inner_;
    std::size_t count_;
    transactions_stack() : inner_(0), count_(0) 
    {
        // the stack at least one element (0) so we can always call to top, i.e. current transaction is 0
    }
    inline void push(transaction* ptr);
    inline void pop();
    std::size_t size() {return count_;}
    transaction* top() {return inner_;}
#endif    
};




}

typedef detail::transactions_stack TransactionsStack;

}}

//-----------------------------------------------------------------------------
#endif // BOOST_STM_TRANSACTION_STACK__HPP


