//
// boost/join/joint.hpp
//
// Copyright (c) 2007 - 2009  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_JOINT_HPP
#define BOOST_JOIN_JOINT_HPP

#include <vector>
#include <bitset>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/join/base/join_base.hpp>
#include <boost/join/base/exceptions.hpp>

namespace boost {
  namespace join {

    namespace detail {

      template <size_t size>
      struct bitmap {
        enum {
          max_size = size
        };
        typedef typename boost::mpl::if_c<(size<=32), unsigned int, std::bitset<size> >::type bitset_type;
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
      typename PortT::argument_type
      top(PortT *p) {
        return p->top();
      }
    
      template <typename PortT>
      std::vector<typename PortT::argument_type>
      top(std::vector<PortT*> &vp) {
        std::vector<typename PortT::argument_type> vv;
        for(size_t i=0; i<vp.size(); i++)
          vv.push_back(vp[i]->top());
        return vv;
      }
    
      template <typename PortT>
      void pop_top(PortT *p) {
        p->pop_top();
      }
    
      template <typename PortT>
      void pop_top(std::vector<PortT*> &vp) {
        for(size_t i=0; i<vp.size(); i++)
          vp[i]->pop_top();
      }

      template <typename P> struct arg_type {};
      template <typename PortT>
      struct arg_type<PortT*> {
        typedef typename PortT::argument_type result;
      };

      template <typename PortT>
      struct arg_type<std::vector<PortT*> > {
        typedef std::vector<typename PortT::argument_type> result;
      };

      template <typename P> struct res_type {};
      template <typename PortT>
      struct res_type<PortT*> {
        typedef typename PortT::result_type result;
      };
    
      template <typename PortT>
      struct res_type<std::vector<PortT*> > {
        typedef void result;
      };

      template <typename PortT>
      struct impl_ptr {
        typedef typename PortT::ImplT* result;
      };
    
      template <typename PortT>
      struct impl_ptr<std::vector<PortT> > {
        typedef std::vector<typename PortT::ImplT*> result;
      };

      template <typename PortT>
      bool is_synch_port(PortT *p) {
        return p->type_ == port::synch;
      }
      template <typename PortT>
      bool is_synch_port(std::vector<PortT*> &vp) {
        if (vp[0]->type_ == port::synch)
          throw single_synch_exception();
        return false;
      }


      //------- chord definitions ------

      template <size_t max_size>
      class chord_common {
      public:
        bitmap<max_size> mask_;// ports of this chord
        size_t num_ports_; //for scheduling policy fire_as_much_as_possible
        int priority_; //sched priority of chord: 0 - max, the bigger the lower

        chord_common(bitmap<max_size> &m, int pri) : 
          mask_(m), priority_(pri) {
          num_ports_ = m.num_of_ones();
        }
        virtual ~chord_common() {}
        virtual bool fire(port *caller_port) = 0;
      };

      //for chords which have a return type (so it must have a synch port)
      template <typename ResT>
      class chord_fire : public chord_base<ResT> {
      public:
        joint_base *joint_;
        synch_port<ResT> *synch_p_;
        chord_fire(joint_base *a, port *p) : joint_(a), synch_p_(0) {
          synch_p_ = static_cast<synch_port<ResT>*>(p);
        }
        bool fire_chord(port *caller_port) {
          joint_->log.msg("a chord fired");
          if(caller_port->type_ == port::synch) {
            //caller is sync, caller_port and synch_p_ should be the same
            //invoke callback later in the same caller thread
            synch_p_->add_chord_fired(this);
            return true;
          }
          else {//caller is async_p
            synch_p_->transfer(this);
            return false;
          }
        }      
      };

      template <>
      class chord_fire<void> : public chord_base<void> {
      public:
        joint_base *joint_;
        synch_port<void> *synch_p_;
        chord_fire(joint_base *a, port *p) : joint_(a), synch_p_(0) {
          if (is_synch_port(p)) synch_p_ = static_cast<synch_port<void>*>(p);
        }
        bool fire_chord(port *caller_port) {
          joint_->log.msg("a chord fired");
          if(caller_port->type_ == port::synch) {
            //caller is sync, caller_port and synch_p_ should be the same
            //invoke callback later in the same caller thread
            synch_p_->add_chord_fired(this);
            return true;
          }
          else {//caller is async_p
            if (synch_p_) {
              synch_p_->transfer(this);
              return false;
            }
            else { //no sync ports in chord
              if(joint_->has_spawn()) {
                //spawn the callback later
                static_cast<async_port*>(caller_port)->add_chord_fired(this);
                return true;
              } else
                throw no_executor_exception();
            }
          }
          return false;
        }      
      };

      template <size_t max_size, typename PortPtrT, typename CallT>
      class chord1 : public chord_common<max_size>, public chord_fire<typename res_type<PortPtrT>::result> {
        typedef typename res_type<PortPtrT>::result result_type;
        typedef typename arg_type<PortPtrT>::result argument_type;
      public:
        PortPtrT port_;
        boost::function1<result_type, argument_type> call_;
        chord1(bitmap<max_size> &m, joint_base* a, PortPtrT p, CallT c, int pri) : 
          chord_common<max_size>(m,pri), 
          chord_fire<result_type>(a,p),
          port_(p), call_(c) {}
        void capture_arguments(boost::function0<result_type> &cb) {
          cb = boost::bind(call_, top(port_));
          pop_top(port_);
        }      
        bool fire(port *caller_port) {
          return chord_fire<result_type>::fire_chord(caller_port);
        }
      };

      template <size_t max_size, typename PortPtrT1, typename PortPtrT2, typename CallT>
      class chord2 : public chord_common<max_size>, public chord_fire<typename res_type<PortPtrT1>::result> {
        typedef typename res_type<PortPtrT1>::result result_type;
        typedef typename arg_type<PortPtrT1>::result argument1_type;
        typedef typename arg_type<PortPtrT2>::result argument2_type;
      public:
        PortPtrT1 port1_;
        PortPtrT2 port2_;
        boost::function2<result_type, argument1_type, argument2_type> call_;
        chord2(bitmap<max_size> &m, joint_base* a, PortPtrT1 p1, PortPtrT2 p2, CallT c, int pri) : 
          chord_common<max_size>(m,pri), 
          chord_fire<result_type>(a,p1),
          port1_(p1), port2_(p2), call_(c) {}
        void capture_arguments(boost::function0<result_type> &cb) {
          cb = boost::bind(call_, top(port1_), top(port2_));
          pop_top(port1_);
          pop_top(port2_);
        }      
        bool fire(port *caller_port) {
          return chord_fire<result_type>::fire_chord(caller_port);
        }
      };

      template <size_t max_size, typename PortPtrT1, typename PortPtrT2, typename PortPtrT3, typename CallT>
      class chord3 : public chord_common<max_size>, public chord_fire<typename res_type<PortPtrT1>::result> {
        typedef typename res_type<PortPtrT1>::result result_type;
        typedef typename arg_type<PortPtrT1>::result argument1_type;
        typedef typename arg_type<PortPtrT2>::result argument2_type;
        typedef typename arg_type<PortPtrT3>::result argument3_type;
      public:
        PortPtrT1 port1_;
        PortPtrT2 port2_;
        PortPtrT3 port3_;
        boost::function3<result_type, argument1_type, argument2_type, argument3_type>  call_;
        chord3(bitmap<max_size> &m, joint_base* a, PortPtrT1 p1, PortPtrT2 p2, PortPtrT3 p3, CallT c, int pri) : 
          chord_common<max_size>(m,pri), 
          chord_fire<result_type>(a,p1),
          port1_(p1), port2_(p2), port3_(p3), call_(c) {}
        void capture_arguments(boost::function0<result_type> &cb) {
          cb = boost::bind(call_, top(port1_), top(port2_), top(port3_));
          pop_top(port1_);
          pop_top(port2_);
          pop_top(port3_);
        }      
        bool fire(port *caller_port) {
          return chord_fire<result_type>::fire_chord(caller_port);
        }
      };

    }

    //------ schedulers ------

    enum schedule_policy {
      schedule_first_match, //(first match will fire)
      schedule_longest_match,  //(longest match will fire)
      schedule_round_robin  //(round robin)
    };
    
    namespace detail {

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
        std::vector<chord_type*>  get_chords() { return chords_; }
        void port_add_chord(chord_type *c, int) {
          chords_.push_back(c);
        }
        void port_del_chord(chord_type *c, int p, bool can_detach) {
          typename std::vector<chord_type*>::iterator iter;
          if ((iter = std::find(chords_.begin(), chords_.end(), c)) != chords_.end())
            chords_.erase(iter);
          if(chords_.size() == 0) {
            this->port_->reset();
            if (can_detach) this->port_->detach_joint();
            this->port_ = 0;
          }
        }
      };
    
        
      //simple priority based schedulers
      template <size_t max_size>
      struct sched_pri_data : public sched_data_base<max_size> {
        typedef chord_common<max_size> chord_type;
        std::vector<std::vector<chord_type*> > chords_; //chords this port participates in
        sched_pri_data() : sched_data_base<max_size>() {
        }
        std::vector<chord_type*> get_chords() {
          std::vector<chord_type*> chds;
          for(size_t i=0; i<chords_.size(); i++)
            for(size_t j=0; j<chords_[i].size(); j++)
              chds.push_back(chords_[i][j]);
          return chds;
        }
        void port_add_chord(chord_type *c, int priority) {
          if(chords_.size() < ((size_t)priority+1))
            for(size_t i=chords_.size(); i<((size_t)priority+1); i++) {
              chords_.push_back(std::vector<chord_type*>());
            }
          chords_[priority].push_back(c);
        }
        void port_del_chord(chord_type *c, int priority, bool can_detach) {
          typename std::vector<chord_type*>::iterator iter;
          if ((iter = std::find(chords_[priority].begin(), chords_[priority].end(), c)) != chords_[priority].end())
            chords_[priority].erase(iter);
          bool bound = false;
          for(size_t i=0; i<chords_.size() && !bound; i++)
            if(chords_[i].size() > 0) bound = true;
          if (!bound) {
            this->port_->reset();
            if (can_detach) this->port_->detach_joint();
            this->port_ = 0;
          }
        }
      };

    }

    template <size_t max_size>
    struct sched_first_match : public detail::sched_data<max_size> {
      typedef detail::bitmap<max_size> bitmap_t;
      typedef detail::chord_common<max_size> chord_type;
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
    struct sched_longest_match : public detail::sched_data<max_size> {
      enum { policy = schedule_longest_match };
      typedef detail::bitmap<max_size> bitmap_t;
      typedef detail::chord_common<max_size> chord_type;
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
    struct sched_round_robin : public detail::sched_data<max_size> {
      enum { policy = schedule_round_robin };
      typedef detail::bitmap<max_size> bitmap_t;
      typedef detail::chord_common<max_size> chord_type;
      int last_chord_fired_;
      sched_round_robin() : detail::sched_data<max_size>(), last_chord_fired_(-1) {}
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
    
    template <size_t max_size>
    struct sched_pri_first_match : public detail::sched_pri_data<max_size> {
      enum { policy = schedule_first_match };
      typedef detail::bitmap<max_size> bitmap_t;
      typedef detail::chord_common<max_size> chord_type;
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
    struct sched_pri_longest_match : public detail::sched_pri_data<max_size> {
      enum { policy = schedule_longest_match };
      typedef detail::bitmap<max_size> bitmap_t;
      typedef detail::chord_common<max_size> chord_type;
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
    struct sched_pri_round_robin : public detail::sched_pri_data<max_size> {
      enum { policy = schedule_round_robin };
      typedef detail::bitmap<max_size> bitmap_t;
      typedef detail::chord_common<max_size> chord_type;
      std::vector<int> last_chord_fired_; //for roundrobin dispatching
      sched_pri_round_robin() : detail::sched_pri_data<max_size>() {
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
      void port_del_chord(chord_type *c, int priority, bool can_detach) {
        typename std::vector<chord_type*>::iterator iter;
        if ((iter = std::find(this->chords_[priority].begin(), this->chords_[priority].end(), c)) != this->chords_[priority].end()) {
          this->chords_[priority].erase(iter);
          last_chord_fired_[priority] = -1;
        }
        bool bound = false;
        for(size_t i=0; i<this->chords_.size() && !bound; i++)
          if(this->chords_[i].size() > 0) bound = true;
        if (!bound) {
          this->port_->reset();
          if(can_detach) this->port_->detach_joint();
          this->port_ = 0;
        }
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

    //------ joint_impl definition ------
    
    namespace detail {

      template <
        template <size_t> class scheduler=sched_first_match, 
        size_t max_size=32
        >
      class joint_impl : public joint_base, private boost::noncopyable {
        template <size_t> friend class chord_common;
        friend class port;

      public:
        typedef bitmap<max_size> bitmap_t;
        typedef scheduler<max_size> sched_type;
        typedef typename joint_base::callable callable;
        typedef function1<void, typename joint_base::callable> spawn_type;
        typedef chord_common<max_size> chord_type;
        typedef sched_type port_data;

      private:

        spawn_type spawn_;
        bitmap_t status_; //bitmap marking which port is active
        int heartbeat_;
        std::vector<port_data> ports_; //all unique ports in this joint, joint not owning
        //them and will not destroy them, joint will destroy port_data
        std::vector<boost::shared_ptr<chord_type> > chords_; //joint owns chords_ and will destroy them

      public:
        joint_impl(spawn_type s = 0, 
                   int hb = 0, //forever
                   const char *name = 0) : 
          joint_base(name), spawn_(s), status_(0), heartbeat_(hb) {}

        ~joint_impl() {
          log.msg("joint is being destructed...");
          reset();
          log.msg("joint is dead now...");
        }

        void reset() {
          log.msg("joint reset...");
          boost::mutex::scoped_lock lock(mutex_);
          internal_reset();
        }

        bool has_spawn(void) { return spawn_ != 0; }
        void spawn(callable c) {
          spawn_(c);
        }

        schedule_policy my_scheduler(void) { return static_cast<schedule_policy>(sched_type::policy); }
      
        bool check_heartbeat() {
          if (heartbeat_ > 0) {
            heartbeat_--;
            if (heartbeat_ == 0) {
              log.msg("heartbeat expires, joint reset...");
              internal_reset();
              return true;
            }
          }
          return false;
        }

      private:
        void internal_reset() {
          chords_remove(); //remove all remaining chords
          ports_.clear();
          chords_.clear();
          status_ = bitmap_t(0);
        }

        void port_remove(int ind) {
          boost::mutex::scoped_lock lock(mutex_);
          log.stream() << "port_remove: port [" << ind << "]" << logger::endl;
          port_data &pd(ports_[ind]);
          std::vector<chord_type*> chds = pd.get_chords();
          for(size_t i=0; i<chds.size(); i++) {
            chord_type *chd = chds[i];
            for(size_t j=0; j<ports_.size(); j++) {
              if(ports_[j].port_ != 0 && chd->mask_.test(ports_[j].mask_)) {
                if((size_t)ind == j)
                  ports_[j].port_del_chord(chd, chd->priority_, false);
                else
                  ports_[j].port_del_chord(chd, chd->priority_, true);
              }
            }
            del_chord(chd->mask_);
          }
        }

        void chords_remove() {
          typename std::vector<boost::shared_ptr<chord_type> >::iterator iter;
          for(iter = chords_.begin(); iter != chords_.end(); ) {
            boost::shared_ptr<chord_type> chd = *iter;
            for(size_t j=0; j<ports_.size(); j++) {
              if (ports_[j].port_ != 0 && chd->mask_.test(ports_[j].mask_)) {
                ports_[j].port_del_chord(chd.get(), chd->priority_, true);
              }
            }
            log.msg("one chord removed");
            iter = chords_.erase(iter);
          }
        }

        //ports call this to notify that a new msg comes
        //return: true - chord fired, false - chord not fired
        bool port_invoked(int ind) {
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
          return false; //no chord fired
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
        void add_port(PortT *pp, bitmap_t &bmap) {
          port_data pd;
          int ind = -1;
          int empty = -1;
          for(size_t i=0; i<ports_.size() && ind == -1;i++)
            if(pp == ports_[i].port_) {
              ind = (int)i;
              pd = ports_[i];
            }
            else if (ports_[i].port_ == 0 && empty == -1)
              empty = i;
          if (ind == -1) {
            if (empty >= 0) ind = empty;
            else ind = (int)ports_.size();
            if((size_t)ind >= max_size) {
              log.msg("too_many_ports_exception thrown");
              throw too_many_ports_exception();
            }
            pd.port_ = pp;
            pd.mask_ = bitmap_t(1<<ind);
            ports_.push_back(pd);
            pp->index_ = ind;
            pp->num_pending_ = 0;
          }
          bmap.set(pd.mask_);
        }
        template <typename PortT>
        void add_port(std::vector<PortT*> &vp, bitmap_t &bmap) {
          for(size_t i=0; i<vp.size(); i++) 
            add_port(vp[i], bmap);
        }
        template <typename PortT>
        void find_port(PortT *p, bitmap_t &bmap) {
          bmap.set(ports_[p->index_].mask_);
        }
        template <typename PortT>
        void find_port(std::vector<PortT*> &vp, bitmap_t &bmap) {
          for(size_t i=0; i<vp.size(); i++) 
            find_port(vp[i], bmap);
        }
        template <typename PortT>
        void port_add_chord(PortT *p, chord_type *c, int priority) {
          port_data &pd(ports_[p->index_]);
          pd.port_add_chord(c, priority);
        }
        template <typename PortT>
        void port_add_chord(std::vector<PortT*> &vp, chord_type *c, int priority) {
          for(size_t i=0; i<vp.size(); i++)
            port_add_chord(vp[i], c, priority);
        }
        template <typename PortT>
        void port_del_chord(PortT *p, chord_type *c, int priority) {
          port_data &pd(ports_[p->index_]);
          pd.port_del_chord(c, priority, true);
        }
        template <typename PortT>
        void port_del_chord(std::vector<PortT*> &vp, chord_type *c, int priority) {
          for(size_t i=0; i<vp.size(); i++)
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
        
        void del_chord(bitmap_t &bmap) {
          log.msg("del_chord...");
          typename std::vector<boost::shared_ptr<chord_type> >::iterator iter;
          for(iter = chords_.begin(); iter != chords_.end(); iter++) {
            if ((*iter)->mask_ == bmap) {
              chords_.erase(iter);
              return;
            }
          }
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

      public:

        //
        // ***** "factory" methods to create chords ******
        //
        //--- chord with 1 port ---
        template <typename PortPtrT, typename CallT>
        void chord(PortPtrT p, CallT c, int priority) {
          bitmap_t bmap;
          add_port(p, bmap);
          is_synch_port(p); // check against vector of synch
          chord_data_validation(bmap);
          boost::shared_ptr<chord_type> cd(new chord1<max_size,PortPtrT,CallT>(bmap, this, p, c, priority));
          chords_.push_back(cd);
          port_add_chord(p,cd.get(),priority);
        }

        template <typename PortPtrT>
        void chord_remove(PortPtrT p) {
          bitmap_t bmap;
          find_port(p, bmap);
          boost::shared_ptr<chord_type> cd;
          if(!del_chord(bmap,cd))
            throw chord_remove_exception();
          port_del_chord(p, cd.get(), cd->priority_);
        }

        template <typename PortPtrT, typename CallT>
        void chord_override(PortPtrT p, CallT c, int priority) {
          bitmap_t bmap;
          find_port(p, bmap); 
          boost::shared_ptr<chord_type> cd;
          if(find_chord(bmap, cd)) { //override chord
            chord1<max_size,PortPtrT,CallT> *cdp = static_cast<chord1<max_size,PortPtrT,CallT>*>(cd.get());
            cdp->call_ = c;
            if (cdp->priority_ != priority) {
              port_del_chord(p, cdp, cdp->priority_);
              port_add_chord(p, cdp, priority);
              cdp->priority_ = priority;
            }
          }
          else
            throw chord_override_exception();
        }

        // ---- chord with 2 ports ----
        template <typename PortPtrT1, typename PortPtrT2, typename CallT>
        void chord(PortPtrT1 p1, PortPtrT2 p2, CallT c, int priority)
        {
          log.msg("joint_impl::chord 2 ports");
          bitmap_t bmap;
          is_synch_port(p1); // check against vector of synch
          if (is_synch_port(p2))
            throw synch_not_1st_exception();
          add_port(p1, bmap);
          add_port(p2, bmap);
          chord_data_validation(bmap);
          boost::shared_ptr<chord_type> cd(new chord2<max_size,PortPtrT1,PortPtrT2,CallT>(bmap, this, p1, p2, c, priority));
          chords_.push_back(cd);
          port_add_chord(p1,cd.get(),priority);
          port_add_chord(p2,cd.get(),priority);
        }

        template <typename PortPtrT1, typename PortPtrT2>
        void chord_remove(PortPtrT1 p1, PortPtrT2 p2) {
          bitmap_t bmap;
          find_port(p1, bmap); 
          find_port(p2, bmap);
          boost::shared_ptr<chord_type> cd;
          if(!del_chord(bmap, cd))
            throw chord_remove_exception();
          port_del_chord(p1, cd.get(), cd->priority_);
          port_del_chord(p2, cd.get(), cd->priority_);
        }

        template <typename PortPtrT1, typename PortPtrT2, typename CallT>
        void chord_override(PortPtrT1 p1, PortPtrT2 p2, CallT c, int priority) {
          bitmap_t bmap;
          find_port(p1, bmap);
          find_port(p2, bmap);
          boost::shared_ptr<chord_type> cd;
          if(find_chord(bmap, cd)) { //override chord
            chord2<max_size,PortPtrT1,PortPtrT2,CallT>* cdp = static_cast<chord2<max_size,PortPtrT1,PortPtrT2,CallT>*>(cd.get());
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
        }

        //---- chord with 3 ports ---

        template <typename PortPtrT1, typename PortPtrT2, typename PortPtrT3, typename CallT>
        void chord(PortPtrT1 p1, PortPtrT2 p2, PortPtrT3 p3, CallT c, int priority)
        {
          bitmap_t bmap;
          is_synch_port(p1); // check against vector of synch
          if (is_synch_port(p2) || is_synch_port(p3))
            throw synch_not_1st_exception();
          add_port(p1, bmap);
          add_port(p2, bmap);
          add_port(p3, bmap);
          chord_data_validation(bmap);
          boost::shared_ptr<chord_type> cd(new chord3<max_size,PortPtrT1,PortPtrT2,PortPtrT3,CallT>(bmap, this, p1, p2, p3, c, priority));
          chords_.push_back(cd);
          port_add_chord(p1,cd.get(),priority);
          port_add_chord(p2,cd.get(),priority);
          port_add_chord(p3,cd.get(),priority);
        }

        template <typename PortPtrT1, typename PortPtrT2, typename PortPtrT3>
        void chord_remove(PortPtrT1 &p1, PortPtrT2 &p2, PortPtrT3 &p3) {
          bitmap_t bmap;
          find_port(p1, bmap);
          find_port(p2, bmap);
          find_port(p3, bmap);
          boost::shared_ptr<chord_type> cd;
          if(!del_chord(bmap, cd))
            throw chord_remove_exception();
          port_del_chord(p1, cd.get(), cd->priority_);
          port_del_chord(p2, cd.get(), cd->priority_);
          port_del_chord(p3, cd.get(), cd->priority_);
        }

        template <typename PortPtrT1, typename PortPtrT2, typename PortPtrT3, typename CallT>
        void chord_override(PortPtrT1 p1, PortPtrT2 p2, PortPtrT3 p3, CallT c, int priority) {
          bitmap_t bmap;
          find_port(p1, bmap);
          find_port(p2, bmap);
          find_port(p3, bmap);       
          boost::shared_ptr<chord_type> cd;
          if(find_chord(bmap, cd)) { //override chord
            chord3<max_size,PortPtrT1,PortPtrT2,PortPtrT3,CallT>* cdp = static_cast<chord3<max_size,PortPtrT1,PortPtrT2,PortPtrT3,CallT>*>(cd.get());
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
        }

      };

    }

    template <
      template <size_t> class scheduler=sched_first_match, 
      size_t max_size=32
      >
    class joint_t {
    public:
      typedef detail::joint_impl<scheduler, max_size> ImplT;
      typedef typename ImplT::sched_type sched_type;
      typedef typename ImplT::callable callable;
      typedef typename ImplT::spawn_type spawn_type;

    private:
      boost::shared_ptr<ImplT> pimpl_;

      template <typename PortT>
      typename PortT::ImplT* get_impl_ptr(PortT &p) {
        if (p.pimpl_ == 0 || p.pimpl_->joint_ == 0) {
          log->msg("a new port impl is created");
          p.pimpl_.reset(new typename PortT::ImplT(p.max_size_, pimpl_));
        }
        if(p.pimpl_->joint_ != 0 && p.pimpl_->joint_ != pimpl_) 
          throw double_association_exception();
        return p.pimpl_.get();
      }

      template <typename PortT>
      std::vector<typename PortT::ImplT*> get_impl_ptr(std::vector<PortT> &vp) {
        if (vp[0].pimpl_ == 0 || vp[0].pimpl_->joint_ == 0) {
          for(size_t i=0; i<vp.size(); i++)
            vp[i].pimpl_.reset(new typename PortT::ImplT(vp[i].max_size_, pimpl_));
        }
        if(vp[0].pimpl_->joint_ != 0 && vp[0].pimpl_->joint_ != pimpl_) 
          throw double_association_exception();
        std::vector<typename PortT::ImplT*> vimpl;
        for(size_t i=0; i<vp.size(); i++)
          vimpl.push_back(vp[i].pimpl_.get());
        return vimpl;
      }

      template <typename PortT, typename exception_type>
      typename PortT::ImplT* check_port(PortT &p) {
        if(p.pimpl_ == 0 || p.pimpl_->joint_ == 0 || p.pimpl_->joint_ != pimpl_) 
          throw new exception_type();
        return p.pimpl_.get();
      }

      template <typename PortT, typename exception_type>
      std::vector<typename PortT::ImplT*> check_port(std::vector<PortT> &vp) {
        std::vector<typename PortT::ImplT*> vimpl;
        for(size_t i=0; i<vp.size(); i++) 
          vimpl.push_back(check_port<PortT, exception_type>(vp[i]));
        return vimpl;
      }

    public:
      logger *log;

      joint_t(spawn_type s = 0, 
              int hb = 0, //forever
              const char *name = 0) : pimpl_(new ImplT(s, hb, name)), log(&(pimpl_->log)) {
        log->msg("joint_t constructor");
      }

      schedule_policy my_scheduler(void) { 
        return pimpl_->my_scheduler(); 
      }

      void reset() { pimpl_->reset(); }

      //
      // ***** "factory" methods to create chords ******
      //
      //--- chord with 1 port ---
      template <typename PortT, typename CallT>
      joint_t& chord(PortT &p, CallT c, int priority=0) {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT>::result pp = get_impl_ptr(p);
        pimpl_->chord(pp, c, priority);
        return *this;
      }

      template <typename PortT>
      joint_t& chord_remove(PortT &p) {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT>::result pp = check_port<PortT,chord_remove_exception>(p);
        pimpl_->chord_remove(pp);
        return *this;
      }

      template <typename PortT, typename CallT>
      joint_t& chord_override(PortT &p, CallT c, int priority=0) {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT>::result pp = check_port<PortT,chord_override_exception>(p);
        pimpl_->chord_override(pp, c, priority);
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT, typename ActorT, typename R, typename ArgT>
      joint_t& chord(PortT &p, 
                 R (ActorT::*c)(ArgT), 
                 int priority=0) {
        return chord(p, boost::bind(c, static_cast<ActorT*>(this), ::_1), priority);
      }
      template <typename PortT, typename ActorT, typename R, typename ArgT>
      joint_t& chord_override(PortT &p, 
                          R (ActorT::*c)(ArgT), 
                          int priority=0) {
        return chord_override(p, boost::bind(c, static_cast<ActorT*>(this), ::_1), priority);
      }


      // ---- chord with 2 ports ----
      template <typename PortT1, typename PortT2, typename CallT>
      joint_t& chord(PortT1 &p1, PortT2 &p2, CallT c, int priority=0)
      {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT1>::result pp1 = get_impl_ptr(p1);
        typename detail::impl_ptr<PortT2>::result pp2 = get_impl_ptr(p2);
        pimpl_->chord(pp1, pp2, c, priority);
        return *this;
      }

      template <typename PortT1, typename PortT2>
      joint_t& chord_remove(PortT1 &p1, PortT2 &p2) {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT1>::result pp1 = check_port<PortT1,chord_remove_exception>(p1);
        typename detail::impl_ptr<PortT2>::result pp2 = check_port<PortT2,chord_remove_exception>(p2);
        pimpl_->chord_remove(pp1, pp2);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename CallT>
      joint_t& chord_override(PortT1 &p1, PortT2 &p2, CallT c, int priority=0) {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT1>::result pp1 = check_port<PortT1,chord_override_exception>(p1);
        typename detail::impl_ptr<PortT2>::result pp2 = check_port<PortT2,chord_override_exception>(p2);
        pimpl_->chord_override(pp1, pp2, c, priority);
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename ActorT, typename R, typename ArgT1, typename ArgT2>
      joint_t& chord(PortT1 &p1, PortT2 &p2, 
                 R (ActorT::*c)(ArgT1, ArgT2), int priority=0) {
        return chord(p1, p2, boost::bind(c, static_cast<ActorT*>(this), ::_1, ::_2), priority);
      }
      template <typename PortT1, typename PortT2, typename ActorT, typename R, typename ArgT1, typename ArgT2>
      joint_t& chord_override(PortT1 &p1, PortT2 &p2, 
                          R (ActorT::*c)(ArgT1, ArgT2), int priority=0) {
        return chord_override(p1, p2, boost::bind(c, static_cast<ActorT*>(this), ::_1, ::_2), priority);
      }

      //---- chord with 3 ports ---

      template <typename PortT1, typename PortT2, typename PortT3, typename CallT>
      joint_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, CallT c, int priority=0)
      {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT1>::result pp1 = get_impl_ptr(p1);
        typename detail::impl_ptr<PortT2>::result pp2 = get_impl_ptr(p2);
        typename detail::impl_ptr<PortT3>::result pp3 = get_impl_ptr(p3);
        pimpl_->chord(pp1, pp2, pp3, c, priority);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3>
      joint_t& chord_remove(PortT1 &p1, PortT2 &p2, PortT3 &p3) {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT1>::result pp1 = check_port<PortT1,chord_remove_exception>(p1);
        typename detail::impl_ptr<PortT2>::result pp2 = check_port<PortT2,chord_remove_exception>(p2);
        typename detail::impl_ptr<PortT3>::result pp3 = check_port<PortT3,chord_remove_exception>(p3);
        pimpl_->chord_remove(pp1, pp2, pp3);
        return *this;
      }

      template <typename PortT1, typename PortT2, typename PortT3, typename CallT>
      joint_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, CallT c, int priority=0) {
        boost::mutex::scoped_lock lock(pimpl_->mutex_);
        typename detail::impl_ptr<PortT1>::result pp1 = check_port<PortT1,chord_override_exception>(p1);
        typename detail::impl_ptr<PortT2>::result pp2 = check_port<PortT2,chord_override_exception>(p2);
        typename detail::impl_ptr<PortT3>::result pp3 = check_port<PortT3,chord_override_exception>(p3);
        pimpl_->chord_override(pp1, pp2, pp3, c, priority);
        return *this;
      }

      //wrappers for pointers to member methods as chord body
      template <typename PortT1, typename PortT2, typename PortT3, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3>
      joint_t& chord(PortT1 &p1, PortT2 &p2, PortT3 &p3, 
                 R (ActorT::*c)(ArgT1, ArgT2, ArgT3), int priority=0) {
        return chord(p1, p2, p3, boost::bind(c, static_cast<ActorT*>(this), ::_1, ::_2, ::_3), priority);
      }
      template <typename PortT1, typename PortT2, typename PortT3, typename ActorT, typename R, typename ArgT1, typename ArgT2, typename ArgT3>
      joint_t& chord_override(PortT1 &p1, PortT2 &p2, PortT3 &p3, 
                          R (ActorT::*c)(ArgT1, ArgT2, ArgT3), int priority=0) {
        return chord_override(p1, p2, p3, boost::bind(c, static_cast<ActorT*>(this), ::_1, ::_2, ::_3)), priority;
      }

    };

    class joint : public joint_t<> {
    public:
      joint(spawn_type s = 0, 
            int hb = 0,
            const char *name = 0) : 
        joint_t<>(s,hb,name) {}
    };

    //utility function to define large/wide joint
    template <
      template <size_t> class scheduler, 
      size_t max_size
      >
    joint_t<scheduler, max_size> joins_t(joint::spawn_type s = 0, int hb = 0, const char *name = 0) {
      return joint_t<scheduler, max_size>(s,hb,name);
    }

    //utility function to define small/default joint
    joint joins(joint::spawn_type s = 0, int hb = 0, const char *name = 0) {
      return joint(s,hb,name);
    }

  }
}

#endif
