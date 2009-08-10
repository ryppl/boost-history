//
// hierarchical_name_space.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HIERARCHICAL_NAME_SPACE_HPP
#define HIERARCHICAL_NAME_SPACE_HPP

#include <map>
#include <list>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/channel/name_spaces/hierarchical_id_trait.hpp>
#include <boost/channel/platforms/synch_policy.hpp>
#include <boost/channel/name.hpp>
#include <boost/channel/name_spaces/name_space_change_notify.hpp>

namespace boost {
  namespace channel {
    /**
     *     hierarchical_name_space:
     *     using trie as routing data structure
     *     using pathname matching
     */
    template<class idtype, class executor_type, class synchpolicy>
    class hierarchical_name_space {
    public:
      typedef idtype id_type;
      typedef id_trait<id_type> id_trait;
      typedef synchpolicy synch_policy;
      typedef executor_type executor;
      typedef	typename id_trait::token_type token_type;
      typedef typename synch_policy::platform platform;
      //we dont care about In/Out operations here, just names and bindings
      typedef name<id_type,executor_type,synch_policy> name;

    private:
      typedef hierarchical_name_space<idtype,executor_type,synchpolicy> ns_type;

      struct trie_node {
        token_type token; //name of this node
        /// -- trie data structure --
        trie_node *parent;
        std::map<token_type, trie_node *> children;
        typename synch_policy::mutex trie_lock_; //to protect trie structure
        /// -- bindings --
        // normal name binding
        std::list<name *> outs_;
        std::list<name *> ins_;
        // wildcard name binding
        std::list<name *> outs_w_;
        std::list<name *> ins_w_;
        typename synch_policy::mutex bind_lock_; //to protect binding structure
        trie_node(token_type e, trie_node *p) : token(e), parent(p) {
        }
        ~trie_node() {
          typename synch_policy::scoped_lock lock(trie_lock_);
          for(typename std::map<token_type, trie_node *>::iterator iter = children.begin();
              iter != children.end(); iter++) {
            if (iter->second != NULL)
              delete iter->second;
          }
        }
      };

      //name_space
      boost::shared_ptr<trie_node> ns_root_; //root of name_space tree
      //no need for a global name_space lock, since each node has its own lock
      //may be too much protection, in future we can swtch back to a global lock

      executor_type *exec_; //default executor_type for all async_opers/callbacks in channel
      //for sending out local name_space change events
      detail::name_space_change_notify<ns_type> ns_notifier_;

    public:

      hierarchical_name_space(executor_type *e) :
	ns_root_(new trie_node(id_trait::root_token, NULL)),
	exec_(e),
	ns_notifier_(*this) {
      }

      ~hierarchical_name_space() {
        //shared_ptr make sure delete notifier (so its named_in/outs are deleted)
        //before deleting name space
      }

      executor_type * get_exec(void) {
        return exec_;
      }

      /**
       * Local operations on name_space:
       * bind/unbind named_in/out
       * for each bind/unbind, 3 kinds of changes made:
       * 1. binding to the trie data structure
       * 2. binding between named_out and named_in
       * 3. exported ids/names exposed to external channels
       *
       * specialty with hierarchical name_space because of wildcard in names/ids:
       *    1> bindings (out<->in):
       *                 . bind with all "opposite" names/wildcards in trie at the node with this id
       *                 . bind with all upper level "opposite" wildcards, no matter this is wildcard or not
       *                 . if this is wildcard, bind with all lower level "opposite" names
       *    2> external ids (exposed): wildcard names do hide/unhide :
       *                 . a wildcard name will hide/unhide all "same-side" names in trie at lower levels
       *                 . any names can be hiden by a "same-side" wildcard at upper level
       *
       * normal algorithms:
       * 1. traverse the trie to the destination node:
       *    . if some path or node do not exist, create them
       *    . check if it is hidden by a upper level wildcard at the same side
       *    . check if it can bind to upper level wildcards at opposite, add bindings
       * 2. at the destination node:
       *    . check if it can bind to opposite names and wildcards at this node, add bindings
       *    . if it is not a wildcard, and not hidden, simply check if it should be "notify"ed
       *    . if this is a wildcard:
       *      . check if it can bind to all lower level "opposite" names, add bindings
       *      . check if it is not hidden, it hide/unhide all "same-side" names at lower level;
       *              call notify_external to hide hidden names.
       *      . if is not hidden call notify on it; if hidden, call notify_internal
       */
      ///
      void bind_named_out(name *n)
      {
        bool wildcard = id_trait::wildcard_name(n->id_);
        bool hidden = false;

        /**
         * traverse from root to node designated by n->id_
         */
        trie_node *node = ns_root_.get();
        for (size_t i=0; i<n->id_.size(); i++) {
          if (n->id_[i] == id_trait::wildcard_token && i == (n->id_.size()-1))
            break;
          ///check if it is hidden by a upper level wildcard at the same side
          if (!hidden && name::exported_name(n)) {
            typename synch_policy::scoped_lock lock(node->bind_lock_);
            if (!node->outs_w_.empty()) {
              for(typename std::list<name *>::iterator iter = node->outs_w_.begin();
                  iter != node->outs_w_.end() && !hidden; iter++) {
                name *m = (*iter);
                if (name::exported_name(m))
                  hidden = true;
              }
            }
          }
          ///check if it can bind to upper level wildcards at opposite, add bindings
          {
            typename synch_policy::scoped_lock lock(node->bind_lock_);
            if (!node->ins_w_.empty()) {
              for(typename std::list<name *>::iterator iter = node->ins_w_.begin();
                  iter != node->ins_w_.end(); iter++) {
                name *m = (*iter);
                ///setup bindings
                if (name::scope_checking(n,m)) {
                  n->bind(m);
                  m->bind(n);
                  platform::log("bind_named_out/bind : "+id_trait::id_to_string(n->id_)+"->"+id_trait::id_to_string(m->id_));
                }
              }
            }
          }
          ///if some path or node do not exist, create them
          {
            typename synch_policy::scoped_lock lock(node->trie_lock_);
            typename std::map<token_type, trie_node *>::iterator iter = node->children.find(n->id_[i]);
            if (iter == node->children.end()) {
              trie_node *cnd = new trie_node (n->id_[i], node);
              node->children[n->id_[i]] = cnd;
              node = cnd;
            }
            else
              node = iter->second;
          }
        }

        /// now we have reached the node for this n->id_ (without trailing wildcard)
        //
        ///check if it can bind to opposite names , add bindings
        ///binding to opposite wildcard at this node already handled during traverse
        {
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          if (!node->ins_.empty()) {
            for(typename std::list<name *>::iterator iter = node->ins_.begin();
                iter != node->ins_.end(); iter++) {
              name *m = (*iter);
              ///setup bindings
              if (name::scope_checking(n,m)) {
                n->bind(m);
                m->bind(n);
                platform::log("bind_named_out/bind : "+id_trait::id_to_string(n->id_)+"->"+
                              id_trait::id_to_string(m->id_));
              }
            }
          }

          if (!node->ins_w_.empty()) {
            for(typename std::list<name *>::iterator iter = node->ins_w_.begin();
                iter != node->ins_w_.end(); iter++) {
              name *m = (*iter);
              ///setup bindings
              if (name::scope_checking(n,m)) {
                n->bind(m);
                m->bind(n);
                platform::log("bind_named_out/bind : "+id_trait::id_to_string(n->id_)+"->"+
                              id_trait::id_to_string(m->id_));
              }
            }
          }
        }

        if (!wildcard) {
          /// for normal names
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          ///store in name_space
          node->outs_.push_front(n);
          lock.unlock();

          ///notify
          if(name::exported_name(n) && hidden)
            ///if hidden, just notify internal peers
            ns_notifier_.notify_internal(name::bind_out_ev, n);
          else if(n->type_ == name::member_local)
            ///notify all
            ns_notifier_.notify(name::bind_out_ev, n);
        }
        else {
          ///for wildcards
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          ///store in name_space
          node->outs_w_.push_front(n);
          lock.unlock();

          /// bind to lower level opposite names
          {
            typename synch_policy::scoped_lock lock(node->trie_lock_);
            for(typename std::map<token_type, trie_node *>::iterator iter = node->children.begin();
                iter != node->children.end(); iter++)
              bind_out_to_lower_ins(n, iter->second);
          }

          /// notify, and possibly hide lower level names
          if (name::exported_name(n)) {
            if(hidden)
              ns_notifier_.notify_internal(name::bind_out_ev, n);
            else {
              ///at external interface, hide all "same-side" names at lower level,
              ///starting from this node
              update_children_out_exports (node, name::unbind_out_ev, false);
              ns_notifier_.notify(name::bind_out_ev, n);
            }
          }
          else if(n->type_ == name::member_local) {
            ns_notifier_.notify(name::bind_out_ev, n);
          }
        }
      }

      ///
      void unbind_named_out(name *n)
      {
        bool wildcard = id_trait::wildcard_name(n->id_);
        bool hidden = false;

        /**
         * traverse from root to node designated by n->id_
         */
        trie_node *node = ns_root_.get();
        for (size_t i=0; i<n->id_.size(); i++) {
          if (n->id_[i] == id_trait::wildcard_token && i == (n->id_.size()-1))
            break;
          ///check if it is hidden by a upper level wildcard at the same side
          if (!hidden && name::exported_name(n)) {
            typename synch_policy::scoped_lock lock(node->bind_lock_);
            if (!node->outs_w_.empty()) {
              for(typename std::list<name *>::iterator iter = node->outs_w_.begin();
                  iter != node->outs_w_.end() && !hidden; iter++) {
                name *m = (*iter);
                if (name::exported_name(m) && n != m)
                  hidden = true;
              }
            }
          }
          {
            typename synch_policy::scoped_lock lock(node->trie_lock_);
            typename std::map<token_type, trie_node *>::iterator iter = node->children.find(n->id_[i]);
            if (iter == node->children.end()) {
              platform::log("unbind_named_in: branch disappeared");
              return;
            }
            node = iter->second;
          }
        }

        /// now we have reached the node for this n->id_ (without trailing wildcard)

        ///unbind all Out->In
        //n->unbind_all();

        ///del binding from name_space
        if (!wildcard) {
          /// for normal names
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          ///store in name_space
          node->outs_.remove(n);
          lock.unlock();

          ///notify
          if(name::exported_name(n) && hidden)
            ///if hidden, just notify internal peers
            ns_notifier_.notify_internal(name::unbind_out_ev, n);
          else if(n->type_ == name::member_local)
            ///notify all
            ns_notifier_.notify(name::unbind_out_ev, n);
        }
        else {
          ///for wildcards
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          ///store in name_space
          node->outs_w_.remove(n);
          lock.unlock();

          /// notify, and possibly hide lower level names
          if (name::exported_name(n)) {
            if(hidden)
              ns_notifier_.notify_internal(name::unbind_out_ev, n);
            else {
              ///at external interface, unhide all "same-side" names at lower level,
              ///starting from this node
              update_children_in_exports (node, name::bind_out_ev, false);
              ns_notifier_.notify(name::unbind_out_ev, n);
            }
          }
          else if(n->type_ == name::member_local) {
            ns_notifier_.notify(name::unbind_out_ev, n);
          }
        }
      }

      ///
      void bind_named_in(name *n)
      {
        bool wildcard = id_trait::wildcard_name(n->id_);
        bool hidden = false;

        /**
         * traverse from root to node designated by n->id_
         */
        trie_node *node = ns_root_.get();
        for (size_t i=0; i<n->id_.size(); i++) {
          if (n->id_[i] == id_trait::wildcard_token && i == (n->id_.size()-1))
            break;
          ///check if it is hidden by a upper level wildcard at the same side
          if (!hidden && name::exported_name(n)) {
            typename synch_policy::scoped_lock lock(node->bind_lock_);
            if (!node->ins_w_.empty()) {
              for(typename std::list<name *>::iterator iter = node->ins_w_.begin();
                  iter != node->ins_w_.end() && !hidden; iter++) {
                name *m = (*iter);
                if (name::exported_name(m))
                  hidden = true;
              }
            }
          }
          ///check if it can bind to upper level wildcards at opposite, add bindings
          {
            typename synch_policy::scoped_lock lock(node->bind_lock_);
            if (!node->outs_w_.empty()) {
              for(typename std::list<name *>::iterator iter = node->outs_w_.begin();
                  iter != node->outs_w_.end(); iter++) {
                name *m = (*iter);
                ///setup bindings
                if (name::scope_checking(m,n)) {
                  m->bind(n);
                  n->bind(m);
                  platform::log("bind_named_in/bind : "+id_trait::id_to_string(n->id_)+"->"+
                                id_trait::id_to_string(m->id_));
                }
              }
            }
          }
          ///if some path or node do not exist, create them
          {
            typename synch_policy::scoped_lock lock(node->trie_lock_);
            typename std::map<token_type, trie_node *>::iterator iter = node->children.find(n->id_[i]);
            if (iter == node->children.end()) {
              trie_node *cnd = new trie_node (n->id_[i], node);
              node->children[n->id_[i]] = cnd;
              node = cnd;
            }
            else
              node = iter->second;
          }
        }

        /// now we have reached the node for this n->id_ (without trailing wildcard)
        //
        ///check if it can bind to opposite names , add bindings
        ///binding to opposite wildcards at this node already done during traverse
        {
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          if (!node->outs_.empty()) {
            for(typename std::list<name *>::iterator iter = node->outs_.begin();
                iter != node->outs_.end(); iter++) {
              name *m = (*iter);
              ///setup bindings
              if (name::scope_checking(m,n)) {
                m->bind(n);
                n->bind(m);
                platform::log("bind_named_in/bind : "+id_trait::id_to_string(n->id_)+"->"+
                              id_trait::id_to_string(m->id_));
              }
            }
          }
          if (!node->outs_w_.empty()) { //if this is wildcard, this step alrady done during traverse
            for(typename std::list<name *>::iterator iter = node->outs_w_.begin();
                iter != node->outs_w_.end(); iter++) {
              name *m = (*iter);
              ///setup bindings
              if (name::scope_checking(m,n)) {
                m->bind(n);
                n->bind(m);
                platform::log("bind_named_in/bind : "+id_trait::id_to_string(n->id_)+"->"+
                              id_trait::id_to_string(m->id_));
              }
            }
          }
        }

        if (!wildcard) {
          /// for normal names
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          ///store in name_space
          node->ins_.push_front(n);
          lock.unlock();

          ///notify
          if(name::exported_name(n) && hidden)
            ///if hidden, just notify internal peers
            ns_notifier_.notify_internal(name::bind_in_ev, n);
          else if(n->type_ == name::member_local)
            ///notify all
            ns_notifier_.notify(name::bind_in_ev, n);
        }
        else {
          ///for wildcards
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          ///store in name_space
          node->ins_w_.push_front(n);
          lock.unlock();

          /// bind to lower level opposite names
          {
            typename synch_policy::scoped_lock lock(node->trie_lock_);
            for(typename std::map<token_type, trie_node *>::iterator iter = node->children.begin();
                iter != node->children.end(); iter++)
              bind_in_to_lower_outs(n, iter->second);
          }

          /// notify, and possibly hide lower level names
          if (name::exported_name(n)) {
            if(hidden)
              ns_notifier_.notify_internal(name::bind_in_ev, n);
            else {
              ///at external interface, hide all "same-side" names at lower level,
              ///starting from this node
              update_children_in_exports (node, name::unbind_in_ev, false);
              ns_notifier_.notify(name::bind_in_ev, n);
            }
          }
          else if(n->type_ == name::member_local) {
            ns_notifier_.notify(name::bind_in_ev, n);
          }
        }
      }

      ///
      void unbind_named_in(name *n)
      {
        bool wildcard = id_trait::wildcard_name(n->id_);
        bool hidden = false;

        /**
         * traverse from root to node designated by n->id_
         */
        trie_node *node = ns_root_.get();
        for (size_t i=0; i<n->id_.size(); i++) {
          if (n->id_[i] == id_trait::wildcard_token && i == (n->id_.size()-1))
            break;
          ///check if it is hidden by a upper level wildcard at the same side
          if (!hidden && name::exported_name(n)) {
            typename synch_policy::scoped_lock lock(node->bind_lock_);
            if (!node->ins_w_.empty()) {
              for(typename std::list<name *>::iterator iter = node->ins_w_.begin();
                  iter != node->ins_w_.end() && !hidden; iter++) {
                name *m = (*iter);
                if (name::exported_name(m) && n != m)
                  hidden = true;
              }
            }
          }
          {
            typename synch_policy::scoped_lock lock(node->trie_lock_);
            typename std::map<token_type, trie_node *>::iterator iter = node->children.find(n->id_[i]);
            if (iter == node->children.end()) {
              platform::log("unbind_named_in: branch disappeared");
              return;
            }
            node = iter->second;
          }
        }

        /// now we have reached the node for this n->id_ (without trailing wildcard)

        ///unbind all Out->In
        //n->unbind_all();

        ///del binding from name_space
        if (!wildcard) {
          /// for normal names
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          ///store in name_space
          node->ins_.remove(n);
          lock.unlock();

          ///notify
          if(name::exported_name(n) && hidden)
            ///if hidden, just notify internal peers
            ns_notifier_.notify_internal(name::unbind_in_ev, n);
          else if(n->type_ == name::member_local)
            ///notify all
            ns_notifier_.notify(name::unbind_in_ev, n);
        }
        else {
          ///for wildcards
          typename synch_policy::scoped_lock lock(node->bind_lock_);
          ///store in name_space
          node->ins_w_.remove(n);
          lock.unlock();

          /// notify, and possibly hide lower level names
          if (name::exported_name(n)) {
            if(hidden)
              ns_notifier_.notify_internal(name::unbind_in_ev, n);
            else {
              ///at external interface, unhide all "same-side" names at lower level,
              ///starting from this node
              id_type id = n->id_;
              update_children_in_exports (node, name::bind_in_ev, false);
              ns_notifier_.notify(name::unbind_in_ev, n);
            }
          }
          else if(n->type_ == name::member_local) {
            ns_notifier_.notify(name::unbind_in_ev, n);
          }
        }
      }

      void bind_in_to_lower_outs(name *n, trie_node *node) {
        typename synch_policy::scoped_lock lock(node->bind_lock_);
        if (!node->outs_.empty()) {
          for(typename std::list<name *>::iterator iter = node->outs_.begin();
              iter != node->outs_.end(); iter++) {
            name *m = (*iter);
            ///setup bindings
            if (name::scope_checking(m,n)) {
              m->bind(n);
              n->bind(m);
              platform::log("bind_in_to_lower_outs/bind : "+id_trait::id_to_string(n->id_)+"->"+
                            id_trait::id_to_string(m->id_));
            }
          }
        }
        if (!node->outs_w_.empty()) {
          for(typename std::list<name *>::iterator iter = node->outs_w_.begin();
              iter != node->outs_w_.end(); iter++) {
            name *m = (*iter);
            ///setup bindings
            if (name::scope_checking(m,n)) {
              m->bind(n);
              n->bind(m);
              platform::log("bind_in_to_lower_outs/bind : "+id_trait::id_to_string(n->id_)+"->"+
                            id_trait::id_to_string(m->id_));
            }
          }
        }
        lock.unlock();
        //recursively update children
        typename synch_policy::scoped_lock lock2(node->trie_lock_);
        for(typename std::map<token_type, trie_node*>::iterator iter = node->children.begin();
            iter != node->children.end(); iter++) {
          bind_in_to_lower_outs (n, iter->second);
        }
      }

      void bind_out_to_lower_ins(name *m, trie_node *node) {
        typename synch_policy::scoped_lock lock(node->bind_lock_);
        if (!node->ins_.empty()) {
          for(typename std::list<name *>::iterator iter = node->ins_.begin();
              iter != node->ins_.end(); iter++) {
            name *n = (*iter);
            ///setup bindings
            if (name::scope_checking(m,n)) {
              m->bind(n);
              n->bind(m);
              platform::log("bind_out_to_lower_ins/bind : "+id_trait::id_to_string(n->id_)+"->"+
                            id_trait::id_to_string(m->id_));
            }
          }
        }
        if (!node->ins_w_.empty()) {
          for(typename std::list<name *>::iterator iter = node->ins_w_.begin();
              iter != node->ins_w_.end(); iter++) {
            name *n = (*iter);
            ///setup bindings
            if (name::scope_checking(m,n)) {
              m->bind(n);
              n->bind(m);
              platform::log("bind_out_to_lower_ins/bind : "+id_trait::id_to_string(n->id_)+"->"+
                            id_trait::id_to_string(m->id_));
            }
          }
        }
        lock.unlock();
        //recursively update children
        typename synch_policy::scoped_lock lock2(node->trie_lock_);
        for(typename std::map<token_type, trie_node*>::iterator iter = node->children.begin();
            iter != node->children.end(); iter++) {
          bind_out_to_lower_ins (m, iter->second);
        }
      }

      void update_children_out_exports (trie_node *node,
                                        typename name::binding_event oper, bool update_wildcard) {
        bool send2remote = false;

        typename synch_policy::scoped_lock lock1(node->bind_lock_);
        ///if encounter wildcard, stop going deeper
        if (update_wildcard && !node->outs_w_.empty()) {
          name *m;
          for(typename std::list<name *>::iterator iter = node->outs_w_.begin();
              iter != node->outs_w_.end(); iter++) {
            m = *iter;
            if (name::exported_name(m)) {
              send2remote = true;
              if (oper == name::unbind_out_ev)
                m->unbind_all_external();
            }
          }
          if (send2remote) {
            ns_notifier_.notify_external(oper, m);
            return;
          }
        }
        if (!node->outs_.empty()) {
          name *m;
          for(typename std::list<name *>::iterator iter = node->outs_.begin();
              iter != node->outs_.end(); iter++) {
            m = *iter;
            if (name::exported_name(m)) {
              send2remote = true;
              if (oper == name::unbind_out_ev)
                m->unbind_all_external();
            }
          }
          if (send2remote) {
            ns_notifier_.notify_external(oper, m);
          }
        }
        lock1.unlock();

        //recursively update children
        typename synch_policy::scoped_lock lock2(node->trie_lock_);
        for(typename std::map<token_type, trie_node*>::iterator iter = node->children.begin();
            iter != node->children.end(); iter++) {
          update_children_out_exports (iter->second, oper, true);
        }
      }

      void update_children_in_exports (trie_node *node,
                                       typename name::binding_event oper, bool update_wildcard) {
        bool send2remote = false;

        typename synch_policy::scoped_lock lock1(node->bind_lock_);
        ///if encounter wildcard, stop going deeper
        if (update_wildcard && !node->ins_w_.empty()) {
          name *m;
          for(typename std::list<name *>::iterator iter = node->ins_w_.begin();
              iter != node->ins_w_.end(); iter++) {
            m = *iter;
            if (name::exported_name(m)) {
              send2remote = true;
              if (oper == name::unbind_in_ev)
                m->unbind_all_external();
            }
          }
          if (send2remote) {
            ns_notifier_.notify_external(oper, m);
            return;
          }
        }
        if (!node->ins_.empty()) {
          name *m;
          for(typename std::list<name *>::iterator iter = node->ins_.begin();
              iter != node->ins_.end(); iter++) {
            m = *iter;
            if (name::exported_name(m)) {
              send2remote = true;
              if (oper == name::unbind_in_ev)
                m->unbind_all_external();
            }
          }
          if (send2remote) {
            ns_notifier_.notify_external(oper, m);
          }
        }
        lock1.unlock();

        //recursively update children
        typename synch_policy::scoped_lock lock2(node->trie_lock_);
        for(typename std::map<token_type, trie_node*>::iterator iter = node->children.begin();
            iter != node->children.end(); iter++) {
          update_children_in_exports (iter->second, oper, true);
        }
      }

      /**
       * 3. name_space queries
       */

      template <typename Predicate>
      void bound_ids_for_in(Predicate p, std::vector<id_type> &ids)
      {
        bound_ids_for_in_recursive(p, ids, ns_root_.get());
      }

      template <typename Predicate>
      void bound_ids_for_in_recursive(Predicate p, std::vector<id_type> &ids, trie_node *node)
      {
        name *m;
        typename synch_policy::scoped_lock lock1(node->bind_lock_);
        ///if encounter wildcard, stop going deeper
        if (!node->ins_w_.empty()) {
          for(typename std::list<name *>::iterator iter = node->ins_w_.begin();
              iter != node->ins_w_.end(); iter++) {
            m = *iter;
            if (p(m)) {
              ids.push_back(m->id_);
              return;
            }
          }
        }
        if (!node->ins_.empty()) {
          for(typename std::list<name *>::iterator iter = node->ins_.begin();
              iter != node->ins_.end(); iter++) {
            m = *iter;
            if (p(m)) {
              ids.push_back(m->id_);
              break;
            }
          }
        }
        lock1.unlock();

        //recursively update children
        typename synch_policy::scoped_lock lock2(node->trie_lock_);
        for(typename std::map<token_type, trie_node*>::iterator iter = node->children.begin();
            iter != node->children.end(); iter++) {
          bound_ids_for_in_recursive(p, ids, iter->second);
        }
      }

      template <typename Predicate>
      void bound_ids_for_out(Predicate p, std::vector<id_type> &ids)
      {
        bound_ids_for_out_recursive(p, ids, ns_root_.get());
      }

      template <typename Predicate>
      void bound_ids_for_out_recursive(Predicate p, std::vector<id_type> &ids, trie_node *node)
      {
        name *m;
        typename synch_policy::scoped_lock lock1(node->bind_lock_);
        ///if encounter wildcard, stop going deeper
        if (!node->outs_w_.empty()) {
          for(typename std::list<name *>::iterator iter = node->outs_w_.begin();
              iter != node->outs_w_.end(); iter++) {
            m = *iter;
            if (p(m)) {
              ids.push_back(m->id_);
              return;
            }
          }
        }
        if (!node->outs_.empty()) {
          for(typename std::list<name *>::iterator iter = node->outs_.begin();
              iter != node->outs_.end(); iter++) {
            m = *iter;
            if (p(m)) {
              ids.push_back(m->id_);
              break;
            }
          }
        }
        lock1.unlock();

        //recursively update children
        typename synch_policy::scoped_lock lock2(node->trie_lock_);
        for(typename std::map<token_type, trie_node*>::iterator iter = node->children.begin();
            iter != node->children.end(); iter++) {
          bound_ids_for_out_recursive(p, ids, iter->second);
        }
      }
    };
  }
}

#endif

