//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threadalert library of Roland Schwarz
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interthreads/thread_keep_alive.hpp>
#include <boost/interthreads/thread_decorator.hpp>
#include <boost/interthreads/thread_specific_shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <cstdlib>

namespace boost {
namespace interthreads {
namespace detail {

    struct thread_keep_alive_ctx {
        static void default_on_dead_thread(thread::id, thread*) {
            //    std::cout << "default_on_dead_thread" << std::endl;
            ::abort();
        }
        void set_on_dead_thread_i(on_dead_thread_type fct, thread* th) {             
            if (data_==0)
                std::cout << "ERROR thread_keep_alive_ctx::set_on_dead_thread" << std::endl;
            else {
                on_dead_=fct;
                thread_ptr_=th;
            }
        }

        void set_on_dead_thread(on_dead_thread_type fct, thread* th) {             
            boost::lock_guard<boost::mutex> lock(mutex_);
            set_on_dead_thread_i(fct, th);
        }
        thread_keep_alive_ctx() 
            : data_(&data_b)
            , on_dead_(default_on_dead_thread)
            , thread_ptr_(0)
            {}
        ~thread_keep_alive_ctx() {}


        void enable_keep_alive(std::size_t periods, std::size_t checkins) {
            boost::lock_guard<boost::mutex> lock(mutex_);
            data_->total_periods_ = data_->periods_ = periods;
            data_->required_checkins_=checkins;
            data_->enabled_=true;
        }

        void enable_keep_alive() {
            boost::lock_guard<boost::mutex> lock(mutex_);
            data_->enabled_ = true;
        }

        void disable_keep_alive() {
            boost::lock_guard<boost::mutex> lock(mutex_);
            data_->enabled_ = false;
        }
        
        bool enabled_i() {
            return data_->enabled_;
        }
        
        bool enabled() {
            boost::lock_guard<boost::mutex> lock(mutex_);
            return enabled_i();
        }

        thread_keep_alive_internal* backup(thread_keep_alive_internal* new_data) {
            thread_keep_alive_internal* the_backup=data_;
            data_=new_data;
            return the_backup;
        }

        void restore(thread_keep_alive_internal* backup) {
            data_=backup;
        }

        void check_point() {
            ++(data_->checkins_);
        }

        void control(thread::id id) {
            if (data_->enabled_) {
                --(data_->periods_);
                if (data_->periods_==0) {
                    if (dead()) {
                        on_dead(id);
                    } 
                    data_->checkins_=0;
                    data_->periods_=data_->total_periods_;
                }
            }
        }

        typedef thread_specific_shared_ptr <thread_keep_alive_ctx> tssp;
        inline static tssp& instance() {return instance_;}
        boost::mutex& get_mutex() {return mutex_;}

    private:
        thread_keep_alive_internal data_b;
        thread_keep_alive_internal* data_;
        on_dead_thread_type on_dead_;
        thread* thread_ptr_;


        static tssp instance_;
        boost::mutex mutex_;
        bool dead() {
            return ((data_->checkins_ < data_->required_checkins_));
        }

        void on_dead(thread::id id) {
            (on_dead_)(id, thread_ptr_);
        }

        //      static void terminate();
        static void init();

        static thread_decoration initializer_;

    };

    struct keep_alive_mgr {
        static void loop(bool& end) {
            boost::xtime t;
            boost::xtime_get(&t,1); 
            while(!end) {
                t.sec += 1; 
                boost::thread::sleep(t);
                detail::thread_keep_alive_ctx::tssp::lock_type lock(thread_keep_alive_ctx::instance().get_mutex());
                const detail::thread_keep_alive_ctx::tssp::map_type& tmap(
                        thread_keep_alive_ctx::instance().get_map(lock));
                for (thread_keep_alive_ctx::tssp::map_type::const_iterator it = tmap.begin(); 
                    it != tmap.end(); ++it) {
                    it->second->control(it->first);
                }
            }
        }

        keep_alive_mgr() : end_(false), thread_(boost::bind(loop, boost::ref(end_)))  {}
        ~keep_alive_mgr() {
            interrupt_keep_alive();
        }

        static keep_alive_mgr* instance() { return instance_.get(); }
        static void initialize() {
            boost::call_once(flag, init);
        }

        void interrupt_keep_alive() {
            end_=true;
            thread_.join();
        }

    private:
        static boost::shared_ptr<keep_alive_mgr> instance_;
        static void init() {
            instance_.reset(new keep_alive_mgr());
        }

        bool end_;
        boost::thread thread_;
        static once_flag flag;
//        keep_alive_mgr(const keep_alive_mgr&);
    };

    // WARNING: static variable: thread_keep_alive_ctx::instance_ uses keep_alive_mgr::instance_ on its destructor so don't change the order of these declarations
    thread_specific_shared_ptr<detail::thread_keep_alive_ctx> thread_keep_alive_ctx::instance_;
    boost::shared_ptr<keep_alive_mgr> keep_alive_mgr::instance_;
    // WARNING: static variable
    thread_decoration thread_keep_alive_ctx::initializer_(thread_keep_alive_ctx::init);

    boost::once_flag keep_alive_mgr::flag;
//    void thread_keep_alive_ctx::terminate() {    } 

    void thread_keep_alive_ctx::init() {
        keep_alive_mgr::initialize();
//        boost::this_thread::at_thread_exit(terminate);
        instance_.reset(new thread_keep_alive_ctx());
    }



#ifdef BOOST_INTERTHREADS_KEEP_ALIVE_TEST
    struct in_nb_kills {
        unsigned     kills_;
        in_nb_kills();
        void operator()() {
            ++kills_;
        }
    };
#endif
    
    thread_keep_alive_internal::thread_keep_alive_internal() 
    : enabled_(false)
    , periods_(0)
    , checkins_(0)
    , total_periods_(0)
    , required_checkins_(0)
    {}
}

namespace this_thread { 
    enable_keep_alive::enable_keep_alive(
            std::size_t periods, std::size_t checkins) 
    {
        backup_=detail::thread_keep_alive_ctx::instance()->backup(&data_);
        detail::thread_keep_alive_ctx::instance()->enable_keep_alive(periods, checkins);
    }

    enable_keep_alive::~enable_keep_alive() {
        detail::thread_keep_alive_ctx::instance()->restore(backup_);
    }

    disable_keep_alive::disable_keep_alive() {
        backup_=detail::thread_keep_alive_ctx::instance()->backup(&data_);
        detail::thread_keep_alive_ctx::instance()->disable_keep_alive();
    }

    disable_keep_alive::~disable_keep_alive() {
        detail::thread_keep_alive_ctx::instance()->restore(backup_);
    }

void keep_alive_point() {
    detail::thread_keep_alive_ctx::instance()->check_point();
}

bool keep_alive_enabled() {
    return detail::thread_keep_alive_ctx::instance()->enabled();
}

void set_on_dead_thread(void (*fct)(thread::id, thread*), thread* th) {
    if (detail::thread_keep_alive_ctx::instance().get()==0) {
        std::cout << "detail::thread_keep_alive_ctx::instance().get()==0" << std::endl;
    } else {
        detail::thread_keep_alive_ctx::instance()->set_on_dead_thread(fct, th);
    }
}

}

bool keep_alive_enabled(thread::id id) {
    detail::thread_keep_alive_ctx::tssp::lock_type lock(detail::thread_keep_alive_ctx::instance().get_mutex());
    return detail::thread_keep_alive_ctx::instance()[id]->enabled();
}

void set_on_dead_thread(thread::id id, void (*fct)(thread::id, thread*), thread* th) { 
    detail::thread_keep_alive_ctx::tssp::lock_type lock(detail::thread_keep_alive_ctx::instance().get_mutex());
    detail::thread_keep_alive_ctx::instance()[id]->set_on_dead_thread(fct, th);
}

}
}


