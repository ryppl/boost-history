//
// arbiters_async.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef ARBITERS_ASYNC_HPP
#define ARBITERS_ASYNC_HPP

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
       * Async arbiters:
       * asynch message passing synchronization patterns (choice, join)
       * part of pull_dispatcher
       * see CCR and Comega document
       */
      /**
       * choice
       */
      template<typename name_space, typename platform,
               template<class , class , class > class queue_type = unbounded_que>
      class choice_arbiter_async {
      public:
	typedef pull_recver_base<name_space, platform, queue_type> recver_type;
	typedef pull_sender_base<name_space, platform, queue_type> sender_type;
        typedef	typename name_space::id_type id_type;
	typedef typename name_space::name name;
	typedef typename name_space::synch_policy synch_policy;
	typedef typename name_space::executor executor;
	typedef message<id_type> msg_type;
	typedef named_in<name_space, recver_type> named_in_type;
	typedef named_out<name_space, sender_type> named_out_type;
	typedef choice_arbiter_async<name_space,platform,queue_type> my_type;

	typedef boost::function1<void, boost::shared_ptr<void> > call_back_t;

	name_space *ch_;
	executor *exec_;
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

	choice_arbiter_async(name_space &ch, executor *e = NULL) :
          ch_(&ch),
          exec_(e!=NULL?e:ch.get_exec()) {}

	choice_arbiter_async(executor *e = NULL) :
          ch_(NULL),
          exec_(e) {}

	~choice_arbiter_async() {
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::deque<entry*>::iterator iter;
          for (iter = ins_.begin(); iter != ins_.end(); iter++)
            if ((*iter) != NULL) {
              delete (*iter);
            }
	}

	//called from inside dispatcher
	void invoke(named_in_type * &in, id_type, boost::shared_ptr<void>) {
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::deque<entry*>::iterator iter;
          bool found = false;
          for(iter = ins_.begin(); iter != ins_.end() && !found; iter++)
            if(in == (*iter)->in_) {
              found = true;
              break;
            }
          if(found && in->claim()) {
            boost::shared_ptr<msg_type> msg;
            if (in->pull(msg) > 0) {
              //successfully get a msg here, should we use executor to run callback?
              //since invoke already run in executor, so dont delay again
              (*iter)->handler_(msg->data_);
            }
          }
	}

	bool bind(id_type & id,
                  call_back_t cb,
                  typename named_in_type::scope_type scope = named_in_type::scope_global) {
          if (ch_ == NULL) return false;
          typename synch_policy::scoped_lock lock(mutex_);
          if (ins_map_.find(id) == ins_map_.end()) {
            entry * ent = new entry();
            ent->in_ = new named_in_type(*ch_,id,
                                         boost::bind(&my_type::invoke, this, boost::ref(ent->in_), _1, _2),
                                         scope,named_in_type::member_local,exec_);
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
          ent->in_ = new named_in_type(boost::bind(&my_type::invoke, this, boost::ref(ent->in_), _1, _2),
                                       exec_);
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
            iterq = std::find(ins_.begin(), ins_.end(),ni);
            if (iterq != ins_.end()) {
              ins_.erase(iterq);
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
      class join_arbiter_async {
      public:
      public:
	typedef pull_recver_base<name_space,platform,queue_type> recver_type;
	typedef pull_sender_base<name_space,platform,queue_type> sender_type;
	typedef typename name_space::id_type id_type;
	typedef typename name_space::name name;
	typedef typename name_space::synch_policy synch_policy;
	typedef typename name_space::executor executor;
	typedef message<id_type> msg_type;
	typedef named_in<name_space, recver_type> named_in_type;
	typedef named_out<name_space, sender_type> named_out_type;
	typedef join_arbiter_async<name_space,platform,queue_type> my_type;

	typedef boost::function1<void, std::vector<boost::shared_ptr<void> >& > call_back_t;

	name_space *ch_;
	executor *exec_;
	call_back_t handler_;
	std::deque<named_in_type*> ins_;
	std::map<id_type, named_in_type*> ins_map_; //keep locking order
	typename synch_policy::mutex mutex_;

	join_arbiter_async(name_space &ch,
                           std::vector<std::pair<id_type, typename named_in_type::scope_type> > & ids,
                           call_back_t cb,
                           executor *e = NULL) :
          ch_(&ch),
          exec_(e!=NULL?e:ch.get_exec()),
          handler_(cb) {
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::vector<std::pair<id_type,
            typename named_in_type::scope_type> >::iterator iter;
          for(iter = ids.begin(); iter != ids.end(); iter++) {
            named_in_type * ni = new named_in_type(*ch_,iter->first,
                                                   boost::bind(&my_type::invoke, this, _1, _2),
                                                   iter->second,
                                                   named_in_type::member_local,exec_);
            ins_.push_back(ni); //keep order to avoid deadlock
            ins_map_[iter->first] = ni;
          }
	}

	join_arbiter_async(name_space &ch,
                           call_back_t cb,
                           executor *e = NULL) :
          ch_(&ch),
          exec_(e!=NULL?e:ch.get_exec()),
          handler_(cb) {
	}

	join_arbiter_async(call_back_t cb, executor *e = NULL) :
          ch_(NULL),
          exec_(e),
          handler_(cb) {}

	~join_arbiter_async() {
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::deque<named_in_type*>::iterator iter;
          for (iter = ins_.begin(); iter != ins_.end(); iter++)
            delete (*iter);
	}

	//called from inside dispatcher
	//2 phase protocol
	void invoke(id_type, boost::shared_ptr<void>) {
          typename synch_policy::scoped_lock lock(mutex_);
          typename std::deque<named_in_type*>::iterator iter;
          //first claim the messages
          //for (typename std::map<id_type,named_in_type*>::iterator iter1 = ins_map_.begin(); iter1 != ins_map_.end(); iter1++) {
          for (iter = ins_.begin(); iter != ins_.end(); iter++) {
            named_in_type *in = (*iter);
            if(!in->claim()) {
              //roll back
              //for (typename std::map<id_type,named_in_type*>::iterator iter2 = ins_map_.begin();
              //iter2 != iter1; iter2++)
              for (typename std::deque<named_in_type*>::iterator iter2 = ins_.begin();
                   iter2 != iter; iter2++)
                (*iter2)->unclaim();
              return;
            }
          }
          //reaching here, we have claimed all the member messages,
          //commit (pull msgs and run callback)
          boost::shared_ptr<msg_type> msg;
          std::vector<boost::shared_ptr<void> > result;
          for (iter = ins_.begin(); iter != ins_.end(); iter++) {
            named_in_type *in = (*iter);
            msg.reset();
            in->pull(msg);
            result.push_back(msg->data_);
          }
          handler_(result);
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
                                                 scope,
                                                 named_in_type::member_local,exec_);
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
          named_in_type * ni = new named_in_type(boost::bind(&my_type::invoke, this, _1, _2),
                                                 exec_);
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
            iterq = std::find(ins_.begin(), ins_.end(),ni);
            if (iterq != ins_.end()) {
              ins_.erase(iterq);
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
