// Boost.Print library

// Copyright Jeffrey Faust 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <map>
#include <list>
#include <boost/explore.hpp>
#include <boost/explore/is_assoc_iter.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( is_assoc_iter_test, C, test_types )
{
    bool mapHasAssocIter = boost::explore::is_assoc_iter<std::map<int, double>::iterator>::type();
    BOOST_CHECK(mapHasAssocIter);

    bool multimapHasAssocIter = boost::explore::is_assoc_iter<std::multimap<std::string, bool>::iterator>::type();
    BOOST_CHECK(multimapHasAssocIter);

    bool vectorHasAssocIter = boost::explore::is_assoc_iter<std::vector<double>::iterator>::type();
    BOOST_CHECK(!vectorHasAssocIter);

    bool pointerHasAssocIter = boost::explore::is_assoc_iter<int*>::type();
    BOOST_CHECK(!pointerHasAssocIter);

    bool pairHasAssocIter = boost::explore::is_assoc_iter<std::pair<int, double> >::type();
    BOOST_CHECK(!pairHasAssocIter);

    bool listOfPairHasAssocIter = boost::explore::is_assoc_iter<std::list<std::pair<int, double> >::iterator >::type();
    BOOST_CHECK(!listOfPairHasAssocIter);
}
