//
// named_in_out.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NAMED_IN_OUT_HPP
#define NAMED_IN_OUT_HPP

#include <vector>
#include <map>
#include <iostream>

namespace boost {
  namespace channel {
    /**
     * named_out/named_in provides the basic functionality of out/in
     * for use inside name_space, always use broadcast_dispatcher
     */
    template<typename name_space, typename dispatcher_sender_type>
    class named_out: public name_space::name, public dispatcher_sender_type {
    public:
      typedef typename name_space::executor executor_type;
      typedef typename name_space::id_type id_type;
      typedef typename name_space::name name_type;
      name_space * ch_;
      executor_type *exe_;
      named_out (name_space & ch,
                 id_type id,
                 typename name_type::scope_type scope = name_type::scope_global,
                 typename name_type::member_type type = name_type::member_local,
                 executor_type *e = NULL
                 ) :
        name_space::name (id,scope,type),
        dispatcher_sender_type(this,e!=NULL?e:ch.get_exec()),
        ch_(&ch), exe_(e!=NULL?e:ch.get_exec()) {
        ch_->bind_named_out(this);
      }
      template <typename binding_cb_type>
      named_out (name_space & ch,
                 id_type id,
                 binding_cb_type cb,
                 typename name_type::scope_type scope = name_type::scope_global,
                 typename name_type::member_type type = name_type::member_local,
                 executor_type *e = NULL
                 ) :
        name_space::name (id, scope, type, cb, e!=NULL?e:ch.get_exec()),
        dispatcher_sender_type(this,e!=NULL?e:ch.get_exec()),
        ch_(&ch), exe_(e!=NULL?e:ch.get_exec()) {
        ch_->bind_named_out(this);
      }
      //unnamed named_out
      named_out (executor_type *e = NULL) :
        name_space::name (),
        dispatcher_sender_type(this,e),
        ch_(NULL), exe_(e) {
      }
      ~named_out () {
        if (ch_ != NULL)
          ch_->unbind_named_out(this);
      }

      //attach unnamed named_out to name space
      void attach_name_space (name_space & ch,
                              id_type id,
                              typename name_type::scope_type scope = name_type::scope_global,
                              typename name_type::member_type type = name_type::member_local) {
        this->set_name (id,scope,type);
        ch_ = &ch;
        ch_->bind_named_out(this);
      }
      template <typename binding_cb_type>
      void attach_name_space (name_space & ch,
                              id_type id,
                              binding_cb_type cb,
                              typename name_type::scope_type scope = name_type::scope_global,
                              typename name_type::member_type type = name_type::member_local) {
        this->set_name (id, scope, type, cb, exe_!=NULL?exe_:ch.get_exec()),
          ch_ = &ch;
        ch_->bind_named_out(this);
      }
    };

    template <
      typename name_space,
      typename dispatcher_recver_type
      >
    class named_in : public name_space::name,
                     public dispatcher_recver_type {
    public:
      typedef typename name_space::executor executor_type;
      typedef typename name_space::id_type id_type;
      typedef typename name_space::name name_type;
      name_space * ch_;
      executor_type *exe_;
      //asynch recvers constructor
      template <typename recver_type,typename binding_cb_type>
      named_in (name_space & ch,
                id_type id,
                recver_type rc,
                binding_cb_type bc,
                typename name_type::scope_type scope = name_type::scope_global,
                typename name_type::member_type type = name_type::member_local,
                executor_type *e = NULL
                ) :
        name_space::name (id,scope,type,bc, e!=NULL?e:ch.get_exec()),
        dispatcher_recver_type(this, rc, e!=NULL?e:ch.get_exec()), ch_(&ch),
        exe_(e!=NULL?e:ch.get_exec()) {
        ch_->bind_named_in(this);
      }
      //asynch recvers constructor
      template <typename recver_type>
      named_in (name_space & ch,
                id_type id,
                recver_type rc,
                typename name_type::scope_type scope = name_type::scope_global,
                typename name_type::member_type type = name_type::member_local,
                executor_type *e = NULL
                ) :
        name_space::name (id,scope,type),
        dispatcher_recver_type(this, rc, e!=NULL?e:ch.get_exec()), ch_(&ch),
        exe_(e!=NULL?e:ch.get_exec()) {
        ch_->bind_named_in(this);
      }
      //synch/active recvers constructor
      named_in (name_space & ch,
                id_type id,
                typename name_type::scope_type scope = name_type::scope_global,
                typename name_type::member_type type = name_type::member_local,
                executor_type *e = NULL
                ) :
        name_space::name (id,scope,type),
        dispatcher_recver_type(this), ch_(&ch), exe_(e!=NULL?e:ch.get_exec()) {
        ch_->bind_named_in(this);
      }
      //unnamed named_in
      template <typename recver_type>
      named_in (recver_type rc,
                executor_type *e = NULL
                ) :
        name_space::name (),
        dispatcher_recver_type(this, rc, e),
        ch_(NULL), exe_(e) {
      }
      ~named_in () {
        if (ch_ != NULL)
          ch_->unbind_named_in(this);
      }
      //
      template <typename binding_cb_type>
      void attach_name_space (name_space & ch,
                              id_type id,
                              binding_cb_type bc,
                              typename name_type::scope_type scope = name_type::scope_global,
                              typename name_type::member_type type = name_type::member_local
                              ) {
        if (exe_ == NULL) exe_ = ch.get_exec();
        this->set_name (id,scope,type,bc, exe_),
          ch_ = &ch;
        ch_->bind_named_in(this);
        (dispatcher_recver_type*) this->set_exe(exe_);
      }
      //
      void attach_name_space (name_space & ch,
                              id_type id,
                              typename name_type::scope_type scope = name_type::scope_global,
                              typename name_type::member_type type = name_type::member_local
                              ) {
        if (exe_ == NULL) exe_ = ch.get_exec();
        this->set_name (id,scope,type);
        ch_ = &ch;
        ch_->bind_named_in(this);
        (dispatcher_recver_type*) this->set_exe(exe_);
      }
    };

    /**
     * named_out_bundle/named_in_bundle or named_pub/named_sub
     * no binding callbacks installed
     */
    template <
      typename name_space,
      typename dispatcher_sender_type
      >
    class named_out_bundle {
    public:
      typedef typename name_space::id_type id_type;
      typedef typename name_space::id_trait id_trait;
      typedef typename name_space::executor executor_type;
      typedef typename name_space::synch_policy synch_policy;
      typedef named_out<name_space,dispatcher_sender_type> named_out;
    public:
      named_out_bundle(name_space & ch,
                       typename named_out::member_type type = named_out::member_local,
                       executor_type *e = NULL) :
        ch_(ch), type_(type), exec_(e!=NULL?e:ch.get_exec()) {
      }
      ~named_out_bundle() {
        for(typename std::map<id_type, named_out*>::iterator iter=named_outs_.begin();
            iter != named_outs_.end(); iter++)
          delete iter->second;
      }
      int get_ids(std::vector<id_type> &ids) {
        for(typename std::map<id_type, named_out*>::iterator iter=named_outs_.begin();
            iter != named_outs_.end(); iter++)
          ids.push_back(iter->first);
        return ids.size();
      }
      bool bound(id_type & id) {
        typename std::map<id_type, named_out*>::iterator iter = named_outs_.find(id);
        if (iter != named_outs_.end()) {
          return iter->second->num_bindings() > 0;
        }
        return false;
      }
      named_out * find(id_type & id) {
        typename std::map<id_type, named_out*>::iterator iter = named_outs_.find(id);
        if (iter != named_outs_.end()) {
          return iter->second;
        }
        return NULL;
      }
      named_out * find_match(id_type & id) {
        for(typename std::map<id_type, named_out*>::iterator iter = named_outs_.begin();
            iter != named_outs_.end(); iter++)
          if(id_trait::match(iter->first, id)) {
            return iter->second;
          }
        return NULL;
      }
      void bind(std::vector<std::pair<id_type,typename named_out::scope_type> > & ids) {
        for(typename std::vector<id_type>::iterator iter=ids.begin();iter != ids.end(); iter++) {
          bind(iter->first, iter->second);
        }
      }
      bool bind(id_type & id, typename named_out::scope_type scope = named_out::scope_global) {
        if (named_outs_.find(id) == named_outs_.end()) {
          named_outs_[id] = new named_out(ch_, id, scope, type_, exec_);
          return true;
        }
        return false;
      }
      bool unbind(id_type & id) {
        typename std::map<id_type, named_out*>::iterator iter = named_outs_.find(id);
        if (iter != named_outs_.end()) {
          delete iter->second;
          named_outs_.erase(iter);
          return true;
        }
        return false;
      }
      void unbind_all(void) {
        for(typename std::map<id_type, named_out*>::iterator iter=named_outs_.begin();
            iter != named_outs_.end(); iter++)
          delete iter->second;
        named_outs_.clear();
      }
    private:
      name_space & ch_;
      typename named_out::member_type type_;
      executor_type *exec_;
      std::map<id_type, named_out*> named_outs_;
    };

    template <
      typename name_space,
      typename dispatcher_recver_type
      >
    class named_in_bundle {
    public:
      typedef typename name_space::synch_policy synch_policy;
      typedef typename name_space::id_type id_type;
      typedef typename name_space::id_trait id_trait;
      typedef typename name_space::executor executor_type;
      typedef named_in<name_space,dispatcher_recver_type> named_in;
    private:
      struct named_in_creator_base {
        virtual named_in* create(name_space & ch, id_type &id,
                                 typename named_in::scope_type scope,
                                 typename named_in::member_type t,
                                 executor_type *e) = 0;
        virtual ~named_in_creator_base() {}
      };
      template <typename recver_type>
      struct named_in_creator: public named_in_creator_base {
        recver_type recver_;
        named_in_creator(recver_type rc): recver_(rc) {}
        named_in* create(name_space & ch, id_type &id,
                         typename named_in::scope_type scope,
                         typename named_in::member_type t,
                         executor_type *e) {
          return new named_in(ch,id,recver_,scope,t,e);
        }
      };
      name_space & ch_;
      typename named_in::member_type type_;
      executor_type *exec_;
      std::map<id_type, named_in*> named_ins_;
      named_in_creator_base *creator_;
    public:
      template <typename recver_type>
      named_in_bundle(name_space & ch, recver_type rc,
                      typename named_in::member_type type = named_in::member_local,
                      executor_type *e = NULL) :
        ch_(ch), type_(type), exec_(e!=NULL?e:ch.get_exec()), creator_(new named_in_creator<recver_type>(rc)) {
      }
      ~named_in_bundle() {
        for(typename std::map<id_type, named_in*>::iterator iter=named_ins_.begin();
            iter != named_ins_.end(); iter++)
          delete iter->second;
        delete creator_;
      }
      int get_ids(std::vector<id_type> &ids) {
        for(typename std::map<id_type, named_in*>::iterator iter=named_ins_.begin();
            iter != named_ins_.end(); iter++)
          ids.push_back(iter->first);
        return ids.size();
      }
      named_in * find(id_type & id) {
        typename std::map<id_type, named_in*>::iterator iter = named_ins_.find(id);
        if (iter != named_ins_.end()) {
          return iter->second;
        }
        return NULL;
      }
      named_in * find_match(id_type & id) {
        for(typename std::map<id_type, named_in*>::iterator iter = named_ins_.begin();
            iter != named_ins_.end(); iter++)
          if(id_trait::match(iter->first, id)) {
            return iter->second;
          }
        return NULL;
      }
      void bind(std::vector<std::pair<id_type,typename named_in::scope_type> > & ids) {
        for(typename std::vector<std::pair<id_type,typename named_in::scope_type> >::iterator iter=ids.begin();
            iter != ids.end(); iter++) {
          bind(iter->first, iter->second);
        }
      }
      bool bind(id_type & id, typename named_in::scope_type scope = named_in::scope_global) {
        if (named_ins_.find(id) == named_ins_.end()) {
          named_ins_[id] = creator_->create(ch_, id, scope, type_, exec_);
          return true;
        }
        return false;
      }
      bool unbind(id_type & id) {
        typename std::map<id_type, named_in*>::iterator iter = named_ins_.find(id);
        if (iter != named_ins_.end()) {
          delete iter->second;
          named_ins_.erase(iter);
          return true;
        }
        return false;
      }
      void unbind_all(void) {
        for(typename std::map<id_type, named_in*>::iterator iter=named_ins_.begin();
            iter != named_ins_.end(); iter++)
          delete iter->second;
        named_ins_.clear();
      }
    };
  }
}

#endif

