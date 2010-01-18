//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_PERSISTENT_TRANSACTION_MANAGER_HPP
#define BOOST_PERSISTENT_TRANSACTION_MANAGER_HPP


#ifndef BOOST_PERSISTENT_CONFIGURATION

#include <boost/persistent/basic_transaction_manager.hpp>
#include <boost/persistent/multiversion_object_resource.hpp>
#include <boost/persistent/redo_storage.hpp>
#include <boost/persistent/block_storage.hpp>
#include <boost/persistent/mapped_file_block.hpp>

/// Is only defined if BOOST_PERSISTENT_CONFIGURATION is not yet defined.
///
/// Default configuration:
/// \code
/// boost::persistent::basic_transaction_manager<
/// 	boost::persistent::multiversion_object_resource<
/// 		boost::persistent::redo_storage<
/// 			boost::persistent::block_storage<
/// 				boost::persistent::mapped_file_block
/// 			>
/// 		>
/// 	>
/// >
/// \endcode
/// \brief Defines the default configuration, using the default transaction manager
#define BOOST_PERSISTENT_CONFIGURATION \
	boost::persistent::basic_transaction_manager< \
		boost::persistent::multiversion_object_resource< \
			boost::persistent::redo_storage< \
				boost::persistent::block_storage< \
					boost::persistent::mapped_file_block \
				> \
			> \
		> \
	>

#endif


namespace boost{
namespace persistent{

/// \brief An alias of the configured transaction manager
typedef BOOST_PERSISTENT_CONFIGURATION transaction_manager;


}
}

#endif //BOOST_PERSISTENT_TRANSACTION_MANAGER_HPP