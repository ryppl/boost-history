//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_BASIC_TRANSACTION_HEADER_HPP
#define BOOST_TRANSACT_BASIC_TRANSACTION_HEADER_HPP

#include <boost/noncopyable.hpp>
#include <boost/transact/exception.hpp>
#include <boost/assert.hpp>

namespace boost{
namespace transact{

/// Begins a transaction on construction, and rolls it back on destruction if it is
/// still active.
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
	/// Throws: \c no_transaction_manager, \c io_failure, \c thread_resource_error,
	/// resource-specific exceptions
	/// thrown by resource managers beginning local transactions.
	/// \brief Constructs a basic_transaction, beginning a new transaction scope
	explicit basic_transaction()
		: parent(TxMgr::has_active_transaction() ? &TxMgr::active_transaction() : 0)
		, tx(TxMgr::begin_transaction())
		, done(false){
		TxMgr::bind_transaction(this->tx);
	}

	/// The transaction is rolled back if it is still active. Exceptions
	/// caused by rolling back the transaction are ignored.
	///
	/// Throws: Nothing
	/// \brief Destructs the basic_transaction object
	~basic_transaction(){
		if(!this->done){
			try{
				TxMgr::rollback_transaction(this->tx);
			}catch(...){
#ifndef NDEBUG
				std::cerr << "ignored exception" << std::endl;
#endif
			}
		}
		try{
			this->pop();
		}catch(...){
#ifndef NDEBUG
			std::cerr << "ignored exception" << std::endl;
#endif
		}
	}

	/// If this is a nested transaction, sets the active transaction to the parent transaction.
	/// If this is a root transaction, resets the active transaction.
	///
	/// Throws: \c isolation_exception, \c io_failure, \c thread_resource_error,
	/// resource-specific exceptions thrown by resource managers committing local
	/// transactions.
	/// \brief Commits the transaction.
	void commit(){
		this->done=true;
		try{
			TxMgr::commit_transaction(this->tx);
		}catch(...){
			this->pop();
			throw;
		}
	}
	/// \cond
	void commit_(){
		this->commit();
	}
	/// \endcond

	/// If this is a nested transaction, sets the active transaction to the parent transaction.
	/// If this is a root transaction, resets the active transaction.
	///
	/// Throws: \c io_failure, \c thread_resource_error, resource-specific exceptions
	/// thrown by resource managers rolling back transactions.
	/// \brief Unwinds all changes made during this transaction.
	void rollback(){
		this->done=true;
		try{
			TxMgr::rollback_transaction(this->tx);
		}catch(...){
			this->pop();
			throw;
		}
		this->pop();
	}

	/// Throws: thread_resource_error
	/// \brief Binds the current thread to this transaction
	void bind(){
		TxMgr::bind_transaction(this->tx);
	}

	/// Throws: thread_resource_error
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
		if(this->parent) TxMgr::bind_transaction(*this->parent);
		else TxMgr::unbind_transaction();
	}

	typename TxMgr::transaction *parent;
	typename TxMgr::transaction tx;
	bool done;
	/// \endcond
};


namespace detail{

template<class TxMgr>
struct commit_on_destruction{
	explicit commit_on_destruction(basic_transaction<TxMgr> &tx)
		: tx(&tx){}
	~commit_on_destruction(){
		if(this->tx) this->tx->commit();
	}
	void nullify(){
		this->tx=0;
	}
private:
	basic_transaction<TxMgr> *tx;
};

}

}
}


#define BOOST_TRANSACT_BASIC_BEGIN_TRANSACTION(TXMGR) \
	{ \
		int ___control; \
		boost::transact::basic_transaction<TXMGR> ___tx; \
		while(true){ \
			try{ \
				boost::transact::detail::commit_on_destruction<TXMGR> ___commit(___tx); \
				try{ \
					do{ \
						___control=1; \
						if(false);else

#define BOOST_TRANSACT_BASIC_RETRY(TXMGR) \
						___control=0; \
						break; \
					}while((___control=2),false); \
				}catch(...){ \
					___commit.nullify(); \
					throw; \
				} \
				break; \
			}catch(boost::transact::isolation_exception &___i){ \
				___i.unwind<TXMGR>(); \
				do{ \
					___control=1; \
					if(false);else

#define BOOST_TRANSACT_BASIC_END_RETRY(TXMGR) \
					___control=0; \
					break; \
				}while((___control=2),false); \
			} \
			BOOST_ASSERT(___control == 0); \
			___tx.restart();
		}; \
		BOOST_ASSERT(___control == 0); \
	}void()

#define BOOST_TRANSACT_BASIC_END_RETRY_IN_LOOP(TXMGR) \
					___control=0; \
					break; \
				}while((___control=2),false); \
			} \
			if(___control != 0) break; }
			___tx.restart();
		}; \
		if(___control != 0){ \
			if(___control==1) break; \
			else continue; \
		} \
	}void()

#define BOOST_TRANSACT_BASIC_END_TRANSACTION(TXMGR) \
	BOOST_TRANSACT_BASIC_RETRY(TXMGR){} \
	BOOST_TRANSACT_BASIC_END_RETRY(TXMGR)

#define BOOST_TRANSACT_BASIC_END_TRANSACTION_IN_LOOP(TXMGR) \
	BOOST_TRANSACT_BASIC_RETRY(TXMGR){} \
	BOOST_TRANSACT_BASIC_END_RETRY_IN_LOOP(TXMGR)




#endif