#ifndef BOOST_DATABASE_ROW_SET_HPP_INCLUDED
#define BOOST_DATABASE_ROW_SET_HPP_INCLUDED

#include "iterator.hpp"
#include "row.hpp"
#include "field_description.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <vector>

namespace boost {

	namespace db {

		namespace detail {

			struct abstract_row_set;
		}

		struct row_set {

			explicit row_set(shared_ptr<detail::abstract_row_set>);

			typedef db::iterator	iterator;
			typedef iterator		const_iterator;

			unsigned int size() const;

			bool empty() const;

			iterator begin() const;
			iterator end() const;

			db::field_description field_description(unsigned int) const;
			db::field_description field_description(const std::string&) const;
			
			optional<unsigned int> field_index(const std::string&) const;

		private:

			shared_ptr<detail::abstract_row_set> pimpl;
		};
	}
}

#endif /* BOOST_DATABASE_ROW_SET_HPP_INCLUDED */
