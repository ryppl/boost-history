#define BOOST_TEST_MODULE Consumer
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include <boost/iterator/consumer_iterator.hpp>
#include <boost/range/distance.hpp>

#include <boost/assert.hpp>

#include <vector>
#include <list>
#include <iterator>

struct increasing_consumer
{
    typedef int input_type;
    
    increasing_consumer() : count(1)
    {
    }
    
    template<typename In>
    In ltr(In begin, In end)
    {
        BOOST_ASSERT(std::distance(begin, end) >= count);
        
        for(int i=0; i<count; i++)
            ++begin;
            
        if(begin != end)
            ++count;
        return begin;
    }
    
    template<typename In>
    In rtl(In begin, In end)
    {
        BOOST_ASSERT(std::distance(begin, end) >= count);
        
        for(int i=0; i<count; i++)
            --end;

        if(end != begin)
            --count;
        return end;
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
    using boost::consumer_iterator;
    using boost::make_consumer_iterator;
    
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    consumer_iterator<int*, increasing_consumer> begin =
        make_consumer_iterator(data, data + sizeof data / sizeof data[0], data, increasing_consumer());
        
    consumer_iterator<int*, increasing_consumer> end =
        make_consumer_iterator(data, data + sizeof data / sizeof data[0], data + sizeof data / sizeof data[0], increasing_consumer());
        
    int count = 1;
    int acc = count;
    consumer_iterator<int*, increasing_consumer> it = begin;
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
    boost::consumer_iterator<T, increasing_consumer>();
}


