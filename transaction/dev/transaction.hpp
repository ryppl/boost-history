//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_PERSISTENT_DEFAULT_TRANSACTION_HEADER_HPP
#define BOOST_PERSISTENT_DEFAULT_TRANSACTION_HEADER_HPP

#include <boost/persistent/basic_transaction.hpp>
#include <boost/persistent/transaction_manager.hpp>


namespace boost{
namespace persistent{

/// \brief An alias of \c basic_transaction using the default transaction manager
typedef basic_transaction<transaction_manager> transaction;


}
}



/// \brief An alias of BOOST_PERSISTENT_BASIC_ATOMIC using the default transaction manager
#define BOOST_PERSISTENT_ATOMIC \
	BOOST_PERSISTENT_BASIC_ATOMIC(boost::persistent::transaction_manager)

/// \brief An alias of BOOST_PERSISTENT_BASIC_RETRY using the default transaction manager
#define BOOST_PERSISTENT_RETRY \
	BOOST_PERSISTENT_BASIC_RETRY(boost::persistent::transaction_manager)


#endif //BOOST_PERSISTENT_TRANSACTION_HEADER_HPP