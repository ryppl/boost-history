#ifndef BOOST_DATABASE_EXCEPTION_HPP_INCLUDED
#define BOOST_DATABASE_EXCEPTION_HPP_INCLUDED

#include <stdexcept>

namespace boost {

	namespace db {

		struct exception : std::runtime_error {

			exception(const std::string& message)
				: std::runtime_error(message) {
			}
		};

		struct invalid_field_name : exception {

			invalid_field_name(const std::string& name_)
				: exception("invalid field name: " + name_), name(name_) {
			}

			const std::string name;
		};
	}
}

#endif /* BOOST_DATABASE_EXCEPTION_HPP_INCLUDED */
