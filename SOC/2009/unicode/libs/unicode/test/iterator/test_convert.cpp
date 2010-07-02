#define BOOST_TEST_MODULE Pipe
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include <boost/iterator/pipe_iterator.hpp>
#include <boost/range/distance.hpp>

#include <boost/assert.hpp>

#include <vector>
#include <list>
#include <iterator>

struct increasing_pipe
{
    typedef int input_type;
    typedef int output_type;
    
    increasing_pipe() : count(1)
    {
    }
    
    template<typename In, typename Out>
    std::pair<In, Out> ltr(In begin, In end, Out out)
    {
        for(int i=0; i<count; i++)
            *out++ = *begin + i;
            
        if(++begin != end)
            ++count;
            
        return std::make_pair(begin, out);
    }
    
    template<typename In, typename Out>
    std::pair<In, Out> rtl(In begin, In end, Out out)
    {
        --end;
        for(int i=0; i<count; i++)
            *out++ = *end + i;

        if(end != begin)
            --count;
            
        return std::make_pair(end, out);
    }
    
    int count;
};

BOOST_AUTO_TEST_CASE( increasing_test )
{
    using boost::pipe_iterator;
    using boost::make_pipe_iterator;
    
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    pipe_iterator<int*, increasing_pipe> begin =
        make_pipe_iterator(data, data + sizeof data / sizeof data[0], data, increasing_pipe());
        
    pipe_iterator<int*, increasing_pipe> end =
        make_pipe_iterator(data, data + sizeof data / sizeof data[0], data + sizeof data / sizeof data[0], increasing_pipe());
        
    pipe_iterator<int*, increasing_pipe> it = begin;
    int count = 1;
    for(; it != end; ++count)
        for(int i=0; i<count; ++i, ++it)
            BOOST_CHECK_EQUAL(*it, data[count-1]+i);
        
        
    --it;
    --count;
    for(;;)
    {
        for(int i=count-1; i>=0; --i, --it)
            BOOST_CHECK_EQUAL(*it, data[count-1]+i);
        
        if(it == begin)
            break;
            
        --count;
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
    boost::pipe_iterator<T, increasing_pipe>();
}
