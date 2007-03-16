#include "stdafx.h"
#include "reference_counted_transaction_manager.hpp"

#include "exception.hpp"

namespace boost {

namespace db {

reference_counted_transaction_manager::reference_counted_transaction_manager(
	const function<void ()>& start__,
	const function<void ()>& commit__,
	const function<void ()>& rollback__
) : start_(start__), commit_(commit__), rollback_(rollback__) {
}

void
reference_counted_transaction_manager::start() {

	if(count == 0) {

		start_();
	}

	++count;
}

void
reference_counted_transaction_manager::commit() {

	if(count == 0) {

		throw exception("commit called with no active transaction");
	}

	if(count == 1) {

		commit_();
	}
	
	--count;	
}

void
reference_counted_transaction_manager::rollback() {

	if(count == 0) {

		throw exception("rollback called with no active transaction");
	}

	rollback_();

	count = 0;
}
}
}
