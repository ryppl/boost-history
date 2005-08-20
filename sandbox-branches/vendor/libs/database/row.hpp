#ifndef BOOST_DATABASE_ROW_HPP_INCLUDED
#define BOOST_DATABASE_ROW_HPP_INCLUDED

#include "detail/convert_field.hpp"

#include "field.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

namespace boost {

	namespace db {

		namespace detail {

			struct abstract_row_set;
		}

		struct row {

			row(shared_ptr<detail::abstract_row_set>, unsigned int);

			unsigned int index;

			template <typename Type, typename Key> optional<Type> get(const Key& k) const {

				if(const optional<field> f = get_field(k)) {

					return detail::convert_field<Type>(*f);
				}

				return optional<Type>();
			}

			template <typename Type, typename Key> Type get(const Key& k, const Type& default_) const {

				if(const optional<Type> rval = get<Type>(k)) {

					return *rval;
				}

				return default_;
			}

			optional<field> get_field(unsigned int index) const;

			optional<field> get_field(const std::string& name) const;

		private:

			shared_ptr<detail::abstract_row_set> pimpl;
		};
	}
}

#endif /* BOOST_DATABASE_ROW_HPP_INCLUDED */
