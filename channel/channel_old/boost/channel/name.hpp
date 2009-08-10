//
// name.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NAME_HPP
#define NAME_HPP

#include <list>
#include <map>
#include <algorithm>
#include <boost/utility.hpp>
#include <boost/bind.hpp>
#include <boost/channel/platforms/synch_policy.hpp>
#include <boost/channel/config.hpp>

namespace boost {
  namespace channel {

    namespace detail {

      //name binding callback; should i use boost::function?
      template<typename name>
      class binding_callback_base {
      public:
        void invoke(name *n, typename name::binding_event e) {
          invoke_func_(this, n, e);
        }

        void destroy() {
          destroy_func_(this);
        }

      protected:
        typedef void
        (*invoke_func_type)(binding_callback_base*, name *n, typename name::binding_event e);
        typedef void (*destroy_func_type)(binding_callback_base*);

        binding_callback_base(invoke_func_type invoke_func, destroy_func_type destroy_func) :
          invoke_func_(invoke_func), destroy_func_(destroy_func) {
        }

        ~binding_callback_base() {
        }

      private:
        invoke_func_type invoke_func_;
        destroy_func_type destroy_func_;
      };

      template<typename name, typename handler_type>
      class binding_callback: public binding_callback_base<name> {
      public:
        binding_callback(handler_type handler) :
          binding_callback_base<name> (&binding_callback<name, handler_type>::invoke_handler,
                                       &binding_callback<name, handler_type>::destroy_handler), handler_(handler) {
        }

        static void invoke_handler(binding_callback_base<name>* base, name *n,
                                   typename name::binding_event e) {
          static_cast<binding_callback<name, handler_type>*> (base)->handler_(n, e);
        }

        static void destroy_handler(binding_callback_base<name>* base) {
          delete static_cast<binding_callback<name, handler_type>*> (base);
        }

      private:
        handler_type handler_;
      };

    }

    class BOOST_CHANNEL_DECL name_base {
    public:
      enum scope_type {
        scope_local = 0, scope_remote, scope_global, scope_number
      };

      enum member_type {
        member_all = -1, member_local = 0,//local queues and callbacks
        member_remote, //interfaces to remote/local name_spaces
        member_number
      };

      enum binding_event {
        bind_out_ev = 0, unbind_out_ev, bind_in_ev, unbind_in_ev, bind_ev, unbind_ev
      };

      ///binding scope checking table:
      ///row: publish_member_type * scope_number + publish_scope
      ///col: subscribe_member_type * scope_number + subscribe_scope
      ///since "remote" members can only send_to/recv_from "local"
      ///members, only the upper-left 4x4 sub-matrix have value
      ///value "1" marks a valid combo of publisher(row) sending to subscriber(col)
      ///value "0" marks invalid
      static short scope_checking_tbl_[][name_base::scope_number * name_base::member_number];
    };

    /// "name" should be noncopyable; otherwise the connection between
    /// named_in and named_out will be messed up
    template<typename id_type, typename executor, typename synch_policy>
    class BOOST_CHANNEL_DECL name: public name_base, private boost::noncopyable {
    public:

      typedef std::list<name *> binding_set_type;

      id_type id_;
      typename name_base::scope_type scope_;
      typename name_base::member_type type_;
      executor *exec_;

      typename synch_policy::mutex bind_lock_;
      std::list<name *> bindings_;
      detail::binding_callback_base<name> *binding_cb_;

      /*
        template <typename id_type, typename synch_policy>
        friend bool operator< (const name<id_type,synch_policy> &left,
        const name<id_type,synch_policy> &right);
      */
    protected:
      //make constructor protected, so only children can be created
      template<typename binding_cb_type>
        name(id_type id, scope_type scope, member_type type, binding_cb_type cb, executor *e) :
        id_(id), scope_(scope), type_(type), exec_(e), binding_cb_(new detail::binding_callback<name,
                                                                   binding_cb_type>(cb)) {
      }

      name(id_type id, scope_type scope, member_type type) :
        id_(id), scope_(scope), type_(type), exec_(NULL), binding_cb_(NULL) {
      }

      name() :
        id_(), scope_(scope_local), type_(member_local), exec_(NULL), binding_cb_(NULL) {
      }

      ~name() {
        unbind_all(); //detach all peers
        if (binding_cb_ != NULL)
          binding_cb_->destroy();
      }

      template<typename binding_cb_type>
        void set_name(id_type id, scope_type scope, member_type type, binding_cb_type cb, executor *e) {
        id_ = id;
        scope_ = scope;
        type_ = type;
        exec_ = e;
        binding_cb_ = new detail::binding_callback<name, binding_cb_type>(cb);
      }

      void set_name(id_type id, scope_type scope, member_type type) {
        id_ = id;
        scope_ = scope;
        type_ = type;
      }

      void binding_callback(name *n, binding_event e) {
        if (binding_cb_ != NULL) {
          if (exec_ != NULL) //execute binding callback in executor
            exec_->execute(boost::bind(&detail::binding_callback_base<name>::invoke, binding_cb_,
                                       n, e));
          else
            //execute in place
            binding_cb_->invoke(n, e);
        }
      }

    public:
      int num_bindings(void) {
        return bindings_.size();
      }
      std::list<name *> & bindings(void) {
        return bindings_;
      }

      //validate the src and dest scope matches
      static bool scope_checking(name *src, name *dst) {
        short src_row = src->type_ * name_base::scope_number + src->scope_;
        short dst_col = dst->type_ * name_base::scope_number + dst->scope_;
        return name_base::scope_checking_tbl_[src_row][dst_col] == 1;
      }

      //the following 2 callbacks invoked by name_space to notify binding changes
      void bind(name *n) {
        typename synch_policy::scoped_lock lock(bind_lock_);
        //add n to bindings_
        if (std::find(bindings_.begin(), bindings_.end(), n) == bindings_.end()) {
          bindings_.push_front(n);
          //callback
          binding_callback(n, bind_ev);
        }
      }
      void unbind(name *n) {
        typename std::list<name *>::iterator iter;
        typename synch_policy::scoped_lock lock(bind_lock_);
        if ((iter = std::find(bindings_.begin(), bindings_.end(), n)) != bindings_.end()) {
          //remove n from bindings_
          //bindings_.remove(n);
          bindings_.erase(iter);
          //callback
          binding_callback(n, unbind_ev);
        }
      }

      //during name-destruction, disconn all
      void unbind_all(void) {
        typename synch_policy::scoped_lock lock(bind_lock_);
        //unbind_all
        for (typename std::list<name *>::iterator iter = bindings_.begin(); iter != bindings_.end();) {
          //remove n from bindings_
          typename std::list<name *>::iterator cur = iter;
          name *n = *cur;
          n->unbind(this); //remove reference to me at remote
          iter++;
          bindings_.erase(cur);
        }
      }

      //unbind all external bindings; this must be a member_local
      void unbind_all_external(void) {
        typename synch_policy::scoped_lock lock(bind_lock_);
        //unbind_all_external
        for (typename std::list<name *>::iterator iter = bindings_.begin(); iter != bindings_.end();)
          if ((*iter)->type_ == member_remote) {
            //remove n from bindings_
            typename std::list<name *>::iterator cur = iter;
            name *n = *cur;
            n->unbind(this); //remove reference to me at remote
            iter++;
            bindings_.erase(cur);
          }
      }

      //predicates used for name space queries
      ///names from internal and visible at interfaces to external channels
      static bool exported_name(name *n) {
        if (n->type_ == member_local && (n->scope_ == scope_remote || n->scope_ == scope_global))
          return true;
        return false;
      }

      ///names from internal and visible to internal peers
      static bool internal_name(name *n) {
        if (n->type_ == member_local && (n->scope_ == scope_local || n->scope_ == scope_global))
          return true;
        return false;
      }

      ///all/any names in name_space
      static bool any_name(name *n) {
        return true;
      }

    };

    /*
      template <typename id_type, typename synch_policy>
      friend bool operator< (const name<id_type,synch_policy> &left,
      const name<id_type,synch_policy> &right) {
      return left.id_ < right.id_;
      }
    */
  }
}

#endif

