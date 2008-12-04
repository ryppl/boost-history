// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/dataflow/managed/port.hpp>
#include <boost/dataflow/managed/network.hpp>
#include <boost/assign/std/vector.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df=boost::dataflow;

// source just provides an output port
class source : public df::managed::component
{
public:
    source(df::managed::network &network, int value)
        : df::managed::component(network), output(*this)
    {
        output.set(value);
    }
    df::managed::port<int, df::ports::producer> output;
};

// adder has two input ports and an output port.
// invoking adds the two inputs into the output.
class adder : public df::managed::component
{
public:
    adder(df::managed::network &network)
        : component(network), output(*this), input1(*this), input2(*this)
        , invoke_count(0)
    {}
    void invoke()
    {
        invoke_count++;
        output.set(input1.get()+input2.get());
    }

    df::managed::port<int, df::ports::producer> output;
    df::managed::port<int, df::ports::consumer> input1;
    df::managed::port<int, df::ports::consumer> input2;
    unsigned invoke_count;
};

// multiplier has two input ports and an output port.
// invoking multipilies the two inputs into the output.
class multiplier : public df::managed::component
{
public:
    multiplier(df::managed::network &network)
        : component(network), output(*this), input1(*this), input2(*this)
        , invoke_count(0)
    {}
    void invoke()
    {
        invoke_count++;
        output.set(input1.get()*input2.get());
    }

    df::managed::port<int, df::ports::producer> output;
    df::managed::port<int, df::ports::consumer> input1;
    df::managed::port<int, df::ports::consumer> input2;
    unsigned invoke_count;
};

BOOST_AUTO_TEST_CASE( test ) 
{
    // our network
    df::managed::network network;
    
    // our four sources with initial values
    source sourceA(network, 0);
    source sourceB(network, 1);
    source sourceC(network, 2);
    source sourceD(network, 3);
    
    // two adders and a multiplier
    adder AplusB(network), CplusD(network);
    multiplier ApBtimesCpD(network);
    
    // ---Connect the dataflow network ---------------------
    //
    //     ,---------.       ,---.
    //     | sourceA | ----> | A |       ,---.
    //     `---------'       | + | ----> | A |
    //     ,---------.       | B |       | + |
    //     | sourceB | ----> |   |       | B |
    //     `---------'       `---'       |   |
    //     ,---------.       ,---.       | * |
    //     | sourceC | ----> |   |       |   |
    //     `---------'       | C |       | C |
    //     ,---------.       | + | ----> | + |
    //     | sourceD | ----> | D |       | D |
    //     `---------'       `---'       `---'
    //
    // -----------------------------------------------------
    connect(sourceA.output, AplusB.input1);
    connect(sourceB.output, AplusB.input2);
    connect(sourceC.output, CplusD.input1);
    connect(sourceD.output, CplusD.input2);
    connect(AplusB.output, ApBtimesCpD.input1);
    connect(CplusD.output, ApBtimesCpD.input2);
    
    using namespace boost::assign;
    
    // provide a hard coded topological sort for now
//    std::vector<df::managed::component *> topological_sort;
//    topological_sort += &sourceA, &sourceB, &sourceC, &sourceD, &AplusB, &CplusD, &ApBtimesCpD; 
//    network.topological_sort(topological_sort);

    
    // update will invoke all components that need to be invoked
    network.update();
    BOOST_CHECK_EQUAL(ApBtimesCpD.output.get(), (0+1)*(2+3));
    BOOST_CHECK_EQUAL(AplusB.invoke_count, 1u);
    BOOST_CHECK_EQUAL(CplusD.invoke_count, 1u);
    BOOST_CHECK_EQUAL(ApBtimesCpD.invoke_count, 1u);
    
    // change the output of sourceA
    sourceA.output.set(4);
    network.update();
    BOOST_CHECK_EQUAL(ApBtimesCpD.output.get(), (4+1)*(2+3));
    BOOST_CHECK_EQUAL(AplusB.invoke_count, 2u);
    // C and D did not change, hence CplusD should not get invoked!
    BOOST_CHECK_EQUAL(CplusD.invoke_count, 1u);
    BOOST_CHECK_EQUAL(ApBtimesCpD.invoke_count, 2u);
    
    sourceC.output.set(5);
    sourceD.output.set(6);
    network.update();
    BOOST_CHECK_EQUAL(ApBtimesCpD.output.get(), (4+1)*(5+6));
    // A and B did not change, hence AplusB should not get invoked!
    BOOST_CHECK_EQUAL(AplusB.invoke_count, 2u);
    BOOST_CHECK_EQUAL(CplusD.invoke_count, 2u);
    BOOST_CHECK_EQUAL(ApBtimesCpD.invoke_count, 3u);
    
} // int test_main(int, char* [])



