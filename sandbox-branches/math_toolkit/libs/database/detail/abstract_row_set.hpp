#ifndef BOOST_DB_ABSTRACT_ROW_SET_HPP_INCLUDED
#define BOOST_DB_ABSTRACT_ROW_SET_HPP_INCLUDED

#include <boost/optional.hpp>

#include <string>

namespace boost {

	namespace db {

		struct field_description;

		namespace detail {

			struct abstract_row_set {

				virtual ~abstract_row_set() {}

				virtual optional<unsigned int> field_index(const std::string& name) const = 0;

				virtual db::field_description field_description(unsigned int index) const = 0;
				
				virtual unsigned int number_of_fields() const = 0;

				virtual unsigned int size() const = 0;

				virtual optional<field> get(unsigned int field_index, unsigned int row_index) const = 0;
			};
		}
	}
}

#endif /*  BOOST_DB_ABSTRACT_ROW_SET_HPP_INCLUDED */
