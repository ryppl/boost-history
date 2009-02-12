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
#include <boost/synchro/monitor.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#if 0
#include <boost/synchro/conc/conc_tuple.hpp>
#else
#include <boost/interthreads/algorithm.hpp>
#include <boost/interthreads/typeof/threader.hpp>
#endif

namespace {
    volatile int Run = 1;
}

using namespace boost::synchro;
using namespace boost;


//[Histogram
class Histogram : boost::synchro::concurrent_component {

    class HistogramData : public exclusive_monitor<> {
        unsigned R[100];
    public:
        HistogramData() {
            for (int i = 0; i<100; i++) R[i] = 0;
        }
        void Add(unsigned i) {
            synchronizer _(*this->mutex());
            R[i]+=1;
        }
        void Sub(unsigned i) {
            synchronizer _(*this->mutex());
            R[i]=(R[i]>0)?R[i]-1:0;
        }
        unsigned Get(unsigned i) {
            synchronizer _(*this->mutex());
            return R[i];
        }
    };
    class Display : boost::synchro::concurrent_component {
        HistogramData& histogramData_;
    public:
        typedef void result_type;
        Display(HistogramData& histogramData) : histogramData_(histogramData) {}
        void operator()()
        {
            for (int i=10;i>0;--i) {
                boost::this_thread::sleep(boost::posix_time::seconds(1));
                for (unsigned i =0; i<100; ++i) {
                    std::cout << "==(" << i << ", "
                        << histogramData_.Get(i) << ")"
                    << std::endl;
                }
            }
        }
    };
    class Update : boost::synchro::concurrent_component {
        Histogram::port& request_;
    public:
        typedef void result_type;
        Update(Histogram::port& req) : request_(req) {}
        void operator()()
        {
            for (int i=10;i>0;--i) {
                accept(request_);
            }
        }
    };
    port request_;
    HistogramData histogramData_;
//    friend class Update;
    Display disp_;
    Update updater_;
public:
    typedef void result_type;
    Histogram()
    : disp_(histogramData_)
    , updater_(request_)
    {}
    void NewValue(int V) {
        port::synchronizer _(request_);
        if (V>0) histogramData_.Add(V);
        else histogramData_.Sub(-V);
    }
    void operator()() {
        #if 0
        conc_tuple<Display,Update> conc_(disp_, updater_);
        conc_();
        conc_.join();
        #else
        boost::interthreads::shared_threader ae;
        BOOST_AUTO(conc_, boost::interthreads::fork_all(ae, boost::bind(boost::ref(disp_)), boost::bind(boost::ref(updater_))));
        boost::interthreads::join_all(conc_);
        #endif
        
    }
};
//]
class  S : boost::synchro::concurrent_component {
    Histogram& histogram_;
public:
    typedef void result_type;
    S(Histogram& histogram) : histogram_(histogram) {}
    void operator()()
    {
        for (int i=10;i>0;--i) {
            // TODO replace by random
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            histogram_.NewValue(i);

        }
    }
};

int main()
{
    Histogram histo;
    S snd(histo);
    #if 0
    conc_tuple<Histogram, S> conc(histo, snd);
    conc();
    boost::this_thread::sleep(boost::posix_time::seconds(10));

    conc.join();
    #else
    boost::interthreads::shared_threader ae;
    BOOST_AUTO(conc, boost::interthreads::fork_all(ae, boost::bind(boost::ref(histo)), boost::bind(boost::ref(snd))));
    boost::this_thread::sleep(boost::posix_time::seconds(10));
    boost::interthreads::join_all(conc);
    #endif

    return 0;
}


