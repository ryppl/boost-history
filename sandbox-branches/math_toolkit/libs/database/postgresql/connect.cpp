#include "connect.hpp"

#define HAVE_STRDUP

#ifdef ERROR
#undef ERROR
#endif

#include "libpq-fe.h"
#include "postgres.h"
#include "catalog/pg_type.h"

#include "../exception.hpp"
#include "../reference_counted_transaction_manager.hpp"

#include "../detail/abstract_database.hpp"
#include "../detail/abstract_row_set.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

namespace boost {

namespace db {

namespace postgresql {

namespace {

	typedef shared_ptr< ::pg_conn> connection_ptr;

	typedef shared_ptr< ::pg_result> query_result_ptr;

	field_description::data_type determine_field_type(::Oid id) {

		switch(id) {

			case NUMERICOID :
			case FLOAT8OID	:
			case FLOAT4OID	: return field_description::double_;

			case BOOLOID	: return field_description::bool_;

			case INT8OID	: return field_description::int8;
                
			case INT4OID	:
			case INT2OID	: return field_description::int4;
		}

		return field_description::text;
	}

		

	struct row_set_imp : detail::abstract_row_set {

		explicit row_set_imp(query_result_ptr rows_) : rows(rows_) {}

		optional<unsigned int> field_index(const std::string& field_name) const {
		
			const int rval = ::PQfnumber(rows.get(), field_name.c_str());

			if(rval != -1) {

				return static_cast<unsigned int>(rval);
			}

			return boost::optional<unsigned int>();		
		}

		db::field_description field_description(unsigned int index) const {
		
			db::field_description rval;

			rval.name	= ::PQfname(rows.get(), index);
			rval.type	= determine_field_type(::PQftype(rows.get(), index));
			rval.size	= ::PQfsize(rows.get(), index);

			return rval;		
		}
		
		unsigned int number_of_fields() const {

			return static_cast<unsigned int>(::PQnfields(rows.get()));
		}

		unsigned int size() const {

			return static_cast<unsigned int>(::PQntuples(rows.get()));
		}

		optional<field> get(unsigned int field_index, unsigned int row_index) const {

			if(::PQgetisnull(rows.get(), row_index, field_index)) {

				return optional<field>();
			}
		
			const char* data = ::PQgetvalue(rows.get(), row_index, field_index);

			switch(field_description(field_index).type) {

			case field_description::bool_
				: return field(::strlen(data) > 0
				&& *data != '0' && *data != 'f' && *data != 'F' && *data != 'N' && *data != 'n');

			case field_description::int4 : return field(atoi(data));

			case field_description::int8 : return field(lexical_cast<long long>(data));

			case field_description::double_ : return field(::atof(data));
			}

			return field(std::string(data));	
		}

	private:

		query_result_ptr rows;
	};

	query_result_ptr
	execute_query(connection_ptr connection, const std::string& query) {

		if(::pg_result* rval = ::PQexec(connection.get(), query.c_str())) {

			return query_result_ptr(rval, ::PQclear);
		}

		throw exception("Error in query: " + query + " : " + ::PQerrorMessage(connection.get()));
	}

	connection_ptr make_connection(const std::string& connect_string) {

		connection_ptr rval(::PQconnectdb(connect_string.c_str()), ::PQfinish);

		if(::PQstatus(rval.get()) != CONNECTION_OK) {

			throw exception(::PQerrorMessage(rval.get()));
		}

		return rval;
	}

	struct database_impl : detail::abstract_database {

		explicit database_impl(const std::string& connect_string)
			:	connection(make_connection(connect_string)),
				transactions(
					call_execute("START TRANSACTION"),
					call_execute("COMMIT"),
					call_execute("ROLLBACK")
				) {
		}

		std::auto_ptr<detail::abstract_row_set> query(const std::string& s) const {

			query_result_ptr rval = execute_query(connection, s);

			if(::PQresultStatus(rval.get()) == PGRES_TUPLES_OK) {

				return std::auto_ptr<detail::abstract_row_set>(
					new row_set_imp(rval)
				);
			}

			throw exception("Error in query: " + s + " : " + ::PQresultErrorMessage(rval.get()));
		}

		execute_result execute(const std::string& s) const {
		
			query_result_ptr result = execute_query(connection, s);

			if(::PQresultStatus(result.get()) != PGRES_COMMAND_OK) {

				throw exception("error with query: " + s + " : " + ::PQresultErrorMessage(result.get()));
			}

			execute_result rval;

			const char* affected = ::PQcmdTuples(result.get());

			if(strlen(affected) > 0) {

				rval.rows_affected.reset(static_cast<unsigned int>(atoi(affected)));
			}

			const ::Oid id = ::PQoidValue(result.get());

			if(id != InvalidOid) {

				rval.inserted_object_id.reset(id);
			}

			return rval;	
		}

		void start_transaction() {

			transactions.start();
		}

		void commit_transaction() {

			transactions.commit();
		}
		
		void rollback_transaction() {

			transactions.rollback();
		}

	private:

		function<void ()> call_execute(const std::string& query) {

			return boost::bind(execute, this, query);
		}

		connection_ptr connection;

		reference_counted_transaction_manager transactions;
	};
}

database
connect(const std::string& connect_string) {

	return database(
		shared_ptr<detail::abstract_database>(new database_impl(connect_string))
	);
}
}
}
}
