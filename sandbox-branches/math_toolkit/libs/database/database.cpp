#include "database.hpp"
#include "row_set.hpp"
#include "exception.hpp"

#include "detail/abstract_row_set.hpp"
#include "detail/abstract_database.hpp"


namespace boost {

namespace db {

	struct database::internal_state {

		internal_state(shared_ptr<detail::abstract_database> pimpl_)
			: pimpl(pimpl_), transaction_depth(0) {
		}

		shared_ptr<detail::abstract_database> pimpl;

		int transaction_depth;
	};

database::database(shared_ptr<detail::abstract_database> pimpl_)
: state(new internal_state(pimpl_)) {
}

row_set
database::query(const std::string& s) const {

	return row_set(
		shared_ptr<detail::abstract_row_set>(
			state->pimpl->query(s).release()
		)
	);
}

execute_result
database::execute(const std::string& s) const {

	return state->pimpl->execute(s);
}

void
database::start_transaction() const {

	state->pimpl->start_transaction();
}

void
database::commit_transaction() const {

	state->pimpl->commit_transaction();
}

void
database::rollback_transaction() const {

	state->pimpl->rollback_transaction();
}
}
}
