#ifndef BOOST_DATABASE_REFERENCE_COUNTED_TRANSACTION_MANAGER_HPP_INCLUDED
#define BOOST_DATABASE_REFERENCE_COUNTED_TRANSACTION_MANAGER_HPP_INCLUDED

#include <boost/function.hpp>
#include <boost/utility.hpp>

namespace boost {

	namespace db {

		struct reference_counted_transaction_manager : noncopyable {

			reference_counted_transaction_manager(
				const function<void ()>& start,
				const function<void ()>& commit,
				const function<void ()>& rollback
			);

			void start();
			void commit();
			void rollback();

		private:

			const function<void ()> start_, commit_, rollback_;

			int count;
		};
	}
}

#endif /* BOOST_DATABASE_REFERENCE_COUNTED_TRANSACTION_MANAGER_HPP_INCLUDED */
