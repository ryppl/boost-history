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

#ifndef BOOST_STM_LANGUAGE_LIKE__HPP
#define BOOST_STM_LANGUAGE_LIKE__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>

//---------------------------------------------------------------------------
// Transaction control constructs
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// do not remove if (). It is necessary a necessary fix for compilers
// that do not destroy index variables of for loops. In addition, the
// rand()+1 check is necessarily complex so smart compilers can't
// optimize the if away
//---------------------------------------------------------------------------

#ifdef BOOST_STM_COMPILER_DONT_DESTROY_FOR_VARIABLES
#define BOOST_STM_COMPILER_DONT_DESTROY_FOR_VARIABLES_WORKAROUND if (0==rnd()+1) {} else
#else
#define BOOST_STM_COMPILER_DONT_DESTROY_FOR_VARIABLES_WORKAROUND
#endif

//---------------------------------------------------------------------------
// Usage
// BOOST_STM_USE_ATOMIC(_) {
//   transactional block
// }
//---------------------------------------------------------------------------

#define BOOST_STM_USE_ATOMIC(TX)                                \
    BOOST_STM_COMPILER_DONT_DESTROY_FOR_VARIABLES_WORKAROUND    \
    for (boost::stm::transaction TX;                            \
        !TX.committed() && TX.restart();                        \
        TX.end())

//---------------------------------------------------------------------------
// Usage
// BOOST_STM_TRY_ATOMIC(_) {
//   transactional block
// } BOOST_STM_RETRY    // or BOOST_STM_CACHE_BEFORE_RETRY(E)
//                      // or BOOST_STM_BEFORE_RETRY
//---------------------------------------------------------------------------

#define BOOST_STM_TRY_ATOMIC(TX)                                \
    BOOST_STM_COMPILER_DONT_DESTROY_FOR_VARIABLES_WORKAROUND    \
    for (boost::stm::transaction TX;                            \
            !   TX.committed()                                  \
            &&  TX.restart();                                   \
        TX.no_throw_end()) try

//---------------------------------------------------------------------------
// Usage
// BOOST_STM_TRY_ATOMIC(_) {
//   transactional block
// } BOOST_STM_RETRY    // or BOOST_STM_CACHE_BEFORE_RETRY(E)
//                      // or BOOST_STM_BEFORE_RETRY
//---------------------------------------------------------------------------

#define BOOST_STM_ATOMIC(TX)                                     \
    BOOST_STM_COMPILER_DONT_DESTROY_FOR_VARIABLES_WORKAROUND    \
    for (boost::stm::transaction TX;                            \
            !   TX.committed()                                  \
            &&  TX.check_throw_before_restart()                 \
            &&  TX.restart_if_not_inflight();                    \
        TX.no_throw_end()) try

//---------------------------------------------------------------------------
// Catch a named abort exception leting the user to do somethink before retry
//---------------------------------------------------------------------------
#define BOOST_STM_CACHE_BEFORE_RETRY(E) catch (boost::stm::aborted_tx &E)

//---------------------------------------------------------------------------
// Catch a unnamed abort exception leting the user to do somethink before retry
//---------------------------------------------------------------------------
#define BOOST_STM_BEFORE_RETRY catch (boost::stm::aborted_tx &)

//---------------------------------------------------------------------------
// Catch a unnamed abort exception and retry
//---------------------------------------------------------------------------
#define BOOST_STM_RETRY catch (boost::stm::aborted_tx &) {}
#define BOOST_STM_END_ATOM BOOST_STM_RETRY

//---------------------------------------------------------------------------
// Catch a named exception and re-throw it after commiting
//---------------------------------------------------------------------------
#define BOOST_STM_RETHROW(TX, E) catch (E&) {(TX).commit(); throw;}

//---------------------------------------------------------------------------
// Catch any exception and re-throw it after commiting
//---------------------------------------------------------------------------
#define BOOST_STM_RETHROW_ANY(TX) catch (...) {(TX).commit(); throw;}

//---------------------------------------------------------------------------
// Catch a named exception and abort the transaction TX
//---------------------------------------------------------------------------

#define BOOST_STM_TX_ABORT_ON_EXCEPTION(TX, E) \
    catch (E&) {(TX).abort();}
//---------------------------------------------------------------------------
// Catch any exception and abort the transaction TX
//---------------------------------------------------------------------------

#define BOOST_STM_TX_ABORT_ANY(TX) \
    catch (...) {(TX).abort();}

//---------------------------------------------------------------------------
// helper function to implement BOOST_STM_RETURN
namespace boost { namespace stm { namespace detail {

template <typename T>
T commit_and_return(transaction&t, T expression) {
    t.commit(); return expression;
}

}}}

//---------------------------------------------------------------------------
// return the expression EXPRESSION from inside a transaction T
//---------------------------------------------------------------------------
#define BOOST_STM_TX_RETURN(TX, EXPRESSION) \
    return boost::stm::detail::commit_and_return(TX, EXPRESSION)

//---------------------------------------------------------------------------
// return the expression EXPRESSION from inside a transaction T
//---------------------------------------------------------------------------
#define BOOST_STM_RETURN(EXPRESSION) \
    if (boost::stm::current_transaction()==0) return EXPRESSION;  \
    else BOOST_STM_TX_RETURN(*boost::stm::current_transaction(), EXPRESSION)

//---------------------------------------------------------------------------
// break: exit from the transaction block associate to T successfully
//---------------------------------------------------------------------------
#define BOOST_STM_BREAK    continue

//---------------------------------------------------------------------------
// continue: exit from the transaction block associate to T successfully
//---------------------------------------------------------------------------
#define BOOST_STM_CONTINUE continue

//---------------------------------------------------------------------------
// goto : exit from the transaction block associate to T successfully jumping to the named label LABEL
// Note thta label must be outside the transaction block.
//---------------------------------------------------------------------------
#define BOOST_STM_TX_GOTO(TX, LABEL) \
    if (!TX.commit());else goto LABEL

#define BOOST_STM_GOTO(LABEL) \
    if (boost::stm::current_transaction()==0) goto LABEL;  \
    else BOOST_STM_TX_GOTO(*boost::stm::current_transaction(), LABEL)

//---------------------------------------------------------------------------
// aborts the transaction TX
//---------------------------------------------------------------------------
#define BOOST_STM_TX_ABORT(TX) TX.abort()

//---------------------------------------------------------------------------
// aborts the current transaction
//---------------------------------------------------------------------------
#define BOOST_STM_ABORT     \
    if (boost::stm::current_transaction()==0) ;  \
    else BOOST_STM_TX_ABORT(*boost::stm::current_transaction())

//---------------------------------------------------------------------------
// aborts the transaction TX
//---------------------------------------------------------------------------
#define BOOST_STM_TX_ABORT_ALL(TX) TX.abort()

//---------------------------------------------------------------------------
// aborts the current transaction
//---------------------------------------------------------------------------
#define BOOST_STM_ABORT_ALL     \
    if (boost::stm::current_transaction()==0) ;  \
    else BOOST_STM_ABORT_ALL(*boost::stm::current_transaction())

//---------------------------------------------------------------------------
// Memory management
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// creates a new allocated object on transaction TX
//---------------------------------------------------------------------------

#define BOOST_STM_TX_NEW_PTR(TX, T_PARAMS) \
    ((TX).throw_if_forced_to_abort_on_new(), \
    (TX).as_new(new T_PARAMS))

//---------------------------------------------------------------------------
// creates a new array of object of type T on transaction TX
//---------------------------------------------------------------------------

#define BOOST_STM_TX_NEW_ARRAY(TX, SIZE, T) \
    ((TX).throw_if_forced_to_abort_on_new(), \
    (TX).as_new_array(new T[SIZE], SIZE))


//---------------------------------------------------------------------------
// creates a new allocated object on the current transaction
//---------------------------------------------------------------------------
#define BOOST_STM_NEW_PTR(T_PARAMS) \
    ((boost::stm::current_transaction()!=0)  \
    ? BOOST_STM_TX_NEW_PTR(*boost::stm::current_transaction() \
        , T_PARAMS) \
    : new T_PARAMS)

//---------------------------------------------------------------------------
// creates a new array of objects of type T on the current transaction
//---------------------------------------------------------------------------
#define BOOST_STM_NEW_ARRAY(SIZE, T) \
    ((boost::stm::current_transaction()!=0)  \
    ? BOOST_STM_TX_NEW_ARRAY(*boost::stm::current_transaction() \
        , SIZE, T) \
    : new T[SIZE])

//---------------------------------------------------------------------------
// deletes the allocated object on transaction TX
//---------------------------------------------------------------------------

#define BOOST_STM_TX_DELETE_PTR(TX, PTR) \
    (TX).delete_tx_ptr(PTR);

//---------------------------------------------------------------------------
// deletes the allocated object on transaction TX
//---------------------------------------------------------------------------

#define BOOST_STM_TX_DELETE_ARRAY(TX, PTR) \
    (TX).delete_tx_array(PTR);

//---------------------------------------------------------------------------
// deletes the allocated object on transaction TX
//---------------------------------------------------------------------------

#define BOOST_STM_DELETE_PTR(PTR) \
    if (boost::stm::current_transaction()!=0)  \
        BOOST_STM_TX_DELETE_PTR(*boost::stm::current_transaction(), \
            PTR) \
    else delete PTR

//---------------------------------------------------------------------------
// deletes the allocated object on transaction TX
//---------------------------------------------------------------------------

#define BOOST_STM_DELETE_ARRAY(PTR) \
    if (boost::stm::current_transaction()!=0)  \
        BOOST_STM_TX_DELETE_ARRAY(*boost::stm::current_transaction(), \
            PTR) \
    else delete [] PTR

///////////////////////////////////////////////////////////////////////////////
#endif // BOOST_STM_LANGUAGE_LIKE__HPP


