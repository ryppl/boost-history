/**
 * \file examples/factories/yes_no_dlg.cpp
 *
 *  This example shows how to use the generic function callers.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_EXAMPLES_FACTORIES_YES_NO_DLG_HPP
#define BOOST_MIRROR_EXAMPLES_FACTORIES_YES_NO_DLG_HPP

#include <boost/char_type_switch/iostream.hpp>

const bool yes_no_dlg(const ::boost::cts::bchar* msg)
{
	using namespace ::boost;
	const cts::bchar yes = BOOST_CTS_LIT('y');
	const cts::bchar no = BOOST_CTS_LIT('n');
	cts::bchar result;
	do
	{
		cts::bcout() << msg << ::std::flush;
		cts::bcin() >> result;
		cts::bcin().ignore();
	} while(result != yes && result != no);
	return result == yes;
}

#endif

