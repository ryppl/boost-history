#ifndef BOOST_DATABASE_DETAIL_ABSTRACT_DATABASE_HPP_INCLUDED
#define BOOST_DATABASE_DETAIL_ABSTRACT_DATABASE_HPP_INCLUDED

#include <string>
#include <memory>

namespace boost {

	namespace db {

		struct execute_result;

		namespace detail {

			struct abstract_row_set;

			struct abstract_database {

				virtual ~abstract_database() {}

				virtual std::auto_ptr<abstract_row_set> query(const std::string&) const = 0;

				virtual execute_result execute(const std::string&) const = 0;

				virtual void start_transaction() = 0;
				virtual void commit_transaction() = 0;
				virtual void rollback_transaction() = 0;
			};
		}
	}
}

#endif /* BOOST_DATABASE_DETAIL_ABSTRACT_DATABASE_HPP_INCLUDED */
