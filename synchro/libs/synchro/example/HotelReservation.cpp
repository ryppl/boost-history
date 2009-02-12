//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/synchro/conc/concurrent_component.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <string>

#if 0
#include <boost/synchro/conc/conc_tuple.hpp>
#endif

#include <boost/interthreads/algorithm.hpp>
#include <boost/interthreads/typeof/threader.hpp>

#include <boost/test/unit_test.hpp>
boost::mutex MTX;

#define  ABOOST_TEST_MESSAGE(MSG) { \
/*boost::lock_guard<boost::mutex> g(MTX);*/ \
std::cout << __LINE__ << MSG << std::endl; \
}

#define  ABOOST_TEST_CHECKPOINT(MSG) { \
    /*boost::lock_guard<boost::mutex> g(MTX); */\
    std::cout << __LINE__ << MSG << std::endl; \
}

using namespace boost::synchro;
using namespace boost;

//namespace {
//    volatile int Run = 1;
//}

//[ReservationHandler
class ReservationHandler : public boost::synchro::concurrent_component
{
protected:
    port lock_or_interrupt_;
    object_port request_;
    const concurrent_component_base* sender_;
    bool closed_;
    bool interrupted_;
public:
    typedef void result_type;
    ReservationHandler() : sender_(0), closed_(true), interrupted_(false) {}
    void Lock(const concurrent_component_base* snd) {
        ABOOST_TEST_CHECKPOINT("Lock")
        port::synchronizer _(lock_or_interrupt_);
        ABOOST_TEST_CHECKPOINT("Locked")
        sender_ = snd;
        closed_ =false;
    }
    void Interrupt() {
        ABOOST_TEST_CHECKPOINT("Interrupt")
        port::synchronizer _(lock_or_interrupt_);
        interrupted_ =true;
    }
//    void Reserve(const concurrent_component_base& snd) {
//        object_port::synchronizer _(request_, snd);
//        Reserv();
//    }
//    virtual void Reserve()=0;

    void Close(const concurrent_component_base* snd) {
        ABOOST_TEST_CHECKPOINT("Close")
        object_port::synchronizer _(request_, snd);
        closed_ =true;
        sender_ = 0;
        DoClose();
    }
    virtual void DoClose() {};
    void operator()() {
        while (!interrupted_) {
            accept(lock_or_interrupt_);
            if (interrupted_) break;
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
        ABOOST_TEST_CHECKPOINT("Reserve")
        object_port::synchronizer _(request_, snd);
        // ...
        ++roomNo;
        ABOOST_TEST_CHECKPOINT(roomNo<<":"<<guestName<<" #"<<noOfPersons)
        return roomNo;
    }
    virtual void DoClose(){};
    virtual ~HotelResHandler() {};
};
//]

//[HotelResHandler_P
class P : public boost::synchro::concurrent_component {
public:
    typedef void result_type;
    HotelResHandler& hrh_;
    P(HotelResHandler& hrh):hrh_(hrh) {}
    void operator()() {
        ABOOST_TEST_MESSAGE("P Lock")
        hrh_.Lock(this);
        ABOOST_TEST_MESSAGE("P Reserve")
        unsigned rno1 = hrh_.Reserve(this, std::string("Peter Olsen"),4);
        ABOOST_TEST_CHECKPOINT(rno1)
        ABOOST_TEST_MESSAGE("P Reserve")
        unsigned rno2 = hrh_.Reserve(this, std::string("Anne Nielsen"),2);
        ABOOST_TEST_CHECKPOINT(rno2)
        hrh_.Close(this);
        ABOOST_TEST_MESSAGE("P Close")
    }
};
//]

//[HotelResHandler_Q
class Q : public boost::synchro::concurrent_component {
public:
    typedef void result_type;
    HotelResHandler& hrh_;
    Q(HotelResHandler& hrh):hrh_(hrh) {}
    void operator()() {
        ABOOST_TEST_MESSAGE("Q Lock")
        hrh_.Lock(this);
        ABOOST_TEST_MESSAGE("Q Reserve")
        unsigned rno1 = hrh_.Reserve(this, std::string("Carmen"),5);
        ABOOST_TEST_CHECKPOINT(rno1)
        ABOOST_TEST_MESSAGE("Q Reserve")
        unsigned rno2 = hrh_.Reserve(this, std::string("Javier"),1);
        ABOOST_TEST_CHECKPOINT(rno2)
        hrh_.Close(this);
        ABOOST_TEST_MESSAGE("Q Close")
    }
};
//]

class R : public boost::synchro::concurrent_component {
public:
    typedef void result_type;
    HotelResHandler& hrh_;
    R(HotelResHandler& hrh):hrh_(hrh) {}
    void operator()() {
        try {
            ABOOST_TEST_MESSAGE("R Reserve")
            unsigned rno1 = hrh_.Reserve(this, std::string("Carmen"),5);
        }catch (bad_sender& exp) {
            ABOOST_TEST_MESSAGE("You need to Lock first")
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
    boost::interthreads::shared_threader ae;
    BOOST_AUTO(hrh_act, boost::interthreads::fork(ae, boost::bind(boost::ref(hrh))));
    BOOST_AUTO(conc, boost::interthreads::fork_all(ae, boost::bind(boost::ref(p)), boost::bind(boost::ref(q)), boost::bind(boost::ref(r))));
    boost::interthreads::join_all(conc);
    hrh.Interrupt();
    boost::interthreads::join(hrh_act);
#else
    conc_tuple<HotelResHandler, P, Q, R> conc(hrh, p, q, r);
    conc();

    boost::this_thread::sleep(boost::posix_time::seconds(4));

    hrh.Interrupt();
    conc.join();

#if 0    
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
    
#endif
    return 0;
}
//]

