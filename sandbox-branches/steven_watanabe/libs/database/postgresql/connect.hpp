#ifndef BOOST_DATABASE_POSTGRESQL_CONNECT_HPP_INCLUDED
#define BOOST_DATABASE_POSTGRESQL_CONNECT_HPP_INCLUDED

#include "../database.hpp"

#include <string>

namespace boost {

	namespace db {

		namespace postgresql {

			database connect(const std::string& connect_string);
		}
	}
}

#endif /* BOOST_DATABASE_POSTGRESQL_CONNECT_HPP_INCLUDED */
