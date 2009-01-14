//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////
//#define CONCENTRATOR
#include "./timestamped.hpp"
#include "./async_ostream.hpp"
#include <boost/interthreads/thread_decorator.hpp>
#include <boost/interthreads/thread_specific_shared_ptr.hpp>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include <deque>                                // std::priority_queue
#include <queue>                                // std::priority_queue
#include <iostream>                             // std::ostream
#include <sstream>                              // std::stringstream

boost::mutex out_global_mutex;

namespace boost {
namespace interthreads {
namespace detail {
    typedef timestamped<std::stringstream> element_type;
    typedef std::queue<element_type*> queue_type;
    typedef std::priority_queue<queue_type*, std::deque<element_type*>, element_type::ptr_comparator_gt> priority_queue_type;
    
    //==========================================================================================

    struct async_ostream_thread_ctx {
        async_ostream_thread_ctx() 
        : current_(new element_type()) 
        , seq_(0)
        {}

        boost::mutex& get_mutex() {return mutex_;}
        std::stringstream& buffer() {return current_->value_;}
        
        std::streamsize write(const char* s, std::streamsize n) {
            buffer().write(s, n);
            return n;
        }
        
        void flush() {
            current_->reset_date(seq_);
            ++seq_;
            ++nb_push;
            if (queue_.size()>100) {
                ++nb_push_gt_1;
                queue_.push(current_);
            } else {
                boost::lock_guard<boost::mutex> lock(mutex_);
                queue_.push(current_);
            }
            current_ = new element_type();
        }           
        element_type* get() {
            if (queue_.size()>100) {
                ++nb_get_gt_1;
                return get_i();
            } else {
                boost::lock_guard<boost::mutex> lock(mutex_);
                return get_i();
            }
        }
        void print_stats() {
            boost::thread::id id= boost::this_thread::get_id();
            boost::lock_guard<boost::mutex> lock(out_global_mutex);
            std::cout << "TID=" << id <<" nb_push " << nb_push  << std::endl;
            std::cout << "TID=" << id <<" nb_push_gt_1 " << nb_push  << std::endl;
            std::cout << "TID=" << id <<" nb_get " << nb_push  << std::endl;
            std::cout << "TID=" << id <<" nb_get_gt_1 " << nb_push  << std::endl;
            std::cout << "TID=" << id <<" inc " << inc_  << std::endl;
        };
        bool empty() {
            return queue_.empty();
        }
        void inc() {
            ++inc_;
        }

        
    private:
        element_type* get_i() {
            if (queue_.empty()) {
                ++nb_get_0;
                return 0;
            }
            ++nb_get;
            element_type* e= queue_.front();
            queue_.pop();
            return  e;
        }
        element_type *current_;
        queue_type queue_;   
        unsigned seq_;   
        boost::mutex mutex_;
        unsigned nb_push_gt_1;
        unsigned nb_push;
        unsigned nb_get_gt_1;
        unsigned nb_get_0;
        unsigned nb_get;
        unsigned inc_;
    };

    //==========================================================================================
#ifdef CONCENTRATOR            
    struct async_ostream_concentrator {
		static void loop(async_ostream_sink::impl* that);
		async_ostream_concentrator(async_ostream_sink::impl* impl_ptr) 
            : thread_(boost::bind(loop, impl_ptr)) {}
		~async_ostream_concentrator() {}

	private:
		boost::thread thread_;
	};
#endif            
	
    //==========================================================================================
    
    typedef thread_specific_shared_ptr<async_ostream_thread_ctx> tsss_type;
    
    struct async_ostream_sink::impl {
        impl(std::ostream& os) 
        : os_(os)
        , tsss_(terminate)
#ifndef CONCENTRATOR            
        , thread_(boost::bind(loop, this)) 
#endif            
        {}
        
        std::ostream& os_;
        tsss_type tsss_;
        priority_queue_type queue_;
#ifdef CONCENTRATOR            
        boost::once_flag concentrator_flag_;
        async_ostream_concentrator* concentrator_;
#else
		boost::thread thread_;            
#endif            
            
        static void terminate(shared_ptr<async_ostream_thread_ctx> that) {
            while (!that->empty()) {
                that->inc();
            }
            //that->print_stats();            
        }
        
#ifdef CONCENTRATOR            
        static void create_concentrator_once(impl* that) {
            that->concentrator_ = new async_ostream_concentrator(that);
        }
        void create_concentrator() {
            boost::call_once(concentrator_flag_, 
                             boost::bind(create_concentrator_once, this));
        }
#else        
		static void loop(impl* that);
#endif            
        
        std::streamsize write(const char* s, std::streamsize n) {
            return tsss_->write(s, n);
        }
        
        void flush() {
            tsss_->flush();
        }            
        
            
    };

    //==========================================================================================
    
    async_ostream_sink::async_ostream_sink(std::ostream& os)
        : impl_(new async_ostream_sink::impl(os)) {}

    std::streamsize async_ostream_sink::write(const char* s, std::streamsize n) {
        return impl_->write(s,n);
    }
        
    void async_ostream_sink::flush() {
        return impl_->flush();
    }
    
#ifdef CONCENTRATOR            
    void async_ostream_concentrator::loop(async_ostream_sink::impl* that) {
#else
    void async_ostream_sink::impl::loop(async_ostream_sink::impl* that) {
#endif        
        std::ostream& os_ = that->os_;
        for(;;) 
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
            { // scope needed don't remove
                tsss_type::lock_type lock(that->tsss_.get_mutex());
                const tsss_type::map_type& tmap(that->tsss_.get_map(lock));
                for (tsss_type::map_type::const_iterator it = tmap.begin(); it != tmap.end(); ++it) {
                    for(int i=0;i<1;++i) {
                        element_type* e= it->second->get();
                        if (e ==0) break;
                        that->queue_.push(e);
                    }
                }
                //it->second->print_stats();                
            }
            if (that->queue_.empty()) {
                boost::this_thread::sleep(boost::posix_time::milliseconds(10));
            } else {
                element_type* e = that->queue_.top();
                that->queue_.pop();
    #ifdef XTIME
                os_ << e->seq_ << "["<<e->date_.sec<<":"<<e->date_.nsec<<"]| " << e->value_.str();
    #else
                os_ << e->seq_ <<  "| " << e->value_.str();
    #endif
                delete e;
            }
            
        }
    }
    
}

    //==========================================================================================
   
    
    async_ostream::async_ostream(std::ostream& os)
        : base_type(os) 
        {}

    void async_ostream::flush() {
        this->base_type::flush();
        async_ostream& d = *this;
        d->flush();
    }
            
    //==========================================================================================
    
    async_ostream cout_(std::cout);
    
    void async_ostream::thread_specific_setup() {
        cout_->impl_->tsss_.reset(new detail::async_ostream_thread_ctx());
#ifdef CONCENTRATOR            
        cout_->impl_->create_concentrator();
#endif        
    }
    
    namespace detail 	{
	    thread_decoration async_ostream_decoration(boost::interthreads::async_ostream::thread_specific_setup);
    }
    
}
}


