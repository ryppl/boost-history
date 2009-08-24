#define BOOST_TEST_MODULE Join
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include <boost/iterator/join_iterator.hpp>
#include <boost/range/as_array.hpp>

#include "../unicode/range_test.hpp"

BOOST_AUTO_TEST_CASE ( join )
{
    int one[] = {1, 2, 3, 4};
    std::vector<int> two;
    two.push_back(5); two.push_back(6);
    std::list<int> three;
    three.push_back(7); three.push_back(8); three.push_back(9);
    int four = 10;
    
    int sum[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    CHECK_BI_EQUALS(
        boost::joined_n(
            boost::as_array(one),
            two,
            three,
            boost::make_iterator_range(&four, &four + 1)
        ),
        sum
    );
}

typedef boost::mpl::list<
    int*,
    const int*,
    std::vector<int>::iterator,
    std::list<int>::iterator,
    std::istream_iterator<char, char>
> instantiate_types;

BOOST_AUTO_TEST_CASE_TEMPLATE( instantiate, T, instantiate_types )
{
}
