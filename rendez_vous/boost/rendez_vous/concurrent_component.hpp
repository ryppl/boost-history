//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/rendez_vous for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_RENDEZ_VOUS_CONC_SYSTEM__HPP
#define BOOST_RENDEZ_VOUS_CONC_SYSTEM__HPP

#include <boost/synchro/semaphore.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <typeinfo>

namespace boost { namespace rendez_vous {

struct concurrent_component_base {};
struct bad_sender {};
struct bad_type {};


namespace detail {
template <typename T>
class concurrent_component_synchro
{
public:
    concurrent_component_synchro()
        : signaled_(false)
    {
    }

    T wait()
    {
        boost::mutex::scoped_lock lock(mutex_);
        cond_.wait(lock);
        return val_;
    }

    bool post(T val)
    {
        boost::mutex::scoped_lock lock(mutex_);
        if (!signaled_) { /*< first post assigns the current thread id >*/
            signaled_=true;
            val_ = val;
            cond_.notify_one();
            return true;
        } else { /*< the other post do nothing >*/
            return false;
        }
    }
    void clear()
    {
        boost::mutex::scoped_lock lock(mutex_);
        signaled_=false;
    }

private:
    mutex mutex_;
    condition_variable cond_;
    bool signaled_;
    T val_;
};
}

class concurrent_component : public concurrent_component_base {
protected:
//[port
    struct port {
        friend class concurrent_component;
        port() : accept_call_(0), until_end_(0) {
        }
        ~port() {
        }
        struct synchronizer {
            port& that_;
            synchronizer(port& that) : that_(that) {
#if 0
                for (;;) {
                    that.accept_call_.wait();
                    if (that_.common_.post(1, that.accept_call_)) break;
                    that.accept_call_.post();
                }
#else
                that.accept_call_.wait();
#endif
            }
            ~synchronizer() {
                that_.until_end_.post();
            }
        };
    private:
        void accept() {
            accept_call_.post();
            until_end_.wait();
        }
#if 0
        void accept_one(port& common) {
            common_ = &common;
        }
#endif
        void accept_begin() {
            accept_call_.post();
        }
        void accept_end() {
            until_end_.wait();
        }

#if 0
        bool accept(const boost::posix_time::ptime &abs_time) {
            accept_call_.post();
            this_thread::sleep(abs_time);
            if (accept_call_.try_wait()) return false;
            until_end_.wait();
            return true;
        }

        static void accept(port& p, const boost::posix_time::ptime &abs_time) {
            p.accept(abs_time);
        }
#endif
        synchro::semaphore accept_call_;
        synchro::semaphore until_end_;
    };
    static void accept(port& p) {
//            accept_call_.common_.post(1);
        p.accept();
    }
    static void accept(port& p1, port& p2) {
        p1.accept_begin();
        p2.accept_begin();
//            p1.accept_call_.common_.wait();
        p1.accept_end();
        p2.accept_end();
    }
#if 0

    static void accept_one(port& p1, port& p2) {
    }
#endif
//]
//[object_port
    struct object_port {
        friend class concurrent_component;
        object_port() : accept_call_(0), until_end_(0) {
        }
        ~object_port() {
        }
        struct synchronizer {
            object_port& that_;
            synchronizer(object_port& that, const concurrent_component_base* snd) : that_(that) {
                if (snd!= that_.sender_) {
//                    std::cout<<"bad_sender "<<snd << "!="<< that_.sender_<< std::endl;
                    throw bad_sender();
                }
                that.accept_call_.wait();
            }
            ~synchronizer() {
                that_.until_end_.post();
            }
        };
    private:
        void accept(const concurrent_component_base* snd) {
            sender_=snd;
            accept_call_.post();
            until_end_.wait();
        }
        synchro::semaphore accept_call_;
        synchro::semaphore until_end_;
        const concurrent_component_base* sender_;
    };
    static void accept(object_port& that, const concurrent_component_base* snd) {
        that.accept(snd);
    }
    static void accept_from(object_port& that, const concurrent_component_base* snd) {
        that.accept(snd);
    }
//]
//[qualified_port
    template <typename TYPE>
    struct qualified_port {
        friend class concurrent_component;
        qualified_port() : accept_call_(0), until_end_(0){
        }
        ~qualified_port() {
        }
        struct synchronizer {
            qualified_port& that_;
            synchronizer(qualified_port& that, const TYPE* snd)  : that_(that) {
                that.accept_call_.wait();
            }
            ~synchronizer() {
                that_.until_end_.post();
            }
        };
    private:
        void accept() {
//            type_=&typeid(*snd);
            accept_call_.post();
            until_end_.wait();
        }
//        static void accept(object_port& that, const concurrent_component_base* snd) {
//            that.accept(snd);
//        }
        synchro::semaphore accept_call_;
        synchro::semaphore until_end_;
//        const std::type_info* type_;
    };
    template <typename TYPE>
    static void accept(qualified_port<TYPE>& that) {
        that.accept();
    }
//]
};
#if EXAMPLES
//[synchronized_communication_between_components_schema
class R;
class S : concurrent_component<> {
public:
    S(R& r);
    void operator()()
    {
        // ...
        e2 = r_.m(e1);
    }
};

class R : concurrent_component<> {
public:
    void m() {
        // ...
    }
    void operator()()
    {
        // ...
    }
};
//]
//[synchronized_communication_between_components
class R;
class S : concurrent_component<> {
public:
    S(R& r);
    void operator()()
    {
        // ...
        e2 = r_.m(e1);
    }
};

class R : concurrent_component<> {
    port p;
public:
    void m() {
        port::synchronizer _(p);
        // ...
    }
    void operator()()
    {
        // ...
        port::accept(p);
    }
};
//]
#endif
}
#if 0
namespace async {
namespace partial_specialization_workaround {
template< typename AE, typename CC >
struct fork<AE,CC&> {
    static typename result_of::fork<AE,CC>::type 
    apply(AE& ae, CC& cc ) {
        return ae.fork(boost::bind(boost::ref(cc)));
    }
};
}
}
#endif
}
#endif
