// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


//[ edit_distance_example
#include <boost/dataflow/phoenix/producer_container.hpp>
#include <boost/dataflow/phoenix/connection/iterator_relative.hpp>
#include <boost/dataflow/phoenix/components.hpp>
#include <boost/dataflow/phoenix/static_function.hpp>

#include <iostream>
#include <boost/array.hpp>
#include <boost/fusion/sequence/generation/make_vector.hpp>
#include <boost/spirit/phoenix/core/argument.hpp>
#include <boost/spirit/phoenix/core/reference.hpp>
#include <boost/spirit/phoenix/core/value.hpp>
#include <boost/spirit/phoenix/function/function.hpp>
#include <boost/spirit/phoenix/operator/self.hpp>
#include <boost/typeof/typeof.hpp>
#include <functional>
#include <algorithm>
#include <vector>

namespace impl
{
    struct min_impl
    {
        template <typename T1, typename T2>
        struct result
        {
            typedef T1 type;
        };
        
        template <typename T1, typename T2>
        T1 operator()(T1 t1, T2 t2) const
        {
            return (std::min)(t1, t2);
        }
    };
    
    struct min3_impl
    {
        template <typename T1, typename T2, typename T3>
        struct result
        {
            typedef T1 type;
        };
        
        template <typename T1, typename T2, typename T3>
        T1 operator()(T1 t1, T2 t2, T3 t3) const
        {
            return (std::min)((std::min)(t1, t2), t3);
        }
    };
    
}

using namespace boost;
using namespace boost::phoenix::iterator_relative;
using namespace boost::phoenix::arg_names;

const phoenix::static_function< ::impl::min_impl> min = phoenix::static_function< ::impl::min_impl>();
const phoenix::static_function< ::impl::min3_impl> min3 = phoenix::static_function< ::impl::min3_impl>();

class edit_distance_matrix
{
public:
    edit_distance_matrix(const char *str) : string_b(str)
    {
        for (int i=0; i<3; i++)
            rows[i].resize(string_b.size()+1);
        distance.resize(string_b.size()+1);
        for (std::vector<double>::iterator it= rows[0].begin()+1; it!=rows[0].end(); it++)
            *it = it - rows[0].begin();
        current_row = prev_row = 0;
        display();
        current_row = 1;
    }
    void process(char c)
    {
        std::string::iterator it_b = string_b.begin();
        for (distance_type::iterator it=distance.begin()+1; it!=distance.end(); it++, it_b++)
        {
            *it = !(*it_b == c);
            std::cout << "-" << *it;
        }
        std::cout << std::endl;
        rows[current_row].eval(rows[prev_row],distance);
        display();
        prev_row = current_row;
        if (++current_row == 3)
            current_row = 0;
    }
    void display()
    {
        for (std::vector<double>::iterator it=rows[current_row].begin();
             it!=rows[current_row].end(); it++)
            std::cout << *it << " ";
        std::cout << std::endl;
    }
private:
    typedef std::vector<double> distance_type;
    
    distance_type distance;
    std::string string_b;
    phoenix::producer_container<
        std::vector<double>,
        BOOST_TYPEOF(min3(prev<1>(arg1) + phoenix::int_<1>(),\
                         prev<1>(arg2) + prev<0>(arg3),\
                         prev<0>(arg2) + phoenix::int_<1>())),
        mpl::vector<
            BOOST_TYPEOF(prev<0>(arg2) + phoenix::int_<1>())
        > > rows[3];
    size_t current_row, prev_row;
};

int main (int argc, char * const argv[])
{
    edit_distance_matrix m("hello");
    m.process('l');
    m.process('l');
    m.process('l');
    m.process('l');
        
    return 0;
}

//]
