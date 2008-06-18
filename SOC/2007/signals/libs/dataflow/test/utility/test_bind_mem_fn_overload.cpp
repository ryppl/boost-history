//[ test_bind_mem_fn

// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/utility/bind_mem_fn_overload.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

class sum
{
public:
    sum(int base)
        : m_base(base)
    {}
    
    int add(int num) const
    {   return m_base + num; }
    
    int add(int num1, int num2) const
    
    {   return m_base + num1 + num2; }

private:
    int m_base;
};

int test_main(int, char* [])
{
    using namespace boost;
    using boost::dataflow::utility::bind_mem_fn_overload;
    using boost::dataflow::utility::bind_mem_fn_const_overload;
    
    sum test_sum(1);
    
    boost::function<int(int)> fun1 = bind_mem_fn_const_overload<int(int)>(&sum::add, test_sum);
    boost::function<int(int, int)> fun2 = bind_mem_fn_const_overload<int(int, int)>(&sum::add, test_sum);
    
    BOOST_CHECK_EQUAL(fun1(2), 3);
    BOOST_CHECK_EQUAL(fun2(2, 3), 6);
    
    const sum const_test_sum(2);

    boost::function<int(int)> cfun1 = bind_mem_fn_overload<int(int)>(&sum::add, const_test_sum);
    boost::function<int(int, int)> cfun2 = bind_mem_fn_overload<int(int, int)>(&sum::add, const_test_sum);

    BOOST_CHECK_EQUAL(cfun1(2), 4);
    BOOST_CHECK_EQUAL(cfun2(2, 3), 7);

    return 0;
} // int test_main(int, char* [])

//]
