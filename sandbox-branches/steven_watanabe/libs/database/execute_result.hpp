#ifndef BOOST_DATABASE_EXECUTE_RESULT_HPP_INCLUDED
#define BOOST_DATABASE_EXECUTE_RESULT_HPP_INCLUDED

#include <boost/optional.hpp>

namespace boost {

	namespace db {

		struct execute_result {

			optional<unsigned int> rows_affected;
			
			optional<long long> inserted_object_id;
		};
	}
}

#endif /* BOOST_DATABASE_EXECUTE_RESULT_HPP_INCLUDED */
