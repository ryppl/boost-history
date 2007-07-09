// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/signal_network/function.hpp>
//#include <boost/signal_network/storage.hpp>

#include <boost/fusion/sequence/container/map.hpp>
#include <iostream>

#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/view/transform_view.hpp>
#include <boost/fusion/sequence/intrinsic.hpp>
#include <boost/fusion/sequence/io.hpp>

#include <boost/type_traits/add_reference.hpp>

namespace fusion = boost::fusion;

//using namespace boost;

float DoublerFunc(float x)
{
	return x*2;
}

namespace dataflow
{
    template<typename X>
    struct print;
};

namespace dataflow
{
    template<>
    struct print<int>
    {
        void operator()(int x)
        {
            std::cout << x << std::endl;
        }
    };
}

namespace specific
{
    struct print : public dataflow::print<int> {};
}

int main(int, char* [])
{
    specific::print()(3);
    
    /*
    signals::storage<void (), signals::unfused> banger;
    signals::function<void (float), float(float), signals::unfused> double_fun1(&DoublerFunc);
    signals::function<void (float), float(float), signals::unfused> double_fun2(&DoublerFunc);
    signals::storage<void (float), signals::unfused> floater(1.0f);
    signals::storage<void (float), signals::unfused> collector(0.0f);*/
    
/*	floater >>= double_fun1 >>= double_fun2 >>= collector;
	floater.send();
    
    assert(collector.at<0>() == 4.0f);
    
    floater.disconnect_all_slots();*/
    
/*    banger >>= boost::fusion::at_key<void()> (floater.send_slot());
    typedef 
    boost::fusion::map<
    boost::fusion::pair<void(), slot_selector_t<signals::storage<void (float), signals::unfused>, void()> >,
    boost::fusion::pair<void(const boost::fusion::vector<> &), slot_selector_t<signals::storage<void (float), signals::unfused>, void (const boost::fusion::vector<> &)> >
        > map_type;
    
    map_type m = floater.send_slot();
    
//    int x = default_slot<map_type, void()>()(m);
    floater >>= collector;
//    banger.send();
    floater.send();
    assert(collector.at<0>() == 1.0f);*/
    
    return 0;
} // int main(int, char* [])