// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/connection/detail/result_of_defined.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/dataflow/signal/component/junction.hpp>
#include <boost/dataflow/signal/component/storage.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

struct nothing
{
};

struct has_result_type
{
    typedef void result_type;
};

struct has_result
{
    template<typename FArgs>
    struct result;
    
    template<typename T>
    struct result<has_result(T)>
    {
        typedef void type;
    };
};

int test_main(int, char* [])
{
//    bool value = signals::detail::result_of_defined<fusion::map<>(void)>::value;
    BOOST_CHECK(!signals::detail::result_of_defined<fusion::map<>(int)>::value);
    BOOST_CHECK(!signals::detail::result_of_defined<fusion::map<>()>::value);
    typedef boost::fusion::map<
        boost::fusion::pair<
            void (void),
            boost::signals::slot_selector<void (void),boost::signals::storage<void (float),boost::signals::unfused> >
        >,
        boost::fusion::pair<
            void (const boost::fusion::vector<> &),
            boost::signals::slot_selector<void (const boost::fusion::vector<> &),boost::signals::storage<void (float),boost::signals::unfused> >
        >
    > send_slot_map_type;
    BOOST_CHECK(!signals::detail::result_of_defined<send_slot_map_type()>::value);
    BOOST_CHECK(!signals::detail::result_of_defined<nothing(int)>::value);
    BOOST_CHECK(!signals::detail::result_of_defined<nothing()>::value);
    BOOST_CHECK(signals::detail::result_of_defined<has_result_type(int)>::value);
    BOOST_CHECK(signals::detail::result_of_defined<has_result_type()>::value);
    BOOST_CHECK(signals::detail::result_of_defined<has_result(int)>::value);
    BOOST_CHECK(!signals::detail::result_of_defined<has_result()>::value);
    BOOST_CHECK(!signals::detail::result_of_defined<has_result(float, float)>::value);
    bool junction_void_value = signals::detail::result_of_defined<
        signals::junction<void(), signals::unfused> ()>::value;
    BOOST_CHECK(junction_void_value);
//    bool junction_int_value = signals::detail::result_of_defined<
//        signals::junction<void(), signals::unfused> (int)>::value;
//    BOOST_CHECK(!junction_int_value);
    
   return 0;
} // int test_main(int, char* [])
