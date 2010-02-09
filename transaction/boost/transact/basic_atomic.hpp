#include <boost/transact/basic_transaction.hpp>

#ifndef BOOST_TRANSACT_BASIC_ATOMIC_HPP
#define BOOST_TRANSACT_BASIC_ATOMIC_HPP

/// \brief Begins a new transaction scope.
///
/// BOOST_TRANSACT_BASIC_ATOMIC and BOOST_TRANSACT_BASIC_COMMIT provide a simple syntax for
/// concurrent transactions.
///
/// When used together the macros
/// expand to a transaction scope that gets repeatedly executed until the transaction
/// is successfully committed without conflicting with another transaction.
///
/// Example: \code
/// #define atomic BOOST_TRANSACT_ATOMIC
/// #define commit BOOST_TRANSACT_COMMIT
///
/// do atomic{
///     ...
/// }commit();
/// \endcode expands to code equivalent to \code
/// do{
///     try{
///         transaction tx;
///         ...
///         tx.commit();
///     }catch(isolation_exception &i){
///         i.unwind<transaction_manager>();
///         continue;
///     }
/// }while(false); \endcode
#define BOOST_TRANSACT_BASIC_ATOMIC(TXMGR) \
	{ \
		try{ \
			boost::transact::basic_transaction<TXMGR> ___tx;

/// Ends a transaction scope.
/// See BOOST_TRANSACT_BASIC_ATOMIC for a detailed description.
#define BOOST_TRANSACT_BASIC_COMMIT(TXMGR) \
			} \
			___tx.commit_(); \
		}catch(boost::transact::isolation_exception &i){ \
			i.unwind<TXMGR>(); \
			continue; \
		} \
	}while(false);

#endif
