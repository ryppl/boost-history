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

using namespace boost::synchro;
using namespace boost;



namespace {
}

//[Master_Slave_Slave
class Slave : boost::synchro::concurrent_component
{
    port receive_;
    unsigned sum_;
    bool End;
public:
	Slave() : End(false) {}
    void Clear() {
//        std::cout << "Clear()" << std::endl;
        port::synchronizer _(receive_);
        sum_ = 0;
    }
    void Add(unsigned v) {
//        std::cout << "Add(" << v << ")" << std::endl;
        port::synchronizer _(receive_);
        sum_+=v;
    }
    unsigned Result() {
        port::synchronizer _(receive_);
//        std::cout << sum_ << "=Result()" << std::endl;
        return sum_;
    }
    void Interrupt() {
//        std::cout << "Interrupt()" << std::endl;
        port::synchronizer _(receive_);
    	End=true;
    }
    void operator()() {
        while (!End) {
//            std::cout << "accept()" << std::endl;
            accept(receive_);
        }
    }
};
//]
//[Master_Slave_Master
struct  Master : boost::synchro::concurrent_component {
    Slave& Slave1_;
    Slave& Slave2_;

    int V[100];
    Master(Slave& slave1, Slave& slave2)
        : Slave1_(slave1)
        , Slave2_(slave2)
        {}
    void operator()()
    {
        for (int i=0; i<100; i++) {
            V[i]=i;
        }
//        std::cout << "before clear" << std::endl;
        Slave1_.Clear();
        Slave2_.Clear();
//        std::cout << "cleared" << std::endl;

        for (int i=0; i<100; i++) {
            if ((V[i]%2) > 0) Slave1_.Add(V[i]);
            else          Slave2_.Add(V[i]);
        }
        std::cout << Slave1_.Result() << std::endl;
        std::cout << Slave2_.Result() << std::endl;
        Slave1_.Interrupt();
        Slave2_.Interrupt();
    }
};
//]

int main()
{

    std::cout << "BEGIN" << std::endl;
    Slave slave1;
    Slave slave2;
    Master master(slave1, slave2);
#if 1
    conc_tuple<Slave, Slave, Master> conc(slave1, slave2, master);
    conc();
    conc.join();
#else
    boost::thread thread1(boost::ref(slave1));
    boost::thread thread2(boost::ref(slave2));
    boost::thread thread3(boost::ref(master));
    std::cout << "wait" << std::endl;

    thread1.join();
    thread2.join();
    thread3.join();
#endif
    return 0;
}

