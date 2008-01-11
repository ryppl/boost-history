//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_VALUE_TYPES_HPP)
#define BOOST_SQL_CLI_DETAIL_VALUE_TYPES_HPP

#include <string>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/optional.hpp>

#include <boost/sql_cli/null_value.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{

typedef boost::mpl::vector <
        signed char, unsigned char, 
        short, unsigned short,
        long, unsigned long,
        long long, unsigned long long,
        float, double, 
        std::string, std::wstring> value_types;

template <template <typename> class TemplateT>
struct add_template
{
	template <typename T>
	struct apply
	{
		typedef TemplateT<T> type;
	};
};

typedef boost::mpl::transform<value_types, add_template<boost::optional> >::type optional_types;    
typedef boost::mpl::transform<value_types, add_template<null_value> >::type null_types;    

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_VALUE_TYPES_HPP)
