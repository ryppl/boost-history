#include "iterator.hpp"

namespace boost {

namespace db {

iterator::iterator(shared_ptr<detail::abstract_row_set> r_, ptrdiff_t index)
: r(row(r_, static_cast<unsigned int>(index))) {
}

bool
iterator::operator==(const iterator& rhs) const {

	return index() == rhs.index();
}

iterator&
iterator::operator++() {

	++r.index;

	return *this;
}

iterator&
iterator::operator--() {

	--r.index;

	return *this;
}

const row&
iterator::operator*() const {

	return r;
}

ptrdiff_t
iterator::index() const {

	return r.index;
}
}
}
