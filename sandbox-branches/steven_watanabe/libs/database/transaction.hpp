#ifndef BOOST_DATABASE_TRANSACTION_HPP_INCLUDED
#define BOOST_DATABASE_TRANSACTION_HPP_INCLUDED

#include <boost/utility.hpp>

namespace boost {

	namespace db {

		struct database;

		struct transaction : noncopyable {

			explicit transaction(const database&);

			~transaction();

			void commit();

		private:

			const database& d;

			bool committed;
		};
	}
}

#endif /* BOOST_DATABASE_TRANSACTION_HPP_INCLUDED */
