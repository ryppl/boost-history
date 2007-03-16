#ifndef BOOST_DATABASE_DATABASE_HPP_INCLUDED
#define BOOST_DATABASE_DATABASE_HPP_INCLUDED

#include "row_set.hpp"
#include "execute_result.hpp"

#include <boost/shared_ptr.hpp>

namespace boost {

	namespace db {

		namespace detail {
			
			struct abstract_database;
		}

		struct database {

			explicit database(shared_ptr<detail::abstract_database>);

			row_set query(const std::string&) const;

			execute_result execute(const std::string&) const;

			void start_transaction() const;
			void commit_transaction() const;
			void rollback_transaction() const;

		private:

			struct internal_state;

			shared_ptr<internal_state> state;
		};
	}
}

#endif /* BOOST_DATABASE_DATABASE_HPP_INCLUDED */
