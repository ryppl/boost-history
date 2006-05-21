#ifndef BOOST_DATABASE_FIELD_DESCRIPTION_HPP_INCLUDED
#define BOOST_DATABASE_FIELD_DESCRIPTION_HPP_INCLUDED

#include <string>

namespace boost {

	namespace db {

		struct field_description {

			std::string name;

			enum data_type { bool_, int4, int8, double_, text};

			data_type type;

			unsigned int size;
		};
	}
}

#endif /* BOOST_DATABASE_FIELD_DESCRIPTION_HPP_INCLUDED */
