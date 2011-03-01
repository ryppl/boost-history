#include <tests/translators.hpp>
#include <tests/rtree_native.hpp>
#include <tests/rtree_filters.hpp>
//#include <tests/rtree_iterator.hpp>

int main()
{
    tests_translators_hpp();
    tests_rtree_native_hpp();
    tests_rtree_filters_hpp();
    //tests_rtree_iterator_hpp();

#ifdef _MSC_VER
    std::cin.get();
#endif

    return 0;
}
