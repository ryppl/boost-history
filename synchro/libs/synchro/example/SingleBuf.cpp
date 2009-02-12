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
#include <boost/synchro/conc/conc_tuple.hpp>

namespace {
    volatile int Run = 1;
}

using namespace boost::synchro;
using namespace boost;

//[SingleBuf
class SingleBuf : boost::synchro::concurrent_component {
    port PutPort_,GetPort_;
    char bufCh_;
    bool End;
public:
	SingleBuf() : End(false) {}
    void Put(char ch) {
        port::synchronizer _(PutPort_);
//        std::cout << "void Put("<<ch<<")" << std::endl;
        bufCh_ = ch;
    }
    char Get() {
        port::synchronizer _(GetPort_);
//        std::cout << bufCh_<<"=Get()" << std::endl;
        return bufCh_;
    }
    void Interrupt() {
        port::synchronizer _(PutPort_);
//        std::cout << "Interrupt()" << std::endl;
    	End=true;
    }
    void operator()() {
        while (!End) {
//            std::cout << "port::accept(PutPort_)"<<std::endl;
            accept(PutPort_);
            if (End) break;
//            std::cout << "port::accept(GetPort_)"<<std::endl;
            accept(GetPort_);
        }
    }
};
struct Producer : boost::synchro::concurrent_component{
    SingleBuf& buf_;
    Producer(SingleBuf& buf) : buf_(buf) {}
    void operator()()
    {
        buf_.Put('h');
        buf_.Put('e');
        buf_.Put('l');
        buf_.Put('l');
        buf_.Put('o');
        buf_.Put(' ');
        buf_.Put('w');
        buf_.Put('o');
        buf_.Put('r');
        buf_.Put('l');
        buf_.Put('d');
        buf_.Put('!');
        buf_.Put('\n');
    }
};
struct  Consumer : boost::synchro::concurrent_component{
    SingleBuf& buf_;
    Consumer(SingleBuf& buf) : buf_(buf) {}
    void operator()() {
        for (;;) {
//            std::cout << "wait " << std::endl;
            char ch = buf_.Get();
            std::cout << ch;
            if (ch=='\n') break;
        }
    }
};
//]

int main()
{
//    std::cout << "press <enter> to end the program" << std::endl;

    SingleBuf buf;
    Producer prod(buf);
    Consumer cons(buf);
    conc_tuple<SingleBuf, Producer, Consumer> conc(buf, prod, cons);
    conc();

    boost::this_thread::sleep(boost::posix_time::seconds(3));
    buf.Interrupt();

    conc.join();
    return 0;
}

