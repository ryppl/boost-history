// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ example_fibonacci

#include <boost/dataflow/connection/operators.hpp>
#include <boost/dataflow/phoenix/components.hpp>
#include <boost/dataflow/phoenix/producer_container.hpp>
#include <boost/dataflow/phoenix/connection/iterator_relative.hpp>
#include <boost/dataflow/phoenix/sequence_environment.hpp>

#include <boost/spirit/phoenix/core/argument.hpp>
#include <boost/spirit/phoenix/core/value.hpp>
#include <boost/spirit/phoenix/function/function.hpp>
#include <boost/spirit/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/phoenix/operator/self.hpp>
#include <boost/progress.hpp>
#include <boost/typeof/typeof.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace boost::dataflow::operators;
using namespace boost::phoenix::arg_names;
using namespace boost::phoenix::iterator_relative;

typedef int fib_type;

// This example shows a number of completely unnecessary ways
// to implement a fibonacci sequence generator.
// Each method uses different ways of connecting the "cells", i.e. elements
// of the sequence.
//
// Conceptually, each cell has two inputs (the previous two cells)
// and the function that combines them into this cell's value (plus).
// Most of the methods here are complete overkill because they provide
// runtime reconnection of cells - which is not necessary here.
// the relative_link_fibonacci and the producer_container examples
// are the closest to capturing the needs of the problem.
// (apart from sane_fibonacci)


// this one uses a persistent environment (lives in the args)
// combined with a phoenix function that adds the pointers that live in
// the args
void phoenix_simple_fibonacci(int n)
{
    // link to two cells - pointers and the function that joins their pointees
    typedef phoenix::sequence_environment<
        dataflow::consumer_group<
            fusion::vector2<const fib_type *, const fib_type *> >,
        BOOST_TYPEOF(*arg1 + *arg2)
    > link_type;

    std::cout << sizeof(phoenix::producer<fib_type, link_type>) << std::endl;

    std::vector<phoenix::producer<fib_type, link_type> > cells(n);

    for (int i=2; i<n; i++)
        // connect the previous two cells to this one
        (cells[i-1] & cells[i-2]) >>= cells[i];
    
    // and run the calculation
    {
        boost::progress_timer t;
        
        cells[0].value = 0;
        cells[1].value = 1;
        for (int i=2; i<n; i++)
            cells[i]();
    }
}

// this one is using the producer_accumulator that takes care of the
// pointing to previous cells via two embedded consumers.
void phoenix_fibonacci(int n)
{
    // the producer_accumulator automaticall accumulates the links
    typedef phoenix::producer_accumulator<std::plus<fib_type>,
        dataflow::consumer_group<
            fusion::vector2<
                phoenix::actor<phoenix::consumer<fib_type> >,
                phoenix::actor<phoenix::consumer<fib_type> > >
        > > link_type;

    std::cout << sizeof(phoenix::producer<fib_type, link_type>) << std::endl;

    std::vector<phoenix::producer<fib_type, link_type> > cells(n);
    
    for (int i=2; i<n; i++)
        // connect the previous two cells to this one
        (cells[i-1] & cells[i-2]) >>= cells[i];
    
    {
        boost::progress_timer t;
        
        cells[0].value = 0;
        cells[1].value = 1;
        for (int i=2; i<n; i++)
            cells[i]();
    }
    
}

// this one is a hand-coded mimic of the above two
struct cell
{
    typedef dataflow::phoenix_producer producer_category;
    typedef fib_type produced_type;
    
    fib_type value;
    const fib_type *ptr1;
    const fib_type *ptr2;
    void operator()()
    {
        value = *ptr1 + *ptr2;
    }
};

void mimic_fibonacci(int n)
{
    
    std::cout << sizeof(cell) << std::endl;
    
    std::vector<cell> cells(n);
    
    for (int i=2; i<n; i++)
    {
        // even though this is hand coded, we still can't resist the >>=
        cells[i-2] >>= cells[i].ptr1;
        cells[i-1] >>= cells[i].ptr2;
    }
    
    {
        boost::progress_timer t;
        
        cells[0].value = 0;
        cells[1].value = 1;
        for (int i=2; i<n; i++)
            cells[i]();
    }    
}

// This one uses relative links embedded in the vector type
// * prev2 refers to the value two back from the supplied iterator,
//   and prev1 refers to the value one back from the supplied iterator.
// Relative links do not need to be stored.
void relative_link_fibonacci(int n)
{
    // each element of the vector contains relative links to the
    // previous two elements
    typedef std::vector<
            phoenix::static_producer<fib_type, BOOST_TYPEOF(prev2 + prev1) >
        > cells_type;
    
    std::cout << sizeof(phoenix::static_producer<fib_type, BOOST_TYPEOF(prev2 + prev1)>) << std::endl;
                        
    cells_type cells(n);
    
    {
        boost::progress_timer t;
        
        cells[0].value = 0;
        cells[1].value = 1;
        for (cells_type::iterator it=cells.begin()+2; it!=cells.end(); it++)
            (*it)(it);
    }
}

// This one uses relative links embedded in a producer_container wrapper.
// * prev2 refers to the value two back from the supplied iterator,
//   and prev1 refers to the value one back from the supplied iterator.
// Relative links do not need to be stored.
void producer_container_fibonacci(int n)
{
    typedef boost::phoenix::producer_container<
        std::vector<fib_type>, // container
        BOOST_TYPEOF(prev2 + prev1), // general rule
        mpl::vector< // init cases
            BOOST_TYPEOF(phoenix::int_<0>()),
            BOOST_TYPEOF(phoenix::int_<1>())
        > > cells_type;

    cells_type cells(n);
    
    {
        boost::progress_timer t;
        
        cells.eval();
    }
}

// This would be what a sane person would write.
void sane_fibonacci(int n)
{
    std::vector<fib_type> cells(n);

    {
        boost::progress_timer t;
        
        cells[0] = 0;
        cells[1] = 1;
        for (int i=2; i<n; i++)
            cells[i] = cells[i-1] + cells[i-2];
    }
}

int main (int argc, char * const argv[])
{
    // never mind the overflow
    int len = 5000000;
    
    // display the cell sizes and times...
    // most of the time comes from memory access (cell size)
    phoenix_simple_fibonacci(len);
    phoenix_fibonacci(len);
    mimic_fibonacci(len);
    relative_link_fibonacci(len);
    producer_container_fibonacci(len);
    sane_fibonacci(len);
    return 0;
}
//]