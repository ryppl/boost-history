
#include <boost/assert.hpp>
#include <boost/unordered_pair.hpp>

using namespace boost;

int main()
{
    unordered_pair<int> a(5, 10), b(10, 5);
    BOOST_ASSERT(a.first() == 5 && a.second() == 10);
    BOOST_ASSERT(b.first() == 5 && b.second() == 10);
    return 0;
}
