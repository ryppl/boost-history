#include "row.hpp"

#include "detail/abstract_row_set.hpp"

#include "exception.hpp"

namespace boost {

namespace db {

row::row(shared_ptr<detail::abstract_row_set> pimpl_, unsigned int index_)
: pimpl(pimpl_), index(index_) {
}

optional<field>
row::get_field(unsigned int field_index) const {

	return pimpl->get(field_index, index);
}

optional<field>
row::get_field(const std::string& name) const {

	if(optional<unsigned int> index = pimpl->field_index(name)) {

		return get_field(*index);
	}

	throw invalid_field_name(name);
}
}
}
