//
// arbiters_sync.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef ARBITERS_SYNC_HPP
#define ARBITERS_SYNC_HPP

#include <deque>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/channel/queues/unbounded_queue.hpp>
#include <boost/channel/dispatchers/pull_dispatcher_base.hpp>

namespace boost {
  namespace channel {

    namespace detail {

      /**
       * synchronous arbiters:
       * message passing synchronization patterns (choice, join)
       * part of pull_dispatcher
       * see CCR and Comega document
       */
      /**
       * choice
       */
      template<typename name_space, typename platform,
               template<class , class , class > class queue_type = unbounded_que>
      class choice_arbiter_sync {
      public:
	typedef pull_recver_base<name_space, platform, queue_type> recver_type;
	typedef pull_sender_base<name_space, platform, queue_type> sender_type;
	typedef	typename name_space::id_type id_type;
	typedef typename name_space::name name;
	typedef typename name_space::synch_policy synch_policy;
	typedef message<id_type> msg_type;
	typedef named_in<name_space, recver_type> named_in_type;
	typedef named_out<name_space, sender_type> named_out_type;
	typedef choice_arbiter_sync<name_space,platform,queue_type> my_type;

	typedef boost::function1<void, boost::shared_ptr<void> > call_back_t;

	name_space *ch_;
	struct entry {
          named_in_type *in_;
          call_back_t handler_;
          ~entry() {
            if (in_ != NULL) delete in_;
          }
	};
	std::map<id_type, entry*> ins_map_;
	std::deque<entry*> ins_;
	typename synch_policy::mutex mutex_;
	typename synch_policy::condition cond_;
	int num_waiting_;
	bool ready_in_set;
	named_in_type *ready_in;

	choice_arbiter_sync(name_space &ch) :
          ch_(&ch), num_waiting_(0), ready_in_set(false), ready_in(NULL) {}

	~choice_arbiter_sync() {
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::deque<entry*>::iterator iter;
          for (iter = ins_.begin(); iter != ins_.end(); iter++)
            if ((*iter) != NULL) {
              delete (*iter);
            }
	}

	//recving thread will block here and wait for notify from senders
	void recv(id_type & id, boost::shared_ptr<void> & msg) {
          boost::shared_ptr<msg_type> mesg;
          typename std::deque<entry*>::iterator iter;
          typename synch_policy::scoped_lock lock(mutex_);
          while(1) {
            //pull data
            if (ready_in_set) {
              ready_in_set = false;
              named_in_type *in = ready_in;
              bool found = false;
              for(iter = ins_.begin(); iter != ins_.end() && !found; iter++)
                if(in == (*iter)->in_) {
                  found = true;
                  break;
                }
              if(found && in->claim()) {
                if (in->pull(mesg) > 0) {
                  if ((*iter)->handler_)
                    (*iter)->handler_(mesg->data_);
                  id = mesg->id_;
                  msg = mesg->data_;
                  return;
                }
              }
            }
            else {
              for(iter = ins_.begin(); iter != ins_.end(); iter++) {
                named_in_type *in = (*iter)->in_;
                if(in->claim()) {
                  if (in->pull(mesg) > 0) {
                    if ((*iter)->handler_)
                      (*iter)->handler_(mesg->data_);
                    id = mesg->id_;
                    msg = mesg->data_;
                    return;
                  }
                }
              }
            }
            num_waiting_++;
            cond_.wait(lock);
            num_waiting_--;
          }
	}

	//recving thread will block here and wait for notify from senders
	void wait(void) {
          boost::shared_ptr<msg_type> mesg;
          typename std::map<id_type, entry>::iterator iter;
          typename synch_policy::scoped_lock lock(mutex_);
          while(1) {
            //pull data
            if (ready_in_set) {
              ready_in_set = false;
              named_in_type *in = ready_in;
              bool found = false;
              for(iter = ins_.begin(); iter != ins_.end() && !found; iter++)
                if(in == iter->in_) {
                  found = true;
                  break;
                }
              if(found && in->claim()) {
                if (in->pull(mesg) > 0) {
                  if (iter->handler_)
                    iter->handler_(mesg->data_);
                  return;
                }
              }
            }
            else {
              for(iter = ins_.begin(); iter != ins_.end(); iter++) {
                named_in_type *in = iter->in_;
                if(in->claim()) {
                  if (in->pull(mesg) > 0) {
                    if (iter->handler_)
                      iter->handler_(mesg->data_);
                    return;
                  }
                }
              }
            }
            num_waiting_++;
            cond_.wait(lock);
            num_waiting_--;
          }
	}

	//called from inside dispatcher
	void invoke(named_in_type * &ni, id_type, boost::shared_ptr<void>) {
          typename synch_policy::scoped_lock lock(mutex_);
          ready_in = ni;
          ready_in_set = true;
          if (num_waiting_ > 0)
            cond_.notify_one();
	}

	bool bind(id_type & id,
                  call_back_t cb = 0,
                  typename named_in_type::scope_type scope = named_in_type::scope_global) {
          if (ch_ == NULL) return false;
          typename synch_policy::scoped_lock lock(mutex_);
          if (ch_ != NULL && ins_map_.find(id) == ins_map_.end()) {
            entry *ent = new entry();
            ent->in_ = new named_in_type(*ch_,id,
                                         boost::bind(&my_type::invoke, this, boost::ref(ent->in_), _1, _2),
                                         scope);
            ent->handler_ = cb;
            ins_.push_back(ent);
            ins_map_[id] = ent;
            return true;
          }
          return false;
	}

	bool unbind(id_type & id) {
          if (ch_ == NULL) return false;
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::map<id_type, entry*>::iterator iter = ins_map_.find(id);
          if (iter != ins_map_.end()) {
            delete iter->second;
            ins_.erase(iter->second);
            ins_map_.erase(iter);
            return true;
          }
          return false;
	}

	bool bind(named_out_type & no, call_back_t cb) {
          typename synch_policy::scoped_lock lock(mutex_);
          entry * ent = new entry();
          ent->in_ = new named_in_type(boost::bind(&my_type::invoke, this, boost::ref(ent->in_), _1, _2));
          ent->handler_ = cb;
          ins_.push_back(ent);
          //bind named_out and named_in
          no.bind(ent->in_);
          ent->in_->bind(&no);
          return true;
	}

	bool unbind(named_out_type & no) {
          typename synch_policy::scoped_lock lock(mutex_);
          std::list<name *> & bindings = no.bindings();
          typename std::list<name *>::iterator iter, curr;
          typename std::deque<entry*>::iterator iterq;
          for(iter = bindings.begin(); iter != bindings.end(); ) {
            curr = iter; iter++;
            named_in_type *ni = (*curr);
            iterq = ins_.find(ni);
            if (iterq != ins_.end()) {
              ins_.erase(ni);
              delete ni;
            }
          }
          return true;
	}

      };

      /**
       * join
       */
      template <typename name_space, typename platform,
                template <class,class,class> class queue_type = unbounded_que>
      class join_arbiter_sync {
      public:
      public:
	typedef pull_recver_base<name_space,platform,queue_type> recver_type;
	typedef pull_sender_base<name_space,platform,queue_type> sender_type;
	typedef typename name_space::id_type id_type;
	typedef typename name_space::name name;
	typedef typename name_space::synch_policy synch_policy;
	typedef message<id_type> msg_type;
	typedef named_in<name_space, recver_type> named_in_type;
	typedef named_out<name_space, sender_type> named_out_type;
	typedef join_arbiter_sync<name_space,platform,queue_type> my_type;

	typedef boost::function1<void, std::vector<boost::shared_ptr<void> >& > call_back_t;

	name_space *ch_;
	call_back_t handler_;
	std::deque<named_in_type*> ins_; //keep parameter order
	std::map<id_type, named_in_type*> ins_map_; //keep locking order
	typename synch_policy::mutex mutex_;
	typename synch_policy::condition cond_;
	int num_waiting_;

	join_arbiter_sync(name_space &ch,
                          std::vector<std::pair<id_type, typename named_in_type::scope_type> > & ids,
                          call_back_t cb = 0) :
          ch_(&ch),
          handler_(cb), num_waiting_(0) {
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::vector<std::pair<id_type,
            typename named_in_type::scope_type> >::iterator iter;
          for(iter = ids.begin(); iter != ids.end(); iter++) {
            named_in_type * ni = new named_in_type(*ch_,iter->first,
                                                   boost::bind(&my_type::invoke, this, _1, _2),
                                                   iter->second);
            ins_.push_back(ni);
            ins_map_[iter->first] = ni;
          }
	}

	join_arbiter_sync(name_space &ch,
                          call_back_t cb = 0) :
          ch_(&ch),
          handler_(cb), num_waiting_(0) {
	}

	join_arbiter_sync(call_back_t cb = 0) :
          ch_(NULL),
          handler_(cb), num_waiting_(0) {
	}

	~join_arbiter_sync() {
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::deque<named_in_type*>::iterator iter;
          for (iter = ins_.begin(); iter != ins_.end(); iter++)
            delete (*iter);
	}

	//called from inside dispatcher
	//2 phase protocol
	void recv(std::vector<boost::shared_ptr<void> > & result) {
          boost::shared_ptr<msg_type> msg;
          typename std::deque<named_in_type*>::iterator iter;
          typename synch_policy::scoped_lock lock(mutex_);
          while(1) {
            bool claimed = true;
            //first claim the messages in proper order based on ids to avoid deadlock
            for (iter = ins_.begin(); iter != ins_.end() && claimed; iter++) {
              named_in_type *in = (*iter);
              if(!in->claim()) {
                //roll back
                for (typename std::deque<named_in_type*>::iterator iter2 = ins_.begin();
                     iter2 != iter; iter2++)
                  (*iter2)->unclaim();
                claimed = false;
              }
            }
            if(claimed) {
              //reaching here, we have claimed all the member messages,
              //commit (pull msgs and run callback)
              result.clear();
              for (iter = ins_.begin(); iter != ins_.end(); iter++) {
                named_in_type *in = (*iter);
                msg.reset();
                in->pull(msg);
                result.push_back(msg->data_);
              }
              if(handler_)
                handler_(result);
              return;
            } else {
              num_waiting_++;
              cond_.wait(lock);
              num_waiting_--;
            }
          }
	}
	void wait(void) {
          boost::shared_ptr<msg_type> msg;
          std::vector<boost::shared_ptr<void> > result;
          typename std::deque<named_in_type*>::iterator iter;
          typename synch_policy::scoped_lock lock(mutex_);
          while(1) {
            bool claimed = true;
            //first claim the messages in proper order based on ids to avoid deadlock
            for (iter = ins_.begin(); iter != ins_.end() && claimed; iter++) {
              named_in_type *in = (*iter);
              if(!in->claim()) {
                //roll back
                for (typename std::deque<named_in_type*>::iterator iter2 = ins_.begin();
                     iter2 != iter; iter2++)
                  (*iter2)->unclaim();
                claimed = false;
              }
            }
            if(claimed) {
              //reaching here, we have claimed all the member messages,
              //commit (pull msgs and run callback)
              result.clear();
              for (iter = ins_.begin(); iter != ins_.end(); iter++) {
                named_in_type *in = (*iter);
                msg.reset();
                in->pull(msg);
                result.push_back(msg->data_);
              }
              if(handler_)
                handler_(result);
              return;
            } else {
              num_waiting_++;
              cond_.wait(lock);
              num_waiting_--;
            }
          }
	}

	void invoke(id_type, boost::shared_ptr<void>) {
          typename synch_policy::scoped_lock lock(mutex_);
          if (num_waiting_ > 0)
            cond_.notify_one();
	}

	bool bind(id_type & id,
                  typename named_in_type::scope_type scope = named_in_type::scope_global) {
          if (ch_ == NULL) return false;
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::deque<named_in_type*>::iterator iter;
          for(iter = ins_.begin(); iter != ins_.end(); iter++)
            if ((*iter)->id_ == id)
              return false;
          named_in_type * ni = new named_in_type(*ch_,id,
                                                 boost::bind(&my_type::invoke, this, _1, _2),
                                                 scope);
          ins_.push_back(ni); //keep order to avoid deadlock
          ins_map_[id] = ni;
          return true;
	}

	bool unbind(id_type & id) {
          if (ch_ == NULL) return false;
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::deque<named_in_type*>::iterator iter;
          for(iter = ins_.begin(); iter != ins_.end(); iter++)
            if ((*iter)->id_ == id) {
              ins_map_.erase(id);
              delete (*iter);
              ins_.erase(iter);
              return true;
            }
          return false;
	}

	bool bind(named_out_type & no, call_back_t cb) {
          typename synch_policy::scoped_lock lock(mutex_);
          named_in_type * ni = new named_in_type(boost::bind(&my_type::invoke, this, _1, _2));
          ins_.push_back(ni);
          //bind named_out and named_in
          no.bind(ni);
          ni->bind(&no);
          return true;
	}

	bool unbind(named_out_type & no) {
          typename synch_policy::scoped_lock lock(mutex_);
          std::list<name *> & bindings = no.bindings();
          typename std::list<name *>::iterator iter, curr;
          typename std::deque<name *>::iterator iterq;
          for(iter = bindings.begin(); iter != bindings.end(); ) {
            curr = iter; iter++;
            named_in_type *ni = (*curr);
            iterq = ins_.find(ni);
            if (iterq != ins_.end()) {
              ins_.erase(ni);
              delete ni;
            }
          }
          return true;
	}
      };

    }
  }
}

#endif
