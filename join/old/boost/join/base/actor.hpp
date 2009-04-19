//
// boost/join/actor.hpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_ACTOR_HPP
#define BOOST_JOIN_ACTOR_HPP

#include <vector>
#include <bitset>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/join/base/join_base.hpp>
#include <boost/join/base/exceptions.hpp>

namespace boost {
  namespace join {

    template <bool flag, typename T, typename U>
    struct select { typedef T result; };
    template <typename T, typename U>
    struct select<false, T, U> { typedef U result; };

    template <size_t size>
    struct bitmap {
      enum {
        max_size = size
      };
      typedef typename select<(size<=32), unsigned int, std::bitset<size> >::result bitset_type;
      bitset_type bitmap_;
      bitmap(unsigned int val=0) : bitmap_(val) {}
      void set(bitmap &t) { bitmap_ |= t.bitmap_; }
      void clear(bitmap &t) { bitmap_ &= ~t.bitmap_; }
      bool test(bitmap &t) { return (bitmap_ & t.bitmap_) != 0; }
      bool match(bitmap &b) { return (~bitmap_ & b.bitmap_) == 0; }
      bool operator==(bitmap &t) { return bitmap_ == t.bitmap_; }
      size_t num_of_ones(void) {
        size_t num = 0;
        for(int i = 0; i<max_size; i++)
          if (bitmap_ & (1<<i)) num++;
        return num;
      }
    };

    template <typename PortT>
    typename PortT::var_type
    top(PortT &p) {
      return p.top();
    }

    template <typename PortT, size_t N>
    boost::array<typename PortT::var_type,N>
    top(boost::array<PortT,N> &vp) {
      boost::array<typename PortT::var_type,N> vv;
      for(size_t i=0; i<N; i++)
        vv[i] = vp[i].top();
      return vv;
    }

    template <typename PortT>
    void pop_top(PortT &p) {
      p.pop_top();
    }

    template <typename PortT, std::size_t N>
    void pop_top(boost::array<PortT,N> &vp) {
      for(size_t i=0; i<N; i++)
        vp[i].pop_top();
    }

    template <typename PortT>
    struct var_type {
      typedef typename PortT::var_type result;
    };

    template <typename PortT, std::size_t N>
    struct var_type<boost::array<PortT,N> > {
      typedef boost::array<typename PortT::var_type,N> result;
    };

    template <typename PortT>
    struct res_type {
      typedef typename PortT::result_type result;
    };

    template <typename PortT, std::size_t N>
    struct res_type<boost::array<PortT,N> > {
      typedef void result;
    };

    //------- chord definitions ------

    template <size_t max_size>
    class chord_common : public chord_base {
    public:
      actor_base *actor_;
      bitmap<max_size> mask_;// ports of this chord
      size_t num_ports_; //for scheduling policy fire_as_much_as_possible
      port *synch_p_;
      int priority_; //sched priority of chord: 0 - max, the bigger the lower

      chord_common(bitmap<max_size> &m, port *s, actor_base* a, int pri) :
        actor_(a), mask_(m), synch_p_(s), priority_(pri) {
        num_ports_ = m.num_of_ones();
      }

      chord_base * fire(port *caller_port) {
        actor_->log.msg("a chord fired");
        if(caller_port->type_ == port::synch) {//caller is sync
          //invoke callback later in the same caller thread
          return this;
        }
        else { //caller is async_p
          //based on ports_, find any synch_port in this chord
          //and transfer control to its thread and let its  thread to run callback
          if (synch_p_) {
            (static_cast<synch_port*>(synch_p_))->transfer(this);
            return 0;
          }
          else { //no sync ports in chord
            if(actor_->has_spawn()) {
              return this;
            } else
              throw no_executor_exception();
          }
        }
        return 0;
      }
    };

    template <size_t max_size, typename PortT, typename CallT>
    class chord1 : public chord_common<max_size>, public chord_oper<typename res_type<PortT>::result> {
      typedef typename res_type<PortT>::result result_type;
    public:
      PortT &port_;
      CallT call_;
      chord1(bitmap<max_size> &m, port *sp, actor_base* a, PortT &p, CallT c, int pri) :
        chord_common<max_size>(m,sp,a,pri), port_(p), call_(c) {
      }
      void capture_arguments(boost::function0<result_type> &cb) {
        cb = boost::bind(call_, top(port_));
        pop_top(port_);
      }
    };

    template <size_t max_size, typename PortT1, typename PortT2, typename CallT>
    class chord2 : public chord_common<max_size>, public chord_oper<typename res_type<PortT1>::result> {
      typedef typename res_type<PortT1>::result result_type;
    public:
      PortT1 &port1_;
      PortT2 &port2_;
      CallT call_;
      chord2(bitmap<max_size> &m, port *sp, actor_base* a, PortT1 &p1, PortT2 &p2, CallT c, int pri) :
        chord_common<max_size>(m,sp,a, pri), port1_(p1), port2_(p2), call_(c) {
      }
      void capture_arguments(boost::function0<result_type> &cb) {
        cb = boost::bind(call_, top(port1_), top(port2_));
        pop_top(port1_);
        pop_top(port2_);
      }
    };

    template <size_t max_size, typename PortT1, typename PortT2, typename PortT3, typename CallT>
    class chord3 : public chord_common<max_size>, public chord_oper<typename res_type<PortT1>::result> {
      typedef typename res_type<PortT1>::result result_type;
    public:
      PortT1 &port1_;
      PortT2 &port2_;
      PortT3 &port3_;
      CallT call_;
      chord3(bitmap<max_size> &m, port *sp, actor_base* a, PortT1 &p1, PortT2 &p2, PortT3 &p3, CallT c, int pri) :
        chord_common<max_size>(m,sp,a,pri), port1_(p1), port2_(p2), port3_(p3), call_(c) {
      }
      void capture_arguments(boost::function0<result_type> &cb) {
        cb = boost::bind(call_, top(port1_), top(port2_), top(port3_));
        pop_top(port1_);
        pop_top(port2_);
        pop_top(port3_);
      }
    };

    template <size_t max_size, typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename CallT>
    class chord4 : public chord_common<max_size>, public chord_oper<typename res_type<PortT1>::result> {
      typedef typename res_type<PortT1>::result result_type;
    public:
      PortT1 &port1_;
      PortT2 &port2_;
      PortT3 &port3_;
      PortT4 &port4_;
      CallT call_;
      chord4(bitmap<max_size> &m, port *sp, actor_base* a, PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, CallT c, int pri) :
        chord_common<max_size>(m,sp,a,pri), port1_(p1), port2_(p2), port3_(p3), port4_(p4), call_(c) {
      }
      void capture_arguments(boost::function0<result_type> &cb) {
        cb = boost::bind(call_, top(port1_), top(port2_), top(port3_), top(port4_));
        pop_top(port1_);
        pop_top(port2_);
        pop_top(port3_);
        pop_top(port4_);
      }
    };

    template <size_t max_size, typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename CallT>
    class chord5 : public chord_common<max_size>, public chord_oper<typename res_type<PortT1>::result> {
      typedef typename res_type<PortT1>::result result_type;
    public:
      PortT1 &port1_;
      PortT2 &port2_;
      PortT3 &port3_;
      PortT4 &port4_;
      PortT5 &port5_;
      CallT call_;
      chord5(bitmap<max_size> &m, port *sp, actor_base* a, PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, CallT c, int pri) :
        chord_common<max_size>(m,sp,a,pri), port1_(p1), port2_(p2), port3_(p3), port4_(p4), port5_(p5), call_(c) {
      }
      void capture_arguments(boost::function0<result_type> &cb) {
        cb = boost::bind(call_, top(port1_), top(port2_), top(port3_), top(port4_), top(port5_));
        pop_top(port1_);
        pop_top(port2_);
        pop_top(port3_);
        pop_top(port4_);
        pop_top(port5_);
      }
    };

    template <size_t max_size, typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename CallT>
    class chord6 : public chord_common<max_size>, public chord_oper<typename res_type<PortT1>::result> {
      typedef typename res_type<PortT1>::result result_type;
    public:
      PortT1 &port1_;
      PortT2 &port2_;
      PortT3 &port3_;
      PortT4 &port4_;
      PortT5 &port5_;
      PortT6 &port6_;
      CallT call_;
      chord6(bitmap<max_size> &m, port *sp, actor_base* a, PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, CallT c, int pri) :
        chord_common<max_size>(m,sp,a,pri), port1_(p1), port2_(p2), port3_(p3), port4_(p4), port5_(p5), port6_(p6), call_(c) {
      }
      void capture_arguments(boost::function0<result_type> &cb) {
        cb = boost::bind(call_, top(port1_), top(port2_), top(port3_), top(port4_), top(port5_), top(port6_));
        pop_top(port1_);
        pop_top(port2_);
        pop_top(port3_);
        pop_top(port4_);
        pop_top(port5_);
        pop_top(port6_);
      }
    };

    template <size_t max_size, typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename CallT>
    class chord7 : public chord_common<max_size>, public chord_oper<typename res_type<PortT1>::result> {
      typedef typename res_type<PortT1>::result result_type;
    public:
      PortT1 &port1_;
      PortT2 &port2_;
      PortT3 &port3_;
      PortT4 &port4_;
      PortT5 &port5_;
      PortT6 &port6_;
      PortT7 &port7_;
      CallT call_;
      chord7(bitmap<max_size> &m, port *sp, actor_base* a, PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, CallT c, int pri) :
        chord_common<max_size>(m,sp,a,pri), port1_(p1), port2_(p2), port3_(p3), port4_(p4), port5_(p5), port6_(p6), port7_(p7), call_(c) {
      }
      void capture_arguments(boost::function0<result_type> &cb) {
        cb = boost::bind(call_, top(port1_), top(port2_), top(port3_), top(port4_), top(port5_), top(port6_), top(port7_));
        pop_top(port1_);
        pop_top(port2_);
        pop_top(port3_);
        pop_top(port4_);
        pop_top(port5_);
        pop_top(port6_);
        pop_top(port7_);
      }
    };

    template <size_t max_size, typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename PortT8, typename CallT>
    class chord8 : public chord_common<max_size>, public chord_oper<typename res_type<PortT1>::result> {
      typedef typename res_type<PortT1>::result result_type;
    public:
      PortT1 &port1_;
      PortT2 &port2_;
      PortT3 &port3_;
      PortT4 &port4_;
      PortT5 &port5_;
      PortT6 &port6_;
      PortT7 &port7_;
      PortT8 &port8_;
      CallT call_;
      chord8(bitmap<max_size> &m, port *sp, actor_base* a, PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, PortT8 &p8, CallT c, int pri) :
        chord_common<max_size>(m,sp,a,pri), port1_(p1), port2_(p2), port3_(p3), port4_(p4), port5_(p5), port6_(p6), port7_(p7), call_(c) {
      }
      void capture_arguments(boost::function0<result_type> &cb) {
        cb = boost::bind(call_, top(port1_), top(port2_), top(port3_), top(port4_), top(port5_), top(port6_), top(port7_), top(port8_));
        pop_top(port1_);
        pop_top(port2_);
        pop_top(port3_);
        pop_top(port4_);
        pop_top(port5_);
        pop_top(port6_);
        pop_top(port7_);
        pop_top(port8_);
      }
    };

    //------ schedulers ------

    enum schedule_policy {
      schedule_first_match, //(first match will fire)
      schedule_longest_match,  //(longest match will fire)
      schedule_round_robin  //(round robin)
    };

    template <size_t max_size>
    struct sched_data_base {
      port *port_;
      bitmap<max_size> mask_;
      sched_data_base() :
        port_(0), mask_(0) {
      }
    };

    //basic schedulers
    template <size_t max_size>
    struct sched_data : public sched_data_base<max_size> {
      typedef chord_common<max_size> chord_type;
      std::vector<chord_type*> chords_; //chords this port participates in
      sched_data() : sched_data_base<max_size>() {}
      void port_add_chord(chord_type *c, int) {
        chords_.push_back(c);
      }
      void port_del_chord(chord_type *c, int) {
        typename std::vector<chord_type*>::iterator iter;
        if ((iter = std::find(chords_.begin(), chords_.end(), c)) != chords_.end())
          chords_.erase(iter);
        bool bound = false;
        if(chords_.size() > 0) bound = true;
        if (!bound) this->port_->reset();
      }
    };

    template <size_t max_size>
    struct sched_first_match : public sched_data<max_size> {
      typedef bitmap<max_size> bitmap_t;
      typedef chord_common<max_size> chord_type;
      enum { policy = schedule_first_match };
      chord_type * scan_chords(bitmap_t status_) {
        chord_type *chord_ready= 0;
        for (size_t i=0; i<this->chords_.size() && chord_ready == 0; i++) {
          if (status_.match(this->chords_[i]->mask_))
            chord_ready = this->chords_[i];
        }
        return chord_ready;
      }
    };

    template <size_t max_size>
    struct sched_longest_match : public sched_data<max_size> {
      enum { policy = schedule_longest_match };
      typedef bitmap<max_size> bitmap_t;
      typedef chord_common<max_size> chord_type;
      chord_type * scan_chords(bitmap_t status_) {
        chord_type *chord_ready= 0;
        size_t chord_size = 0;
        for(size_t i=0; i<this->chords_.size(); i++) {
          if(this->chords_[i]->num_ports_ > chord_size && status_.match(this->chords_[i]->mask_)) {
            chord_ready = this->chords_[i];
            chord_size = chord_ready->num_ports_;
          }
        }
        return chord_ready;
      }
    };

    template <size_t max_size>
    struct sched_round_robin : public sched_data<max_size> {
      enum { policy = schedule_round_robin };
      typedef bitmap<max_size> bitmap_t;
      typedef chord_common<max_size> chord_type;
      int last_chord_fired_;
      sched_round_robin() : sched_data<max_size>(), last_chord_fired_(-1) {}
      chord_type * scan_chords(bitmap_t status_) {
        chord_type *chord_ready= 0;
        size_t min = last_chord_fired_+1;
        for(size_t j=min; j<this->chords_.size() && chord_ready == 0; j++) {
          if(status_.match(this->chords_[j]->mask_)) {
            chord_ready = this->chords_[j];
            last_chord_fired_ = (int)j;
          }
        }
        if (chord_ready == 0 && min > 0)
          for(size_t j=0; j<min && chord_ready == 0; j++) {
            if(status_.match(this->chords_[j]->mask_)) {
              chord_ready = this->chords_[j];
              last_chord_fired_ = (int)j;
            }
          }
        return chord_ready;
      }
    };

    //simple priority based schedulers
    template <size_t max_size>
    struct sched_pri_data : public sched_data_base<max_size> {
      typedef chord_common<max_size> chord_type;
      std::vector<std::vector<chord_type*> > chords_; //chords this port participates in
      sched_pri_data() : sched_data_base<max_size>() {
        chords_.push_back(std::vector<chord_type*>());
      }
      void port_add_chord(chord_type *c, int priority) {
        if(chords_.size() < ((size_t)priority+1))
          for(size_t i=chords_.size(); i<((size_t)priority+1); i++) {
            chords_.push_back(std::vector<chord_type*>());
          }
        chords_[priority].push_back(c);
      }
      void port_del_chord(chord_type *c, int priority) {
        typename std::vector<chord_type*>::iterator iter;
        if ((iter = std::find(chords_[priority].begin(), chords_[priority].end(), c)) != chords_[priority].end())
          chords_[priority].erase(iter);
        bool bound = false;
        for(size_t i=0; i<chords_.size() && !bound; i++)
          if(chords_[i].size() > 0) bound = true;
        if (!bound) this->port_->reset();
      }
    };

    template <size_t max_size>
    struct sched_pri_first_match : public sched_pri_data<max_size> {
      enum { policy = schedule_first_match };
      typedef bitmap<max_size> bitmap_t;
      typedef chord_common<max_size> chord_type;
      chord_type * scan_chords(bitmap_t status_) {
        chord_type *chord_ready= 0;
        for(size_t i=0; i<this->chords_.size() && chord_ready == 0; i++)
          for(size_t j=0; j<this->chords_[i].size() && chord_ready == 0; j++) {
            if(status_.match(this->chords_[i][j]->mask_))
              chord_ready = this->chords_[i][j];
          }
        return chord_ready;
      }
    };

    template <size_t max_size>
    struct sched_pri_longest_match : public sched_pri_data<max_size> {
      enum { policy = schedule_longest_match };
      typedef bitmap<max_size> bitmap_t;
      typedef chord_common<max_size> chord_type;
      chord_type * scan_chords(bitmap_t status_) {
        chord_type *chord_ready= 0;
        for(size_t i=0; i<this->chords_.size() && chord_ready == 0; i++) {
          size_t chord_size = 0;
          for(size_t j=0; j<this->chords_[i].size(); j++) {
            if(this->chords_[i][j]->num_ports_ > chord_size) {
              if(status_.match(this->chords_[i][j]->mask_)) {
                chord_ready = this->chords_[i][j];
                chord_size = chord_ready->num_ports_;
              }
            }
          }
        }
        return chord_ready;
      }
    };

    template <size_t max_size>
    struct sched_pri_round_robin : public sched_pri_data<max_size> {
      enum { policy = schedule_round_robin };
      typedef bitmap<max_size> bitmap_t;
      typedef chord_common<max_size> chord_type;
      std::vector<int> last_chord_fired_; //for roundrobin dispatching
      sched_pri_round_robin() : sched_pri_data<max_size>() {
        last_chord_fired_.push_back(-1);
      }
      void port_add_chord(chord_type *c, int priority) {
        if(this->chords_.size() < ((size_t)priority+1))
          for(size_t i=this->chords_.size(); i<((size_t)priority+1); i++) {
            this->chords_.push_back(std::vector<chord_type*>());
            last_chord_fired_.push_back(-1);
          }
        this->chords_[priority].push_back(c);
      }
      void port_del_chord(chord_type *c, int priority) {
        typename std::vector<chord_type*>::iterator iter;
        if ((iter = std::find(this->chords_[priority].begin(), this->chords_[priority].end(), c)) != this->chords_[priority].end()) {
          this->chords_[priority].erase(iter);
          last_chord_fired_[priority] = -1;
        }
        bool bound = false;
        for(size_t i=0; i<this->chords_.size() && !bound; i++)
          if(this->chords_[i].size() > 0) bound = true;
        if (!bound) this->port_->reset();
      }
      chord_type * scan_chords(bitmap_t status_) {
        chord_type *chord_ready= 0;
        for(size_t i=0; i<this->chords_.size() && chord_ready == 0; i++)
          if(this->chords_[i].size()>0) {
            size_t min = last_chord_fired_[i]+1;
            for(size_t j=min; j<this->chords_[i].size() && chord_ready == 0; j++) {
              if(status_.match(this->chords_[i][j]->mask_)) {
                chord_ready = this->chords_[i][j];
                last_chord_fired_[i] = (int)j;
              }
            }
            if (chord_ready == 0 && min > 0)
              for(size_t j=0; j<min && chord_ready == 0; j++) {
                if(status_.match(this->chords_[i][j]->mask_)) {
                  chord_ready = this->chords_[i][j];
                  last_chord_fired_[i] = (int)j;
                }
              }
          }
        return chord_ready;
      }
    };

    //------ actor definition ------

    template <
      typename exec_type=async_p<typename actor_base::callable>,
      template <size_t> class scheduler=sched_first_match,
      size_t max_size=32
      >
    class actor_t : public actor_base, private boost::noncopyable {
      template <size_t> friend class chord_common;
      friend class port;

    public:
      typedef bitmap<max_size> bitmap_t;
      typedef scheduler<max_size> sched_type;
      typedef typename actor_base::callable callable;
      typedef async_p<typename actor_base::callable> default_executor;
      typedef exec_type executor; //in fact this is a task que to a real executor
      typedef chord_common<max_size> chord_type;
      typedef sched_type port_data;

    private:

      executor *spawn_;
      bitmap_t status_; //bitmap marking which port is active
      int heartbeat_;
      std::vector<port_data> ports_; //all unique ports in this actor, actor not owning
      //them and will not destroy them, actor will destroy port_data
      std::vector<boost::shared_ptr<chord_type> > chords_; //actor owns chords_ and will destroy them

    public:
      actor_t(executor *s = 0,
              int hb = 0, //forever
              const char *name = 0) :
        actor_base(name), spawn_(s), status_(0), heartbeat_(hb) {}

      ~actor_t() {
        reset();
      }

      void reset() {
        boost::mutex::scoped_lock lock(mutex_);
        internal_reset();
      }

      bool has_spawn(void) { return spawn_ != 0; }
      void spawn(callable c) {
        (*spawn_)(c);
      }

      boost::function1<void, callable> get_spawn() {
        return boost::function1<void, callable>(boost::ref(*spawn_));
      }

      schedule_policy my_scheduler(void) { return static_cast<schedule_policy>(sched_type::policy); }
      executor *my_executor(void) { return spawn_; }

      bool check_heartbeat(void) {
        if (heartbeat_ > 0) {
          heartbeat_--;
          if (heartbeat_ == 0) {
            internal_reset();
            return true;
          }
        }
        return false;
      }

    private:
      void internal_reset() {
        //reset all related ports
        for(size_t i=0; i<ports_.size(); i++)
          ports_[i].port_->reset();
        ports_.clear();
        chords_.clear();
        status_ = bitmap_t(0);
      }

      //ports call this to notify that a new msg comes
      //return: true - chord fired, false - chord not fired
      chord_base * port_invoked(int ind) {
        port_data &pd(ports_[ind]);
        pd.port_->num_pending_++;
        if(status_.test(pd.mask_)) {
          //already set
          //log.msg("port_invoked: a port add more pending calls");
          log.stream() << "port_invoked: a port [" << ind << "] add more pending calls" << logger::endl;
          //do nothing
        } else {
          status_.set(pd.mask_);
          //log.msg("port_invoked: a empty port get a call");
          log.stream() << "port_invoked: a empty port [" << ind << "] get a call" << logger::endl;
          chord_type *c = pd.scan_chords(status_); //find chord to fire based on dispatch_policy
          if(c != 0) {
            //update msg arrival status first, mark msgs to be consumed as
            //"unavailable", kinds of "reserve" these msgs
            //so that thread-scheduling will not interfere: another thread may come in
            //between try to take some of these msgs
            update_status(c->mask_);
            return c->fire(pd.port_);
          }
        }
        return 0;
      }

      //one msg will be taken from each port in mask
      //query these ports to see if any msg remaininf and
      //if its bit in status_ should be flipped
      void update_status(bitmap_t &chord_mask) {
        for(size_t i=0; i<ports_.size(); i++)
          if(chord_mask.test(ports_[i].mask_)) {
            ports_[i].port_->num_pending_--;
            if(ports_[i].port_->num_pending_ == 0)
              status_.clear(ports_[i].mask_);
          }
      }

      void port_revoked(int ind) {
        port_data &pd(ports_[ind]);
        pd.port_->num_pending_--;
        if(pd.port_->num_pending_ == 0)
          status_.clear(pd.mask_);
      }

      //
      // utils methods for creating chords
      //
      template <typename PortT>
      void add_port(PortT &p, bitmap_t &bmap) {
        port *pp = &p;
        port_data pd;
        int ind = -1;
        if(pp->actor_ != 0 && pp->actor_ != this)
          throw double_association_exception();
        for(size_t i=0; i<ports_.size() && ind == -1;i++)
          if(pp == ports_[i].port_) {
            ind = (int)i;
            pd = ports_[i];
          }
        if (ind == -1) {
          ind = (int)ports_.size();
          if((size_t)ind >= max_size) {
            log.msg("too_many_ports_exception thrown");
            throw too_many_ports_exception();
          }
          pd.port_ = pp;
          pd.mask_ = bitmap_t(1<<ind);
          ports_.push_back(pd);
          pp->actor_ = this;
          pp->index_ = ind;
          pp->num_pending_ = 0;
        }
        bmap.set(pd.mask_);
      }
      template <typename PortT, std::size_t N>
      void add_port(boost::array<PortT,N> &vp, bitmap_t &bmap) {
        for(size_t i=0; i<N; i++)
          add_port(vp[i], bmap);
      }
      template <typename PortT>
      bool find_port(PortT &p, bitmap_t &bmap) {
        if(p.actor_ == 0 || p.actor_ != this)
          return false;
        bmap.set(ports_[p.index_].mask_);
        return true;
      }
      template <typename PortT, std::size_t N>
      bool find_port(boost::array<PortT,N> &vp, bitmap_t &bmap) {
        for(size_t i=0; i<N; i++)
          if(!find_port(vp[i], bmap))
            return false;
        return true;
      }
      template <typename PortT>
      void port_add_chord(PortT &p, chord_type *c, int priority) {
        port_data &pd(ports_[p.index_]);
        pd.port_add_chord(c, priority);
      }
      template <typename PortT, std::size_t N>
      void port_add_chord(boost::array<PortT,N> &vp, chord_type *c, int priority) {
        for(size_t i=0; i<N; i++)
          port_add_chord(vp[i], c, priority);
      }
      template <typename PortT>
      void port_del_chord(PortT &p, chord_type *c, int priority) {
        port_data &pd(ports_[p.index_]);
        pd.port_del_chord(c, priority);
      }
      template <typename PortT, std::size_t N>
      void port_del_chord(boost::array<PortT,N> &vp, chord_type *c, int priority) {
        for(size_t i=0; i<N; i++)
          port_del_chord(vp[i], c, priority);
      }
      bool find_chord(bitmap_t &bmap, boost::shared_ptr<chord_type> &cd) {
        for(size_t i=0; i<chords_.size(); i++) {
          if (chords_[i]->mask_ == bmap) {
            cd = chords_[i];
            return true;
          }
        }
        return false;
      }
      bool del_chord(bitmap_t &bmap, boost::shared_ptr<chord_type> &cd) {
        typename std::vector<boost::shared_ptr<chord_type> >::iterator iter;
        for(iter = chords_.begin(); iter != chords_.end(); iter++) {
          if ((*iter)->mask_ == bmap) {
            cd = (*iter);
            chords_.erase(iter);
            return true;
          }
        }
        return false;
      }
      void chord_data_validation(bitmap_t &bmap) {
        if (static_cast<schedule_policy>(sched_type::policy) == schedule_first_match) {
          //check_hidden_chord(bitmap_t &bmap)
          for(size_t i=0; i<chords_.size(); i++) {
            if (chords_[i]->mask_.match(bmap) || bmap.match(chords_[i]->mask_))
              throw hidden_chord_exception();
          }
        }
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd))
          throw hidden_chord_exception();
      }

      template <typename PortT>
      typename port::port_type port_type(PortT &p) {
        return p.type_;
      }
      template <typename PortT, std::size_t N>
      typename port::port_type port_type(boost::array<PortT,N> &vp) {
        if (vp[0].type_ == port::synch && N > 1)
          throw single_synch_exception();
        return vp[0].type_;
      }

    public:

      //
      // ***** "factory" methods to create chords ******
      //
      //--- chord with 1 port ---
      template <typename PortT, typename CallT>
      actor_t& chord(PortT &p, CallT c, int priority=0) {
        port *sp = 0;
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if (port_type(p) == port::synch) sp = &p;
        add_port(p, bmap);
        chord_data_validation(bmap);
        boost::shared_ptr<chord_type> cd(new chord1<max_size,PortT,CallT>(bmap, sp, this, p, c, priority));
        chords_.push_back(cd);
        port_add_chord(p,cd.get(),priority);
        return *this;
      }

      template <typename PortT>
      actor_t& chord_remove(PortT &p) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!find_port(p, bmap))
          throw chord_remove_exception();
        boost::shared_ptr<chord_type> cd;
        if(!del_chord(bmap,cd))
          throw chord_remove_exception();
        port_del_chord(p, cd.get(), cd->priority_);
        return *this;
      }

      template <typename PortT, typename CallT>
      actor_t& chord_override(PortT &p, CallT c, int priority=0) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!find_port(p, bmap))
          throw chord_override_exception();
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd)) { //override chord
          chord1<max_size,PortT,CallT> *cdp = static_cast<chord1<max_size,PortT,CallT>*>(cd.get());
          cdp->call_ = c;
          if (cdp->priority_ != priority) {
            port_del_chord(p, cdp, cdp->priority_);
            port_add_chord(p, cdp, priority);
            cdp->priority_ = priority;
          }
        }
        else
          throw chord_override_exception();
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT, typename ActorT, typename R, typename ArgT>
      actor_t& chord(PortT &p,
                     R (ActorT::*c)(ArgT),
                     int priority=0) {
        return chord(p, boost::bind(c, static_cast<ActorT*>(this), _1), priority);
      }
      template <typename PortT, typename ActorT, typename R, typename ArgT>
      actor_t& chord_override(PortT &p,
                              R (ActorT::*c)(ArgT),
                              int priority=0) {
        return chord_override(p, boost::bind(c, static_cast<ActorT*>(this), _1), priority);
      }


      // ---- chord with 2 ports ----
      template <typename PortT1, typename PortT2, typename CallT>
      actor_t& chord(PortT1 &p1, PortT2 &p2, CallT c, int priority=0)
      {
        port *sp = 0;
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if (port_type(p1) == port::synch) sp = &p1;
        if (port_type(p2) == port::synch)
          throw synch_not_1st_exception();
        add_port(p1, bmap);
        add_port(p2, bmap);
        chord_data_validation(bmap);
        boost::shared_ptr<chord_type> cd(new chord2<max_size,PortT1,PortT2,CallT>(bmap, sp, this, p1, p2, c, priority));
        chords_.push_back(cd);
        port_add_chord(p1,cd.get(),priority);
        port_add_chord(p2,cd.get(),priority);
        return *this;
      }

      template <typename PortT1, typename PortT2>
      actor_t& chord_remove(PortT1 &p1, PortT2 &p2) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap)))
          throw chord_remove_exception();
        boost::shared_ptr<chord_type> cd;
        if(!del_chord(bmap, cd))
          throw chord_remove_exception();
        port_del_chord(p1, cd.get(), cd->priority_);
        port_del_chord(p2, cd.get(), cd->priority_);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename CallT>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, CallT c, int priority=0) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap)))
          throw chord_override_exception();
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd)) { //override chord
          chord2<max_size,PortT1,PortT2,CallT>* cdp = static_cast<chord2<max_size,PortT1,PortT2,CallT>*>(cd.get());
          cdp->call_ = c;
          if (cdp->priority_ != priority) {
            port_del_chord(p1, cdp, cdp->priority_);
            port_del_chord(p2, cdp, cdp->priority_);
            port_add_chord(p1, cdp, priority);
            port_add_chord(p2, cdp, priority);
            cdp->priority_ = priority;
          }
        }
        else
          throw chord_override_exception();
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename ActorT, typename R, typename ArgT1, typename ArgT2>
      actor_t& chord(PortT1 &p1, PortT2 &p2,
                     R (ActorT::*c)(ArgT1, ArgT2), int priority=0) {
        return chord(p1, p2, boost::bind(c, static_cast<ActorT*>(this), _1, _2), priority);
      }
      template <typename PortT1, typename PortT2, typename ActorT, typename R, typename ArgT1, typename ArgT2>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2,
                              R (ActorT::*c)(ArgT1, ArgT2), int priority=0) {
        return chord_override(p1, p2, boost::bind(c, static_cast<ActorT*>(this), _1, _2), priority);
      }

      //---- chord with 3 ports ---

      template <typename PortT1, typename PortT2, typename PortT3, typename CallT>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, CallT c, int priority=0)
      {
        port *sp = 0;
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if (port_type(p1) == port::synch) sp = &p1;
        if (port_type(p2) == port::synch || port_type(p3) == port::synch)
          throw synch_not_1st_exception();
        add_port(p1, bmap);
        add_port(p2, bmap);
        add_port(p3, bmap);
        chord_data_validation(bmap);
        boost::shared_ptr<chord_type> cd(new chord3<max_size,PortT1,PortT2,PortT3,CallT>(bmap, sp, this, p1, p2, p3, c, priority));
        chords_.push_back(cd);
        port_add_chord(p1,cd.get(),priority);
        port_add_chord(p2,cd.get(),priority);
        port_add_chord(p3,cd.get(),priority);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3>
      actor_t& chord_remove(PortT1 &p1, PortT2 &p2, PortT3 &p3) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap)))
          throw chord_remove_exception();
        boost::shared_ptr<chord_type> cd;
        if(!del_chord(bmap, cd))
          throw chord_remove_exception();
        port_del_chord(p1, cd.get(), cd->priority_);
        port_del_chord(p2, cd.get(), cd->priority_);
        port_del_chord(p3, cd.get(), cd->priority_);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename CallT>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, CallT c, int priority=0) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap)))
          throw chord_override_exception();
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd)) { //override chord
          chord3<max_size,PortT1,PortT2,PortT3,CallT>* cdp = static_cast<chord3<max_size,PortT1,PortT2,PortT3,CallT>*>(cd.get());
          cdp->call_ = c;
          if (cdp->priority_ != priority) {
            port_del_chord(p1, cdp, cdp->priority_);
            port_del_chord(p2, cdp, cdp->priority_);
            port_del_chord(p3, cdp, cdp->priority_);
            port_add_chord(p1, cdp, priority);
            port_add_chord(p2, cdp, priority);
            port_add_chord(p3, cdp, priority);
            cdp->priority_ = priority;
          }
        }
        else
          throw chord_override_exception();
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename PortT3, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3,
                     R (ActorT::*c)(ArgT1, ArgT2, ArgT3), int priority=0) {
        return chord(p1, p2, p3, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3), priority);
      }
      template <typename PortT1, typename PortT2, typename PortT3, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3,
                              R (ActorT::*c)(ArgT1, ArgT2, ArgT3), int priority=0) {
        return chord_override(p1, p2, p3, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3)), priority;
      }

      //----- chord with 4 ports -----

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename CallT>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, CallT c, int priority=0)
      {
        port *sp = 0;
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if (port_type(p1) == port::synch) sp = &p1;
        if (port_type(p2) == port::synch || port_type(p3) == port::synch || port_type(p4) == port::synch)
          throw synch_not_1st_exception();
        add_port(p1, bmap);
        add_port(p2, bmap);
        add_port(p3, bmap);
        add_port(p4, bmap);
        chord_data_validation(bmap);
        boost::shared_ptr<chord_type> cd(new chord4<max_size,PortT1,PortT2,PortT3,PortT4,CallT>(bmap, sp, this, p1, p2, p3, p4, c, priority));
        chords_.push_back(cd);
        port_add_chord(p1,cd.get(),priority);
        port_add_chord(p2,cd.get(),priority);
        port_add_chord(p3,cd.get(),priority);
        port_add_chord(p4,cd.get(),priority);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4>
      actor_t& chord_remove(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap)))
          throw chord_remove_exception();
        boost::shared_ptr<chord_type> cd;
        if(!del_chord(bmap, cd))
          throw chord_remove_exception();
        port_del_chord(p1, cd.get(), cd->priority_);
        port_del_chord(p2, cd.get(), cd->priority_);
        port_del_chord(p3, cd.get(), cd->priority_);
        port_del_chord(p4, cd.get(), cd->priority_);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename CallT>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, CallT c, int priority=0) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap)))
          throw chord_override_exception();
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd)) { //override chord
          chord4<max_size,PortT1,PortT2,PortT3,PortT4,CallT>* cdp = static_cast<chord4<max_size,PortT1,PortT2,PortT3,PortT4,CallT>*>(cd.get());
          cdp->call_ = c;
          if (cdp->priority_ != priority) {
            port_del_chord(p1, cdp, cdp->priority_);
            port_del_chord(p2, cdp, cdp->priority_);
            port_del_chord(p3, cdp, cdp->priority_);
            port_del_chord(p4, cdp, cdp->priority_);
            port_add_chord(p1, cdp, priority);
            port_add_chord(p2, cdp, priority);
            port_add_chord(p3, cdp, priority);
            port_add_chord(p4, cdp, priority);
            cdp->priority_ = priority;
          }
        }
        else
          throw chord_override_exception();
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4,
                     R (ActorT::*c)(ArgT1, ArgT2, ArgT3, ArgT4), int priority=0) {
        return chord(p1, p2, p3, p4, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4), priority);
      }
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4,
                              R (ActorT::*c)(ArgT1, ArgT2, ArgT3, ArgT4), int priority=0) {
        return chord_override(p1, p2, p3, p4, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4), priority);
      }

      //----- chord with 5 ports ----

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename CallT>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, CallT c, int priority=0)
      {
        port *sp = 0;
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if (port_type(p1) == port::synch) sp = &p1;
        if (port_type(p2) == port::synch || port_type(p3) == port::synch ||
            port_type(p4) == port::synch || port_type(p5) == port::synch)
          throw synch_not_1st_exception();
        add_port(p1, bmap);
        add_port(p2, bmap);
        add_port(p3, bmap);
        add_port(p4, bmap);
        add_port(p5, bmap);
        chord_data_validation(bmap);
        boost::shared_ptr<chord_type> cd(new chord5<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,CallT>(bmap, sp, this, p1, p2, p3, p4, p5, c, priority));
        chords_.push_back(cd);
        port_add_chord(p1,cd.get(),priority);
        port_add_chord(p2,cd.get(),priority);
        port_add_chord(p3,cd.get(),priority);
        port_add_chord(p4,cd.get(),priority);
        port_add_chord(p5,cd.get(),priority);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5>
      actor_t& chord_remove(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap) &&
             find_port(p5, bmap)))
          throw chord_remove_exception();
        boost::shared_ptr<chord_type> cd;
        if(!del_chord(bmap, cd))
          throw chord_remove_exception();
        port_del_chord(p1, cd.get(), cd->priority_);
        port_del_chord(p2, cd.get(), cd->priority_);
        port_del_chord(p3, cd.get(), cd->priority_);
        port_del_chord(p4, cd.get(), cd->priority_);
        port_del_chord(p5, cd.get(), cd->priority_);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename CallT>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, CallT c, int priority=0) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap) &&
             find_port(p5, bmap)))
          throw chord_override_exception();
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd)) { //override chord
          chord5<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,CallT>* cdp = static_cast<chord5<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,CallT>*>(cd.get());
          cdp->call_ = c;
          if (cdp->priority_ != priority) {
            port_del_chord(p1, cdp, cdp->priority_);
            port_del_chord(p2, cdp, cdp->priority_);
            port_del_chord(p3, cdp, cdp->priority_);
            port_del_chord(p4, cdp, cdp->priority_);
            port_del_chord(p5, cdp, cdp->priority_);
            port_add_chord(p1, cdp, priority);
            port_add_chord(p2, cdp, priority);
            port_add_chord(p3, cdp, priority);
            port_add_chord(p4, cdp, priority);
            port_add_chord(p5, cdp, priority);
            cdp->priority_ = priority;
          }
        }
        else
          throw chord_override_exception();
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5,
                     R (ActorT::*c)(ArgT1, ArgT2, ArgT3, ArgT4, ArgT5), int priority=0) {
        return chord(p1, p2, p3, p4, p5, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4, _5), priority);
      }
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5,
                              R (ActorT::*c)(ArgT1, ArgT2, ArgT3, ArgT4, ArgT5), int priority=0) {
        return chord_override(p1, p2, p3, p4, p5, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4, _5), priority);
      }

      //----- chord with 6 ports ----

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename CallT>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, CallT c, int priority=0)
      {
        port *sp = 0;
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if (port_type(p1) == port::synch) sp = &p1;
        if (port_type(p2) == port::synch || port_type(p3) == port::synch ||
            port_type(p4) == port::synch || port_type(p5) == port::synch || port_type(p6) == port::synch)
          throw synch_not_1st_exception();
        add_port(p1, bmap);
        add_port(p2, bmap);
        add_port(p3, bmap);
        add_port(p4, bmap);
        add_port(p5, bmap);
        add_port(p6, bmap);
        chord_data_validation(bmap);
        boost::shared_ptr<chord_type> cd(new chord6<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,CallT>(bmap, sp, this, p1, p2, p3, p4, p5, p6, c, priority));
        chords_.push_back(cd);
        port_add_chord(p1,cd.get(),priority);
        port_add_chord(p2,cd.get(),priority);
        port_add_chord(p3,cd.get(),priority);
        port_add_chord(p4,cd.get(),priority);
        port_add_chord(p5,cd.get(),priority);
        port_add_chord(p6,cd.get(),priority);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6>
      actor_t& chord_remove(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap) &&
             find_port(p5, bmap) &&
             find_port(p6, bmap)))
          throw chord_remove_exception();
        boost::shared_ptr<chord_type> cd;
        if(!del_chord(bmap, cd))
          throw chord_remove_exception();
        port_del_chord(p1, cd.get(), cd->priority_);
        port_del_chord(p2, cd.get(), cd->priority_);
        port_del_chord(p3, cd.get(), cd->priority_);
        port_del_chord(p4, cd.get(), cd->priority_);
        port_del_chord(p5, cd.get(), cd->priority_);
        port_del_chord(p6, cd.get(), cd->priority_);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename CallT>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, CallT c, int priority=0) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap) &&
             find_port(p5, bmap) &&
             find_port(p6, bmap)))
          throw chord_override_exception();
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd)) { //override chord
          chord6<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,CallT>* cdp = static_cast<chord6<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,CallT>*>(cd.get());
          cdp->call_ = c;
          if (cdp->priority_ != priority) {
            port_del_chord(p1, cdp, cdp->priority_);
            port_del_chord(p2, cdp, cdp->priority_);
            port_del_chord(p3, cdp, cdp->priority_);
            port_del_chord(p4, cdp, cdp->priority_);
            port_del_chord(p5, cdp, cdp->priority_);
            port_del_chord(p6, cdp, cdp->priority_);
            port_add_chord(p1, cdp, priority);
            port_add_chord(p2, cdp, priority);
            port_add_chord(p3, cdp, priority);
            port_add_chord(p4, cdp, priority);
            port_add_chord(p5, cdp, priority);
            port_add_chord(p6, cdp, priority);
            cdp->priority_ = priority;
          }
        }
        else
          throw chord_override_exception();
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6,
                     R (ActorT::*c)(ArgT1, ArgT2, ArgT3, ArgT4, ArgT5, ArgT6), int priority=0) {
        return chord(p1, p2, p3, p4, p5, p6, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4, _5, _6), priority);
      }
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6,
                              R (ActorT::*c)(ArgT1, ArgT2, ArgT3, ArgT4, ArgT5, ArgT6), int priority=0) {
        return chord_override(p1, p2, p3, p4, p5, p6, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4, _5, _6), priority);
      }


      //----- chord with 7 ports ----

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename CallT>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, CallT c, int priority=0)
      {
        port *sp = 0;
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if (port_type(p1) == port::synch) sp = &p1;
        if (port_type(p2) == port::synch || port_type(p3) == port::synch || port_type(p4) == port::synch ||
            port_type(p5) == port::synch ||port_type( p6) == port::synch || port_type(p7) == port::synch)
          throw synch_not_1st_exception();
        add_port(p1, bmap);
        add_port(p2, bmap);
        add_port(p3, bmap);
        add_port(p4, bmap);
        add_port(p5, bmap);
        add_port(p6, bmap);
        add_port(p7, bmap);
        chord_data_validation(bmap);
        boost::shared_ptr<chord_type> cd(new chord7<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,PortT7,CallT>(bmap, sp, this, p1, p2, p3, p4, p5, p6, p7, c, priority));
        chords_.push_back(cd);
        port_add_chord(p1,cd.get(),priority);
        port_add_chord(p2,cd.get(),priority);
        port_add_chord(p3,cd.get(),priority);
        port_add_chord(p4,cd.get(),priority);
        port_add_chord(p5,cd.get(),priority);
        port_add_chord(p6,cd.get(),priority);
        port_add_chord(p7,cd.get(),priority);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7>
      actor_t& chord_remove(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap) &&
             find_port(p5, bmap) &&
             find_port(p6, bmap) &&
             find_port(p7, bmap)))
          throw chord_remove_exception();
        boost::shared_ptr<chord_type> cd;
        if(!del_chord(bmap, cd))
          throw chord_remove_exception();
        port_del_chord(p1, cd.get(), cd->priority_);
        port_del_chord(p2, cd.get(), cd->priority_);
        port_del_chord(p3, cd.get(), cd->priority_);
        port_del_chord(p4, cd.get(), cd->priority_);
        port_del_chord(p5, cd.get(), cd->priority_);
        port_del_chord(p6, cd.get(), cd->priority_);
        port_del_chord(p7, cd.get(), cd->priority_);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename CallT>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, CallT c, int priority=0) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap) &&
             find_port(p5, bmap) &&
             find_port(p6, bmap) &&
             find_port(p7, bmap)))
          throw chord_override_exception();
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd)) { //override chord
          chord7<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,PortT7,CallT>* cdp = static_cast<chord7<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,PortT7,CallT>*>(cd.get());
          cdp->call_ = c;
          if (cdp->priority_ != priority) {
            port_del_chord(p1, cdp, cdp->priority_);
            port_del_chord(p2, cdp, cdp->priority_);
            port_del_chord(p3, cdp, cdp->priority_);
            port_del_chord(p4, cdp, cdp->priority_);
            port_del_chord(p5, cdp, cdp->priority_);
            port_del_chord(p6, cdp, cdp->priority_);
            port_del_chord(p7, cdp, cdp->priority_);
            port_add_chord(p1, cdp, priority);
            port_add_chord(p2, cdp, priority);
            port_add_chord(p3, cdp, priority);
            port_add_chord(p4, cdp, priority);
            port_add_chord(p5, cdp, priority);
            port_add_chord(p6, cdp, priority);
            port_add_chord(p7, cdp, priority);
            cdp->priority_ = priority;
          }
        }
        else
          throw chord_override_exception();
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6, typename ArgT7>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7,
                     R (ActorT::*c)(ArgT1,ArgT2,ArgT3,ArgT4,ArgT5,ArgT6,ArgT7), int priority=0) {
        return chord(p1, p2, p3, p4, p5, p6, p7, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4, _5, _6, _7), priority);
      }
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6, typename ArgT7>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7,
                              R (ActorT::*c)(ArgT1,ArgT2,ArgT3,ArgT4,ArgT5,ArgT6,ArgT7), int priority=0) {
        return chord_override(p1, p2, p3, p4, p5, p6, p7, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4, _5, _6, _7), priority);
      }


      //----- chord with 8 ports ----

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename PortT8, typename CallT>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, PortT8 &p8, CallT c, int priority=0)
      {
        port *sp = 0;
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if (port_type(p1) == port::synch) sp = &p1;
        if (port_type(p2) == port::synch || port_type(p3) == port::synch || port_type(p4) == port::synch ||
            port_type(p5) == port::synch || port_type(p6) == port::synch || port_type(p7) == port::synch || port_type(p8) == port::synch)
          throw synch_not_1st_exception();
        add_port(p1, bmap);
        add_port(p2, bmap);
        add_port(p3, bmap);
        add_port(p4, bmap);
        add_port(p5, bmap);
        add_port(p6, bmap);
        add_port(p7, bmap);
        add_port(p8, bmap);
        chord_data_validation(bmap);
        boost::shared_ptr<chord_type> cd(new chord8<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,PortT7,PortT8,CallT>(bmap, sp, this, p1, p2, p3, p4, p5, p6, p7, p8, c, priority));
        chords_.push_back(cd);
        port_add_chord(p1,cd.get(),priority);
        port_add_chord(p2,cd.get(),priority);
        port_add_chord(p3,cd.get(),priority);
        port_add_chord(p4,cd.get(),priority);
        port_add_chord(p5,cd.get(),priority);
        port_add_chord(p6,cd.get(),priority);
        port_add_chord(p7,cd.get(),priority);
        port_add_chord(p8,cd.get(),priority);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename PortT8>
      actor_t& chord_remove(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, PortT8 &p8) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap) &&
             find_port(p5, bmap) &&
             find_port(p6, bmap) &&
             find_port(p7, bmap) &&
             find_port(p8, bmap)))
          throw chord_remove_exception();
        boost::shared_ptr<chord_type> cd;
        if(!del_chord(bmap, cd))
          throw chord_remove_exception();
        port_del_chord(p1, cd.get(), cd->priority_);
        port_del_chord(p2, cd.get(), cd->priority_);
        port_del_chord(p3, cd.get(), cd->priority_);
        port_del_chord(p4, cd.get(), cd->priority_);
        port_del_chord(p5, cd.get(), cd->priority_);
        port_del_chord(p6, cd.get(), cd->priority_);
        port_del_chord(p7, cd.get(), cd->priority_);
        port_del_chord(p8, cd.get(), cd->priority_);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename PortT8, typename CallT>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, PortT8 &p8, CallT c, int priority=0) {
        bitmap_t bmap;
        boost::mutex::scoped_lock lock(mutex_);
        if(!(find_port(p1, bmap) &&
             find_port(p2, bmap) &&
             find_port(p3, bmap) &&
             find_port(p4, bmap) &&
             find_port(p5, bmap) &&
             find_port(p6, bmap) &&
             find_port(p7, bmap) &&
             find_port(p8, bmap)))
          throw chord_override_exception();
        boost::shared_ptr<chord_type> cd;
        if(find_chord(bmap, cd)) { //override chord
          chord8<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,PortT7,PortT8,CallT>* cdp = static_cast<chord8<max_size,PortT1,PortT2,PortT3,PortT4,PortT5,PortT6,PortT7,PortT8,CallT>*>(cd.get());
          cdp->call_ = c;
          if (cdp->priority_ != priority) {
            port_del_chord(p1, cdp, cdp->priority_);
            port_del_chord(p2, cdp, cdp->priority_);
            port_del_chord(p3, cdp, cdp->priority_);
            port_del_chord(p4, cdp, cdp->priority_);
            port_del_chord(p5, cdp, cdp->priority_);
            port_del_chord(p6, cdp, cdp->priority_);
            port_del_chord(p7, cdp, cdp->priority_);
            port_del_chord(p8, cdp, cdp->priority_);
            port_add_chord(p1, cdp, priority);
            port_add_chord(p2, cdp, priority);
            port_add_chord(p3, cdp, priority);
            port_add_chord(p4, cdp, priority);
            port_add_chord(p5, cdp, priority);
            port_add_chord(p6, cdp, priority);
            port_add_chord(p7, cdp, priority);
            port_add_chord(p8, cdp, priority);
            cdp->priority_ = priority;
          }
        }
        else
          throw chord_override_exception();
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename PortT8, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6, typename ArgT7, typename ArgT8>
      actor_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, PortT8 &p8,
                     R (ActorT::*c)(ArgT1,ArgT2,ArgT3,ArgT4,ArgT5,ArgT6,ArgT7,ArgT8), int priority=0) {
        return chord(p1, p2, p3, p4, p5, p6, p7, p8, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4, _5, _6, _7, _8), priority);
      }
      template <typename PortT1, typename PortT2, typename PortT3, typename PortT4, typename PortT5, typename PortT6, typename PortT7, typename PortT8, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6, typename ArgT7, typename ArgT8>
      actor_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, PortT4 &p4, PortT5 &p5, PortT6 &p6, PortT7 &p7, PortT8 &p8,
                              R (ActorT::*c)(ArgT1,ArgT2,ArgT3,ArgT4,ArgT5,ArgT6,ArgT7,ArgT8), int priority=0) {
        return chord_override(p1, p2, p3, p4, p5, p6, p7, p8, boost::bind(c, static_cast<ActorT*>(this), _1, _2, _3, _4, _5, _6, _7, _8), priority);
      }

    };

  class actor : public actor_t<> {
  public:
    actor(executor *s = 0,
          int hb = 0,
          const char *name = 0) :
      actor_t<>(s,hb,name) {}
  };

  //alias "actor" to "joint" for flow based programming
  typedef actor joint;

  }
}

#endif
