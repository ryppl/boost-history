#include "stdafx.h"
#include "transaction.hpp"

#include "database.hpp"
#include "exception.hpp"

namespace boost {

namespace db {

transaction::transaction(const database& d_) : d(d_) {

	d.start_transaction();
}

transaction::~transaction() {

	if(!committed) {

		try {

			d.rollback_transaction();
		}
		catch(const exception&) {
		}
	}
}

void
transaction::commit() {

	d.commit_transaction();

	committed = true;
}
}
}