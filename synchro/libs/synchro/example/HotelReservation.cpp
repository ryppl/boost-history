//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/synchro/conc/concurrent_component.hpp"
#include "boost/synchro/thread/mutex.hpp"
#include "boost/thread.hpp"
#include "boost/ref.hpp"
#include <string>
#include "boost/synchro/conc/conc_tuple.hpp"
#include "boost/test/unit_test.hpp"


#define  ABOOST_TEST_MESSAGE(MSG) std::cout << MSG << std::endl
#define  ABOOST_TEST_CHECKPOINT(MSG) std::cout << MSG << std::endl

using namespace boost::synchro;
using namespace boost;

//namespace {
//    volatile int Run = 1;
//}

//[ReservationHandler
class ReservationHandler : public boost::synchro::concurrent_component
{
protected:
    port start_;
    object_port request_;
    const concurrent_component_base* sender_;
    bool closed_;
    bool end_;
public:
    ReservationHandler() : sender_(0), closed_(true), end_(false) {}
    void Lock(const concurrent_component_base* snd) {
        ABOOST_TEST_CHECKPOINT("Lock");
        port::synchronizer _(start_);
        sender_ = snd;
        closed_ =false;
    }
    void Interrupt() {
        ABOOST_TEST_CHECKPOINT("Interrupt");
        port::synchronizer _(start_);
        end_ =true;
    }
//    void Reserve(const concurrent_component_base& snd) {
//        object_port::synchronizer _(request_, snd);
//        Reserv();
//    }
//    virtual void Reserve()=0;

    void Close(const concurrent_component_base* snd) {
        ABOOST_TEST_CHECKPOINT("Close");
        object_port::synchronizer _(request_, snd);
        closed_ =true;
        sender_ = 0;
        DoClose();
    }
    virtual void DoClose() {};
    void operator()() {
        while (!end_) {
            accept(start_);
            if (end_) break;
            while (!closed_) {
                accept(request_, sender_);
            }
        }
    }
//    ReservationHandler()
//    : closed_(false)
//    {}
    virtual ~ReservationHandler() {};
};
//]

//[HotelResHandler
class HotelResHandler : public ReservationHandler
{
    unsigned roomNo;
public:
    unsigned Reserve(const concurrent_component_base* snd
                    , std::string guestName
                    , unsigned noOfPersons) {
        ABOOST_TEST_CHECKPOINT("Reserve");
        object_port::synchronizer _(request_, snd);
        // ...
        ++roomNo;
        ABOOST_TEST_CHECKPOINT(roomNo<<":"<<guestName<<" #"<<noOfPersons);
        return roomNo;
    }
    virtual void DoClose(){};
    virtual ~HotelResHandler() {};
};
//]

//[HotelResHandler_P
class P : public boost::synchro::concurrent_component {
public:
    HotelResHandler& hrh_;
    P(HotelResHandler& hrh):hrh_(hrh) {}
    void operator()() {
        hrh_.Lock(this);
        unsigned rno1 = hrh_.Reserve(this, std::string("Peter Olsen"),4);
        ABOOST_TEST_CHECKPOINT(rno1);
        unsigned rno2 = hrh_.Reserve(this, std::string("Anne Nielsen"),2);
        ABOOST_TEST_CHECKPOINT(rno2);
        hrh_.Close(this);
    }
};
//]

//[HotelResHandler_Q
class Q : public boost::synchro::concurrent_component {
public:
    HotelResHandler& hrh_;
    Q(HotelResHandler& hrh):hrh_(hrh) {}
    void operator()() {
        hrh_.Lock(this);
        unsigned rno1 = hrh_.Reserve(this, std::string("Carmen"),5);
        ABOOST_TEST_CHECKPOINT(rno1);
        unsigned rno2 = hrh_.Reserve(this, std::string("Javier"),1);
        ABOOST_TEST_CHECKPOINT(rno2);
        hrh_.Close(this);
    }
};
//]

class R : public boost::synchro::concurrent_component {
public:
    HotelResHandler& hrh_;
    R(HotelResHandler& hrh):hrh_(hrh) {}
    void operator()() {
        try {
            unsigned rno1 = hrh_.Reserve(this, std::string("Carmen"),5);
        }catch (bad_sender& exp) {
            ABOOST_TEST_MESSAGE("You need to Lock first");
        }catch (...) {
            std::cout << "... You need to Lock first" << std::endl;
        }
    }
};
//]

//[HotelResHandler_main
int main() {
    HotelResHandler hrh;
    P p(hrh);
    Q q(hrh);
    R r(hrh);
#if 1
    conc_tuple<HotelResHandler, P, Q, R> conc(hrh, p, q, r);
    conc();

    boost::this_thread::sleep(boost::posix_time::seconds(4));

    hrh.Interrupt();
    conc.join();
#else
    boost::thread thread1(boost::ref(hrh));
    boost::thread thread2(boost::ref(p));
    boost::thread thread3(boost::ref(q));
    boost::thread thread4(boost::ref(r));

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    hrh.Interrupt();

    thread2.join();
    thread3.join();
    thread4.join();
    thread1.join();
#endif
    return 0;
}
//]

