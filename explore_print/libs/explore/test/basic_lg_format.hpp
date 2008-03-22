// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost/mpl/if.hpp>
#include <boost/mpl/list.hpp>

#include <boost/explore.hpp>

struct basic_lg_range_format : explore::basic_range_format
{
	static char_ *opening() { return "<=";}
	static char_ *closing() { return "=>";}
	static char_ delimiter() { return '#';}
};

struct basic_lg_range_format_selector
{
	template< typename T>
	struct range_format
	{
		typedef basic_lg_range_format type;
	};
};

struct basic_lg_format : boost::mpl::list< basic_lg_range_format_selector> {};