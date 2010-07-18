#define BOOST_TEST_MODULE Segmenter
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include <boost/iterator/segment_iterator.hpp>
#include <boost/range/distance.hpp>

#include <boost/assert.hpp>

#include <vector>
#include <list>
#include <iterator>

struct increasing_segmenter
{
    typedef int input_type;
    
    increasing_segmenter() : count(1)
    {
    }
    
    template<typename In>
    void ltr(In& begin, In end)
    {
        BOOST_ASSERT(std::distance(begin, end) >= count);
        
        for(int i=0; i<count; i++)
            ++begin;
            
        if(begin != end)
            ++count;
    }
    
    template<typename In>
    void rtl(In begin, In& end)
    {
        BOOST_ASSERT(std::distance(begin, end) >= count);
        
        for(int i=0; i<count; i++)
            --end;

        if(end != begin)
            --count;
    }
    
    int count;
};

template<typename Range>
void check_increasing_sequence(const Range& range, int acc)
{
    for(
        typename boost::range_iterator<
            const Range
        >::type it = boost::begin(range);
        it != boost::end(range);
        ++it, ++acc
    )
        BOOST_CHECK_EQUAL(*it, acc);
        
}

BOOST_AUTO_TEST_CASE( increasing_test )
{
    using boost::segment_iterator;
    using boost::make_segment_iterator;
    
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    segment_iterator<int*, increasing_segmenter> begin =
        make_segment_iterator(data, data + sizeof data / sizeof data[0], data, increasing_segmenter());
        
    segment_iterator<int*, increasing_segmenter> end =
        make_segment_iterator(data, data + sizeof data / sizeof data[0], data + sizeof data / sizeof data[0], increasing_segmenter());
        
    int count = 1;
    int acc = count;
    segment_iterator<int*, increasing_segmenter> it = begin;
    for(; it != end; ++it, acc += count, ++count)
    {
        BOOST_CHECK_EQUAL(boost::distance(*it), count);
        check_increasing_sequence(*it, acc);
    }
        
    --it;
    --count;
    acc -= count;
    for(;;)
    {
        BOOST_CHECK_EQUAL(boost::distance(*it), count);
        check_increasing_sequence(*it, acc);
        
        if(it == begin)
            break;
            
        --it;
        --count;
        acc -= count;
    }
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
    boost::segment_iterator<T, increasing_segmenter>();
}


