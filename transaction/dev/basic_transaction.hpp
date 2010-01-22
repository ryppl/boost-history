//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_PERSISTENT_BASIC_TRANSACTION_HEADER_HPP
#define BOOST_PERSISTENT_BASIC_TRANSACTION_HEADER_HPP

#include <boost/noncopyable.hpp>

namespace boost{
namespace persistent{

/// Begins a transaction on construction, and rolls it back on destruction if it was not yet
/// committed.
///
/// Template parameters:
/// \li \c TxMgr The transaction manager
/// \brief A transaction scope
template<class TxMgr>
class basic_transaction : noncopyable{
public:
	/// Binds the new transaction to this thread.
	/// If there already is an active transaction, the new transaction will be a nested transaction
	/// of the active transaction.
	///
	/// Throws: \c no_active_transaction_manager, \c finalize_error, \c io_failure, \c thread_resource_error
	/// \brief Constructs a basic_transaction, beginning a new transaction scope
	explicit basic_transaction()
		: parent(TxMgr::has_active_transaction() ? &TxMgr::active_transaction() : 0)
		, tx(TxMgr::begin_transaction())
		, done(false){
		TxMgr::bind_transaction(this->tx);
	}

	/// The transaction is rolled back if it was not yet committed.
	///
	/// Throws: Nothing
	/// \brief Destructs the basic_transaction object
	~basic_transaction(){
		this->pop();
		if(!this->done){
			try{
				TxMgr::rollback_transaction(this->tx);
			}catch(...){
#ifndef NDEBUG
				std::cerr << "ignored exception" << std::endl;
#endif
			}
		}
	}

	/// If this is a nested transaction, sets the active transaction to the parent transaction.
	/// If this is a root transaction, resets the active transaction.
	///
	/// Throws: \c isolation_exception,
	/// \c archive_exception, \c io_failure, \c thread_resource_error, any exception thrown by the following user-supplied functions: \c T::T(), \c serialize(), \c save(), \c load(), \c construct(), \c equal(), \c finalize()
	/// \brief Commits the transaction.
	void commit(){
		this->pop();
		this->done=true;
		TxMgr::commit_transaction(this->tx);
	}

	/// If this is a nested transaction, sets the active transaction to the parent transaction.
	/// If this is a root transaction, resets the active transaction.
	///
	/// Throws: \c io_failure, \c thread_resource_error
	/// \brief Unwinds all changes made during this transaction.
	void rollback(){
		this->pop();
		this->done=true;
		TxMgr::rollback_transaction(this->tx);
	}

	/// Throws: Nothing
	/// \brief Binds the current thread to this transaction
	void bind(){
		TxMgr::bind_transaction(this->tx);
	}

	/// Throws: Nothing
	/// \brief If the current thread is bound to this transaction, unbinds it
	void unbind(){
		if(TxMgr::has_active_transaction() &&
			&TxMgr::active_transaction() == &this->tx){
			TxMgr::unbind_transaction();
		}
	}

	/// \cond
private:
	void pop(){
		if(TxMgr::has_active_transaction() && &TxMgr::active_transaction() == &this->tx){
			if(this->parent) TxMgr::bind_transaction(*this->parent);
			else TxMgr::unbind_transaction();
		}
	}

	typename TxMgr::transaction *parent;
	typename TxMgr::transaction tx;
	bool done;
	/// \endcond
};



}
}

/// \brief Begins a new transaction scope.
///
/// BOOST_PERSISTENT_BASIC_ATOMIC and BOOST_PERSISTENT_BASIC_RETRY provide a simple syntax for
/// concurrent transactions.
///
/// When used together the macros
/// expand to a transaction scope that gets repeatedly executed until the transaction
/// is successfully committed without conflicting with another transaction.
///
/// Example: \code
/// #define atomic BOOST_PERSISTENT_ATOMIC
/// #define retry BOOST_PERSISTENT_RETRY
///
/// atomic{
///     ...
/// } retry;
/// \endcode expands to code that is equivalent to \code
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
#define BOOST_PERSISTENT_BASIC_ATOMIC(TXMGR) \
	do{ \
		try{ \
			boost::persistent::basic_transaction<TXMGR> ___tx;

/// Ends a transaction scope.
/// See BOOST_PERSISTENT_BASIC_ATOMIC for a detailed description.
#define BOOST_PERSISTENT_BASIC_RETRY(TXMGR) \
			___tx.commit(); \
		}catch(boost::persistent::isolation_exception &i){ \
			i.unwind<TXMGR>(); \
			continue; \
		} \
	}while(false);




#endif //BOOST_PERSISTENT_BASIC_TRANSACTION_HEADER_HPP