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
#include "boost/synchro/conc/conc_tuple.hpp"

namespace {
    volatile int Run = 1;
}

using namespace boost::synchro;
using namespace boost;

struct Producer;
struct Consumer;

//[QualifiedSingleBuf
class SingleBuf : boost::synchro::concurrent_component {
    qualified_port<Producer> PutPort_;
    qualified_port<Consumer> GetPort_;
//    const concurrent_component_base* sender_;
    char bufCh_;
public:
    void Put(const Producer* snd, char ch) {
        qualified_port<Producer>::synchronizer _(PutPort_, snd);
        bufCh_ = ch;
    }
    char Get(const Consumer* snd) {
        qualified_port<Consumer>::synchronizer _(GetPort_, snd);
        return bufCh_;
    }
    void operator()() {
        while (Run) {
            accept(PutPort_);
            if (!Run) break;
            accept(GetPort_);
        }
    }
};
//]
struct Producer : boost::synchro::concurrent_component {
    SingleBuf& buf_;
    Producer(SingleBuf& buf) : buf_(buf) {}
    void operator()()
    {
        char ch=32;
        while (Run) {
            ch+=1;
            if (ch > 65) ch = 32;
            buf_.Put(this, ch);
        }
    }
};
struct  Consumer : boost::synchro::concurrent_component {
    SingleBuf& buf_;
    Consumer(SingleBuf& buf) : buf_(buf) {}
    void operator()() {
        while (Run) {
            char ch = buf_.Get(this);
            std::cout << ch << std::endl;
        }
    }
};

int main()
{
    std::cout << "press <enter> to end the program" << std::endl;

    SingleBuf buf;
    Producer prod(buf);
    Consumer cons(buf);
    conc_tuple<SingleBuf, Producer, Consumer> conc(buf, prod, cons);
    conc();

    std::cin.get();
    Run = 0;
    conc.join();

    return 0;
}


