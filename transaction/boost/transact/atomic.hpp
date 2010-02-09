#ifndef BOOST_TRANSACT_ATOMIC_HPP
#define BOOST_TRANSACT_ATOMIC_HPP

#include <boost/transact/basic_atomic.hpp>
#include <boost/transact/transaction_manager.hpp>

/// \brief An alias of BOOST_TRANSACT_BASIC_ATOMIC using the default transaction manager
#define BOOST_TRANSACT_ATOMIC \
	BOOST_TRANSACT_BASIC_ATOMIC(boost::transact::transaction_manager)

/// \brief An alias of BOOST_TRANSACT_BASIC_COMMIT using the default transaction manager
#define BOOST_TRANSACT_COMMIT() \
	BOOST_TRANSACT_BASIC_COMMIT(boost::transact::transaction_manager)


#endif
