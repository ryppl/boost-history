//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/synchro/queues/synchro_buffer_family.hpp>
#include <boost/synchro/thread_synchronization_family.hpp>
#include <boost/synchro/process_synchronization_family.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace boost::unit_test;
using namespace boost::synchro;

typedef sync_buffer<int, 10, boost::synchro::thread_synchronization_family> MyBuffer;
MyBuffer gbuf;
boost::mutex gmtx;

void producer(int base)
{
    for(int i = 0; i<10; i++) {
//        thread::sleep(i);
//        {
//        boost::unique_lock<boost::mutex> l(gmtx);
//        std::cout << "buf.push("<<base+i<<");"<<std::endl;
//        }
        gbuf.push(base+i);
    }
}

void consumer() {
    for(int i = 0; i<90; i++) {
//        thread::sleep(i);
        int j;
        j= gbuf.pull();
//        {
//        boost::unique_lock<boost::mutex> l(gmtx);
//        std::cout << "buf.pull() -> "<<j<<" "<<i<< std::endl;
//        }
        boost::this_thread::sleep(boost::posix_time::seconds(1));

//        if (j!= i) return;
    }
}

void test_1() {
    boost::thread thread0(consumer); // start concurrent execution of consumer
    boost::thread thread1(boost::bind(producer, 1000)); // start concurrent execution of producer
    boost::thread thread2(boost::bind(producer, 2000)); // start concurrent execution of producer
    boost::thread thread3(boost::bind(producer, 3000)); // start concurrent execution of consumer
    boost::thread thread4(boost::bind(producer, 4000)); // start concurrent execution of consumer
    boost::thread thread5(boost::bind(producer, 5000)); // start concurrent execution of consumer
    boost::thread thread6(boost::bind(producer, 6000)); // start concurrent execution of consumer
    boost::thread thread7(boost::bind(producer, 7000)); // start concurrent execution of consumer
    boost::thread thread8(boost::bind(producer, 8000)); // start concurrent execution of consumer
    boost::thread thread9(boost::bind(producer, 9000)); // start concurrent execution of consumer
//    thread::sleep(1);

    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    thread9.join();
}

void test_push() {
    sync_buffer<int, 10, boost::synchro::thread_synchronization_family> buf;

    BOOST_CHECK((buf.get_in()==0));
    BOOST_CHECK((buf.get_out()==0));
    BOOST_CHECK((buf.empty()));
    BOOST_CHECK((!buf.full()));
    buf.push(1);
       std::cout << "buf.push("<<1<<");"<<std::endl;
       std::cout << "in_ ="<< buf.get_in()<<std::endl;
       std::cout << "out_ ="<< buf.get_out()<<std::endl;
       std::cout << "(out_%size)+1 ="<< (buf.get_out()%2)+1<<std::endl;

    BOOST_CHECK((buf.get_in()==1));
    BOOST_CHECK((buf.get_out()==0));
    BOOST_CHECK((!buf.empty()));
    BOOST_CHECK((!buf.full()));

    buf.push(2);
       std::cout << "buf.push("<<2<<");"<<std::endl;
       std::cout << "in_ ="<< buf.get_in()<<std::endl;
       std::cout << "out_ ="<< buf.get_out()<<std::endl;
       std::cout << "(out_%size)+1 ="<< (buf.get_out()%2)+1<<std::endl;

    BOOST_CHECK((buf.get_in()==2));
    BOOST_CHECK((buf.get_out()==0));
    BOOST_CHECK((!buf.empty()));
    BOOST_CHECK((buf.full()));


    int l;
    l= buf.pull();
    std::cout << "buf.pull() -> "<<l<<""<<std::endl;
    BOOST_CHECK(l==1);
    BOOST_CHECK((!buf.empty()));
    BOOST_CHECK((!buf.full()));

    l= buf.pull();
    std::cout << "buf.pull() -> "<<l<<""<<std::endl;
    BOOST_CHECK(l==2);
    BOOST_CHECK((buf.empty()));
    BOOST_CHECK((!buf.full()));
}

//int main() {
//    test_1();
//}
//#define BOOST_TEST_MAIN syn
//BOOST_TEST_SUITE( syn )
//
//BOOST_AUTO_TEST_CASE( t1 )
//{
//    test_1();
//}
//
//BOOST_AUTO_TEST_SUITE_END()

test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("sync__queues__sync_buffer_family");
//  test->add(BOOST_TEST_CASE(&test_push));
  test->add(BOOST_TEST_CASE(&test_1));
  return test;
}

