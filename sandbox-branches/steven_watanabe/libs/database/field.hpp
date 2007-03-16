#ifndef BOOST_DATABASE_FIELD_HPP_INCLUDED
#define BOOST_DATABASE_FIELD_HPP_INCLUDED

#include <boost/variant/variant.hpp>

#include <string>

namespace boost {

	namespace db {

		typedef variant<bool,int,long long,double,std::string> field;
	}
}

#endif /* BOOST_DATABASE_FIELD_HPP_INCLUDED */
