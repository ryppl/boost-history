//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_TRANSACTION_HEADER_HPP
#define BOOST_TRANSACT_TRANSACTION_HEADER_HPP

#include <boost/transact/basic_transaction.hpp>
#include <boost/transact/transaction_manager.hpp>


namespace boost{
namespace transact{

/// \brief An alias of \c basic_transaction using the default transaction manager
typedef basic_transaction<transaction_manager> transaction;


}
}


#endif