//
// unnamed_in_out.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef UNNAMED_IN_OUT_HPP
#define UNNAMED_IN_OUT_HPP

#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/channel/name_spaces/null_name_space.hpp>
#include <boost/channel/dispatchers/arbiters_async.hpp>
#include <boost/channel/dispatchers/arbiters_sync.hpp>
#include <boost/channel/dispatchers/broadcast_dispatcher.hpp>
#include <boost/channel/dispatchers/round_robin_dispatcher.hpp>
#include <boost/channel/dispatchers/always_latest_dispatcher.hpp>
#include <boost/channel/dispatchers/pull_dispatcher.hpp>

namespace boost {
  namespace channel {

    /**
     * port class: "unnamed" named_out with pull_dispatcher
     *     same role as port in CCR and channel in C++CSP or JCSP
     */
    template<
      typename executor_type = abstract_executor, //force in-line execution
      typename idtype = null_id,
      typename platform_type = boost_platform,
      typename synchpolicy = mt_synch<platform_type> ,
      typename name_space = null_name_space<idtype, executor_type, synchpolicy> ,
      typename pulldispatcher = pull_dispatcher<name_space, platform_type>
      >
    class port: public named_out<name_space, typename pulldispatcher::sender> {
    public:
      typedef   typename pulldispatcher::choice_async choice_async;
      typedef typename pulldispatcher::join_async join_async;
      typedef typename pulldispatcher::choice_sync choice_sync;
      typedef typename pulldispatcher::join_sync join_sync;
      typedef named_out<name_space, typename pulldispatcher::sender> named_out_type;
      typedef named_in<name_space, typename pulldispatcher::recver> named_in_type;
      port(executor_type *e=NULL) : named_out_type(e) {}
    };

  /**
   * signal and slot: for synch event dispatching
   * signal : "unnamed" named_out with push_dispatchers
   * when signal(named_out) is destroyed, all its remaining slots(named_ins) are unbound;
   * their num_bindings() should return 0
   * when slots(named_in) are destroyed, they are unbound from named_in automatically
   */
  template <
    typename executor_type = abstract_executor, //force in-line execution
    typename idtype = null_id,
    typename platform_type = boost_platform,
    typename synchpolicy = mt_synch<platform_type>,
    typename name_space = null_name_space<idtype,executor_type,synchpolicy>,
    typename push_dispatcher = broadcast_dispatcher<name_space,platform_type>
    >
  class signal : public named_out<name_space, typename push_dispatcher::sender> {
  public:
    typedef named_in<name_space, typename push_dispatcher::recver> slot;
    typedef named_out<name_space, typename push_dispatcher::sender> named_out_type;
    typedef name<idtype, executor_type, synchpolicy> name_type;
    typedef typename name_type::binding_set_type binding_set_type;

    executor_type *exec_;
    signal(executor_type *e=NULL) : named_out_type(e), exec_(e) {}
    ~signal() {
      //signal should be the last to kill after all slots cleanup. do some cleanup ?
      binding_set_type & bindings0 = ((name_type*) this)->bindings_;
      //here something wrong with list::iterator!? so copy to a vector
      std::vector<name_type *> bindings(bindings0.begin(), bindings0.end());
      typename std::vector<name_type *>::iterator iter;
      for(iter = bindings.begin(); iter != bindings.end(); iter++) {
        delete ((slot *) (*iter));
      }
    }

    template <typename recver_type>
    slot * bind(recver_type rc) {
      slot * ss = new slot(boost::bind(rc, _2), exec_);
      ((name_type *)this)->bind((name_type *)ss);
      ((name_type *)ss)->bind((name_type *)this);
      return ss;
    }

    //when slots are destroyed, they unbind from signals automatically
    //here is for the case when we just want unbind, not destroy
    void unbind(slot *ss) {
      ((name_type *)this)->unbind((name_type *)ss);
      ((name_type *)ss)->unbind((name_type *)this);
    }

  };
}
}

#endif

