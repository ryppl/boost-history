#ifndef BOOST_UNICODE_TEST_RANGE_TEST_HPP
#define BOOST_UNICODE_TEST_RANGE_TEST_HPP

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <string>
#include <sstream>

namespace detail
{

template<typename Range>
std::string to_string(const Range& range)
{
    std::stringstream ss;
    bool first = true;
    ss << "[";
    for(
        typename boost::range_iterator<
            const Range
        >::type it = boost::begin(range);
        it != boost::end(range);
        ++it
    )
    {
        if(first)
            first = false;
        else
            ss << ", ";
            
        ss << "0x" << std::hex << *it;
    }
    ss << "]";
    
    return ss.str();
}

template<typename Range>
boost::reverse_range<
    const Range
>
reversed(const Range& range)
{
    return boost::adaptors::reverse(range);
}

} // namespace detail

#define CHECK_EQUALS(range1, range2) \
BOOST_CHECK_MESSAGE( \
    boost::equal(range1, range2), \
    #range1 << " != " << #range2 << " -- (" << detail::to_string(range1) << " != " << detail::to_string(range2) << ")" \
)

/* Checking in both directions allows to make sure the right-to-left
 * part of the pipe also works */
#define CHECK_BI_EQUALS(range1, range2) \
do { \
    using detail::reversed; \
    CHECK_EQUALS(range1, range2); \
    CHECK_EQUALS( \
        reversed(range1), \
        reversed(range2) \
    ); \
} while(0)

#endif
