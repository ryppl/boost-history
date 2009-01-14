#ifndef BOOST_INTERTHREADS_THREADER__HPP
#define BOOST_INTERTHREADS_THREADER__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the unique_threader/unique_joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/detail/move.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/interthreads/fork.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

template <typename ResultType>
class unique_joiner;
    
template <typename ResultType>
class unique_joiner {
public:
    typedef ResultType result_type;
private:
    struct unique_joiner_data {
        unique_future< result_type > fut_;
        thread th_;

        unique_joiner_data() {}
            
        template <typename Nullary> 
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
        unique_joiner_data(thread::native_handle_attr_type& attr, Nullary f) {
            packaged_task<result_type> tsk(f);
            fut_ = tsk.get_future();
            thread th(attr, boost::move(tsk));
            th_ = boost::move(th);
        }
#else
        unique_joiner_data(Nullary f) {
            packaged_task<result_type> tsk(f);
            fut_ = tsk.get_future();
#if 0            
            th_ = boost::move(tsk);
#else            
            thread th(boost::move(tsk));
            th_ = boost::move(th);
#endif            
        }
#endif        

    };
    shared_ptr<unique_joiner_data> data_;
    typedef unique_joiner_data this_impl_type;
    typedef unique_joiner this_type;

    unique_joiner(unique_joiner& other);
    this_type& operator=(unique_joiner& other);

public:
    friend class unique_threader;
    template <typename Nullary> 
    // requires result_of<Nullary>::type  is convertible to ResultType
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
    unique_joiner(thread::native_handle_attr_type& attr, Nullary f) 
    : data_(new this_impl_type(attr, f))
#else
    unique_joiner(Nullary f) 
    : data_(new this_impl_type(f))
#endif        
    {} 

public:
    // move support
#ifdef BOOST_HAS_RVALUE_REFS
    unique_joiner(this_type&& other) {
        data_.swap(other.data_);
    }
    unique_joiner& operator=(unique_joiner&& other) {
        data_=other.data_;
        other.data_.reset(new this_impl_type());
        return *this;
    }

    unique_joiner&& move() {
        return static_cast<unique_joiner&&>(*this);
    }
#else        
    unique_joiner(boost::detail::thread_move_t<unique_joiner> x) {
        data_=x->data_;
        x->data_.reset(new this_impl_type());
    }
            
    unique_joiner& operator=(boost::detail::thread_move_t<unique_joiner> x) {
        this_type tmp_(x);
        swap(tmp_);
        return *this;
    }

    operator boost::detail::thread_move_t<unique_joiner>() {
        return move();
    }            

    boost::detail::thread_move_t<unique_joiner> move() {
        return boost::detail::thread_move_t<unique_joiner>(*this);
    }            
            
#endif        
        
    void swap(this_type& x) {
        data_.swap(x.data_);
    }

    bool joinable() { 
        return data_->th_.joinable(); 
    }
    
    void join() { 
        data_->th_.join(); 
//        wait(); 
    }

    bool join_until(const system_time& abs_time) {
		return data_->th_.timed_join(abs_time);
//        return wait_until(abs_time); 
    }

    template<typename TimeDuration>
    inline bool join_for(TimeDuration const& rel_time)
    {
        return join_until(get_system_time()+rel_time);
    }

    result_type get() const { 
        //data_->th_.join(); 
        return const_cast<unique_future< result_type >*>(&(data_->fut_))->get(); 
    }
   
    result_type operator()() const { return get(); }
    result_type get()  { 
        //data_->th_.join(); 
        return data_->fut_.get(); 
    }
    result_type operator()() { return get(); }

    bool is_ready() const {
        return data_->fut_.is_ready(); 
    }
    bool has_exception() const {
        return data_->fut_.has_exception(); 
    }
    bool has_value() const {
        return data_->fut_.has_value(); 
    }

    void wait() {
        data_->fut_.wait(); 
    }
    bool wait_until(const system_time& abs_time) {
        return data_->fut_.timed_wait_until(abs_time); 
    }

    template<typename TimeDuration>
    inline bool wait_for(TimeDuration const& rel_time)
    {
        return wait_until(get_system_time()+rel_time);
    }

    void detach() {
        data_->th_.detach(); 
    }
            
    void interrupt() {
        data_->th_.interrupt(); 
    }

    bool interruption_requested() const {
        return data_->th_.interruption_requested(); 
    }
    
    thread::id get_id() const {
        return data_->th_.get_id(); 
    }
    
    unique_future<result_type>& get_future() {
        return data_->fut_; 
    }
};

class unique_threader {
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
private: 
    thread_specific_ptr<thread::native_handle_attr_type> attr_;
public: 
    thread::native_handle_attr_type& attr() {
        if(attr_.get() ==0) {
            attr_.reset(new thread::native_handle_attr_type());
        };
        return *attr_.get();
    }
#endif  

public: 
    template <typename T>
    struct handle {
        typedef unique_joiner<T> type;
    };
    template <typename F>
    unique_joiner<typename boost::result_of<F()>::type> 
    fork(F f) {
        typedef typename boost::result_of<F()>::type result_type;
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
        unique_joiner<result_type> unique_joiner_(attr(), f);
#else
        unique_joiner<result_type> unique_joiner_(f);
#endif  
        return boost::move(unique_joiner_);        
    }

};
   
template <>
struct get_future<unique_threader> {
    template <typename T>
    unique_future<T>& operator()(unique_joiner<T>& j) { return j.get_future(); }
};


template <typename ResultType>
class shared_joiner;
    
template <typename ResultType>
class shared_joiner {
public:
    typedef ResultType result_type;
private:
    struct shared_joiner_data {
        shared_future< result_type > fut_;
        thread th_;

        shared_joiner_data() {}
            
        template <typename Nullary> 
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
        shared_joiner_data(thread::native_handle_attr_type& attr, Nullary f) {
            packaged_task<result_type> tsk(f);
            fut_ = tsk.get_future();
            thread th(attr, boost::move(tsk));
            th_ = boost::move(th);
        }
#else
        shared_joiner_data(Nullary f) {
            packaged_task<result_type> tsk(f);
            fut_ = tsk.get_future();
#if 0            
            th_ = boost::move(tsk);
#else            
            thread th(boost::move(tsk));
            th_ = boost::move(th);
#endif            
        }
#endif        

    };
    shared_ptr<shared_joiner_data> data_;
    typedef shared_joiner_data this_impl_type;
    typedef shared_joiner this_type;

//protected:
public:
    friend class shared_threader;
    template <typename Nullary> 
    // requires result_of<Nullary>::type  is convertible to ResultType
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
    shared_joiner(thread::native_handle_attr_type& attr, Nullary f) 
    : data_(new this_impl_type(attr, f))
#else
    shared_joiner(Nullary f) 
    : data_(new this_impl_type(f))
#endif        
    {} 

public:
    shared_joiner(const shared_joiner& other) : data_(other.data_){
    }
    this_type& operator=(const shared_joiner& other) {
        data_=other.data_;
        other.data_.reset(new this_impl_type());
        return *this;
    }

    // move support
#ifdef BOOST_HAS_RVALUE_REFS
    shared_joiner(this_type&& other) {
        data_.swap(other.data_);
    }
    shared_joiner& operator=(shared_joiner&& other) {
        data_=other.data_;
        other.data_.reset(new this_impl_type());
        return *this;
    }

    shared_joiner&& move() {
        return static_cast<shared_joiner&&>(*this);
    }
#else        
    shared_joiner(boost::detail::thread_move_t<shared_joiner> x) {
        data_=x->data_;
        x->data_.reset(new this_impl_type());
    }
            
    shared_joiner& operator=(boost::detail::thread_move_t<shared_joiner> x) {
        this_type tmp_(x);
        swap(tmp_);
        return *this;
    }

    operator boost::detail::thread_move_t<shared_joiner>() {
        return move();
    }            

    boost::detail::thread_move_t<shared_joiner> move() {
        return boost::detail::thread_move_t<shared_joiner>(*this);
    }            
            
#endif        
        
    void swap(this_type& x) {
        data_.swap(x.data_);
    }

    bool joinable() { 
        return data_->th_.joinable(); 
    }
    
    void join() { 
        data_->th_.join(); 
//        wait(); 
    }

    bool join_until(const system_time& abs_time) {
		return data_->th_.timed_join(abs_time);
//        return wait_until(abs_time); 
    }

    template<typename TimeDuration>
    inline bool join_for(TimeDuration const& rel_time)
    {
        return join_until(get_system_time()+rel_time);
    }
    result_type get() { 
        //data_->th_.join(); 
        return data_->fut_.get(); 
    }
    result_type operator()() { return get(); }
    result_type get() const { 
        //data_->th_.join(); 
        return const_cast<shared_future< result_type >*>(&(data_->fut_))->get(); 
    }
   
    result_type operator()() const { return get(); }

    bool is_ready() const {
        return data_->fut_.is_ready(); 
    }
    bool has_exception() const {
        return data_->fut_.has_exception(); 
    }
    bool has_value() const {
        return data_->fut_.has_value(); 
    }

    void wait() {
        data_->fut_.wait(); 
    }
    bool wait_until(const system_time& abs_time) {
        return data_->fut_.timed_wait_until(abs_time); 
    }

    template<typename TimeDuration>
    inline bool wait_for(TimeDuration const& rel_time)
    {
        return wait_until(get_system_time()+rel_time);
    }

    void detach() {
        data_->th_.detach(); 
    }
            
    void interrupt() {
        data_->th_.interrupt(); 
    }

    bool interruption_requested() const {
        return data_->th_.interruption_requested(); 
    }
    
    thread::id get_id() const {
        return data_->th_.get_id(); 
    }
    
    shared_future<result_type>& get_future() {
        return data_->fut_; 
    }
};

class shared_threader {
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
private: 
    thread_specific_ptr<thread::native_handle_attr_type> attr_;
public: 
    thread::native_handle_attr_type& attr() {
        if(attr_.get() ==0) {
            attr_.reset(new thread::native_handle_attr_type());
        };
        return *attr_.get();
    }
#endif  

public: 
    template <typename T>
    struct handle {
        typedef shared_joiner<T> type;
    };
    template <typename F>
    shared_joiner<typename boost::result_of<F()>::type> 
    fork(F f) {
        typedef typename boost::result_of<F()>::type result_type;
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
        return shared_joiner<result_type>(attr(), f);
#else
        return shared_joiner<result_type>(f);
#endif  
    }

};
   
template <>
struct get_future<shared_threader> {
    template <typename T>
    shared_future<T>& operator()(shared_joiner<T>& j) { return j.get_future(); }
};

}
#ifdef BOOST_HAS_RVALUE_REFS
    template <typename T>
    inline interthreads::unique_joiner<T>&& move(interthreads::unique_joiner<T>&& t)
    {
        return t;
    }
#else
    template <typename T>
    inline boost::detail::thread_move_t<interthreads::unique_joiner<T> >
    move(boost::interthreads::unique_joiner<T>&  t)
    {
        return boost::detail::thread_move_t<interthreads::unique_joiner<T> >(t);
    }
#endif
#ifdef BOOST_HAS_RVALUE_REFS
    template <typename T>
    inline interthreads::shared_joiner<T>&& move(interthreads::shared_joiner<T>&& t)
    {
        return t;
    }
#else
    template <typename T>
    inline boost::detail::thread_move_t<interthreads::shared_joiner<T> > 
    move(boost::interthreads::shared_joiner<T>& t)
    {
        return boost::detail::thread_move_t<interthreads::shared_joiner<T> >(t);
    }
#endif
}


#include <boost/config/abi_suffix.hpp>

#endif

