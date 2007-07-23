// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/phoenix/support.hpp>
#include <boost/spirit/phoenix/operator/self.hpp>
#include <boost/spirit/phoenix/core/composite.hpp>
#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/detail/type_deduction.hpp>
#include <boost/spirit/phoenix/operator/detail/unary_eval.hpp>
#include <boost/spirit/phoenix/operator/detail/unary_compose.hpp>
#include <boost/spirit/phoenix/operator/detail/binary_eval.hpp>
#include <boost/spirit/phoenix/operator/detail/binary_compose.hpp>

#include <boost/utility/addressof.hpp>

#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/spirit/phoenix/core.hpp>
#include <boost/typeof/typeof.hpp>

#include <iostream>
#include <vector>

template<typename T>
class wrapper : public T
{
    public:
    void operator()()
    {
    }
    
};

template<typename T>
wrapper<T> *operator & (wrapper<T> &t)
{
    return boost::addressof(t);
}

struct test
{
};

struct test2 : public test {int x;};

struct test3
{
    int x;
    test t;
};

using namespace boost;

int main()
{
    test t;
    test2 t2;
    test3 t3;
    
    std::cout << sizeof(test) << std::endl;
    std::cout << sizeof(test2) << std::endl;
    std::cout << sizeof(test3) << std::endl;

    std::cout << sizeof(t) << std::endl;
    std::cout << sizeof(t2) << std::endl;
    std::cout << sizeof(t3) << std::endl;
    
    fusion::vector<int, int> vec;
    fusion::vector2<int, int> vec2;
    
    std::cout << sizeof(vec) << std::endl;
    std::cout << sizeof(vec2) << std::endl;    
    
    wrapper<BOOST_TYPEOF(phoenix::arg_names::arg1)> wrap;
    std::cout << sizeof(wrap) << std::endl;
    
    //std::vector<BOOST_TYPEOF(phoenix::arg_names::arg1)> unwrap_vec(100);
    std::vector<wrapper<BOOST_TYPEOF(phoenix::arg_names::arg1)> > wrap_vec(100);
    (*wrap_vec.begin())();
}
