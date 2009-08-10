//
// linear_name_space.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef LINEAR_NAME_SPACE_HPP
#define LINEAR_NAME_SPACE_HPP

#include <map>
#include <list>
#include <algorithm>
#include <boost/channel/name_spaces/linear_id_trait.hpp>
#include <boost/channel/platforms/synch_policy.hpp>
#include <boost/channel/name.hpp>
#include <boost/channel/name_spaces/name_space_change_notify.hpp>

namespace boost {
  namespace channel {
    /**
     *     linear_name_space: combo of simple linear space and associative space
     *     . simple linear space: name_matching = exact_match
     *     . assoc name space: name_matching = assoc/fuzzy/regex-based matching
     *     using stl map for linear name_space
     */
    template<typename idtype, typename executor_type, typename synchpolicy,
             bool exact_match = true>
    class linear_name_space {
    public:
      typedef idtype id_type;
      typedef id_trait<id_type> id_trait;
      typedef synchpolicy synch_policy;
      typedef executor_type executor;
      typedef	typename synch_policy::platform platform;
      //we dont care about In/Out operations here, just names and bindings
      typedef name<id_type,executor_type,synch_policy> name;

    private:
      typedef linear_name_space<idtype,executor_type,synchpolicy,exact_match> ns_type;

      struct ns_entry {
        std::list<name *> outs_;
        std::list<name *> ins_;
      };

      //name_space - pubsub table
      std::map<id_type, ns_entry> ns_;
      typename synch_policy::mutex ns_lock_;

      executor_type *exec_; //default executor_type for all async_opers/callbacks in channel
      //for sending out local name_space change events
      detail::name_space_change_notify<ns_type> ns_notifier_;

    public:

      linear_name_space(executor_type *e) : exec_(e), ns_notifier_(*this) {
      }

      ~linear_name_space() {
        typename synch_policy::scoped_lock lock(ns_lock_);
        //clean up name_space structures
        ns_.clear();
      }

      executor_type * get_exec(void) {
        return exec_;
      }

      /**
       * Local operations on Channel/Router name_space:
       * bind/unbind named_in/out
       * for each bind/unbind, 3 kinds of changes made:
       * 1. binding to the name_space data structure
       * 2. binding between named_out and named_in
       * 3. exported ids exposed to external channels
       */
      ///
      void bind_named_out(name *n)
      {
        typename synch_policy::scoped_lock lock(ns_lock_);

        //add new binding to name_space
        ns_entry &entry = ns_[n->id_]; //if first time, the entry will be added
        if (std::find(entry.outs_.begin(), entry.outs_.end(), n) != entry.outs_.end())
          return;

        entry.outs_.push_front(n);

        //platform::log("ns::bind_named_out  name: "+id_trait::id_to_string(n->id_));

        //bind Out->In
        //using reverse_iterator, so that later-added names will be pushed to
        //the front of bindings
        if (exact_match) {
          for(typename std::list<name *>::reverse_iterator iter=entry.ins_.rbegin();
              iter != entry.ins_.rend(); iter++) {
            name *m = (*iter);
            if (name::scope_checking(n,m)) {
              n->bind(m);
              m->bind(n);
            }
          }
        }
        else { //assoc match
          typename std::map<id_type, ns_entry>::iterator it = ns_.begin();
          for (; it != ns_.end(); it++) {
            //platform::log("ns::bind_named_in  compare: "+id_trait::id_to_string(it->first));
            if (id_trait::match(it->first, n->id_)) {
              ns_entry &ent = it->second;
              //bind Out->In
              //using reverse_iterator, so that later-added names will be pushed to
              //the front of bindings
              for(typename std::list<name *>::reverse_iterator iter=ent.ins_.rbegin();
                  iter != ent.ins_.rend(); iter++) {
                name *m = (*iter);
                if (name::scope_checking(n,m)) {
                  n->bind(m);
                  m->bind(n);
                }
              }
            }
          }
        }

        //propagate name_space changes
        lock.unlock();
        if(n->type_ == name_base::member_local)
          ns_notifier_.notify(name_base::bind_out_ev, n);
      }

      ///
      void unbind_named_out(name *n)
      {
        typename synch_policy::scoped_lock lock(ns_lock_);

        //del binding from name_space
        ns_entry &entry = ns_[n->id_];
        entry.outs_.remove(n);
        //clean up
        if (entry.outs_.empty() && entry.ins_.empty())
          ns_.erase(n->id_);

        //unbind all Out->In
        //n->unbind_all();

        //propagate name_space changes
        lock.unlock();
        if(n->type_ == name_base::member_local)
          ns_notifier_.notify(name_base::unbind_out_ev, n);
      }

      ///
      void bind_named_in(name *n)
      {
        typename synch_policy::scoped_lock lock(ns_lock_);

        //add new binding to name_space
        ns_entry &entry = ns_[n->id_]; //if first time, the entry will be added
        if (std::find(entry.ins_.begin(), entry.ins_.end(), n) != entry.ins_.end())
          return;

        entry.ins_.push_front(n);

        //platform::log("ns::bind_named_in  name: "+id_trait::id_to_string(n->id_));

        //bind Out->In
        //using reverse_iterator, so that later-added names will be pushed to
        //the front of bindings
        if (exact_match) {
          for(typename std::list<name *>::reverse_iterator iter=entry.outs_.rbegin();
              iter != entry.outs_.rend(); iter++) {
            name *m = (*iter);
            if (name::scope_checking(m,n)) {
              m->bind(n);
              n->bind(m);
            }
          }
        }
        else {
          typename std::map<id_type, ns_entry>::iterator it = ns_.begin();
          for (; it != ns_.end(); it++) {
            //platform::log("ns::bind_named_in  compare: "+id_trait::id_to_string(it->first));
            if (id_trait::match(it->first, n->id_)) {
              ns_entry &ent = it->second;
              //bind Out->In
              //using reverse_iterator, so that later-added names will be pushed to
              //the front of bindings
              for(typename std::list<name *>::reverse_iterator iter=ent.outs_.rbegin();
                  iter != ent.outs_.rend(); iter++) {
                name *m = (*iter);
                if (name::scope_checking(m,n)) {
                  m->bind(n);
                  n->bind(m);
                }
              }
            }
          }
        }

        //propagate name_space changes
        lock.unlock();
        if(n->type_ == name_base::member_local)
          ns_notifier_.notify(name_base::bind_in_ev, n);
      }

      ///
      void unbind_named_in(name *n)
      {
        typename synch_policy::scoped_lock lock(ns_lock_);

        //del binding from name_space
        ns_entry &entry = ns_[n->id_];
        entry.ins_.remove(n);
        //clean up
        if (entry.outs_.empty() && entry.ins_.empty())
          ns_.erase(n->id_);

        //unbind all Out->In
        //n->unbind_all();

        //propagate name_space changes
        lock.unlock();
        if(n->type_ == name_base::member_local)
          ns_notifier_.notify(name_base::unbind_in_ev, n);
      }

      /**
       * 3. name_space queries
       */
      template <typename Predicate>
      void bound_ids_for_in(Predicate p, std::vector<id_type> &ids)
      {
        typename synch_policy::scoped_lock lock(ns_lock_);
        for(typename std::map<id_type, ns_entry>::iterator iter = ns_.begin();
            iter != ns_.end(); iter++) {
          ns_entry &entry = iter->second;
          if (entry.ins_.empty())
            continue;

          bool found = false;
          for(typename std::list<name*>::iterator iter2 = entry.ins_.begin();
              iter2 != entry.ins_.end() && !found; iter2++) {
            name *n = (*iter2);
            if (p(n)) {
              found = true;
            }
          }
          if (found)
            ids.push_back(iter->first);
        }
      }

      template <typename Predicate>
      void bound_ids_for_out(Predicate p, std::vector<id_type> &ids)
      {
        typename synch_policy::scoped_lock lock(ns_lock_);
        for(typename std::map<id_type, ns_entry>::iterator iter = ns_.begin();
            iter != ns_.end(); iter++) {
          ns_entry &entry = iter->second;
          if (entry.outs_.empty())
            continue;

          bool found = false;
          for(typename std::list<name*>::iterator iter2 = entry.outs_.begin();
              iter2 != entry.outs_.end() && !found; iter2++) {
            name *n = (*iter2);
            if (p(n)) {
              found = true;
            }
          }
          if (found)
            ids.push_back(iter->first);
        }
      }

    };
  }
}

#endif
