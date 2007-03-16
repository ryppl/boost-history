#ifndef BOOST_DATABASE_ITERATOR_HPP_INCLUDED
#define BOOST_DATABASE_ITERATOR_HPP_INCLUDED

#include "row.hpp"

#include <boost/operators.hpp>
#include <boost/shared_ptr.hpp>

namespace boost {

	namespace db {

		namespace detail {

			struct abstract_row_set;
		}

		struct iterator : bidirectional_iterator_helper<iterator, const row> {

			iterator(shared_ptr<detail::abstract_row_set>, ptrdiff_t);

			bool operator==(const iterator&) const;

			iterator& operator++();
			iterator& operator--();

			const row& operator*() const;

			ptrdiff_t index() const;

		private:

			row r;
		};
	}
}

#endif /* BOOST_DATABASE_ITERATOR_HPP_INCLUDED */
