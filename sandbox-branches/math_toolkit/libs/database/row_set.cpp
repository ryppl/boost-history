#include "row_set.hpp"
#include "exception.hpp"

#include "detail/abstract_row_set.hpp"

namespace boost {

namespace db {

row_set::row_set(shared_ptr<detail::abstract_row_set> pimpl_) : pimpl(pimpl_) {
}

unsigned int
row_set::size() const {

	return pimpl->size();
}

bool
row_set::empty() const {

	return size() == 0;
}

iterator
row_set::begin() const {

	return iterator(pimpl, 0);
}

iterator
row_set::end() const {

	return iterator(pimpl, size());
}

db::field_description
row_set::field_description(unsigned int index) const {

	return pimpl->field_description(index);
}

db::field_description
row_set::field_description(const std::string& field_name) const {

	if(optional<unsigned int> index = field_index(field_name)) {

		return field_description(*index);
	}

	throw invalid_field_name(field_name);
}

optional<unsigned int>
row_set::field_index(const std::string& field_name) const {
	
	return pimpl->field_index(field_name);
}
}
}
