//
// null_name_space.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef NULL_NAME_SPACE_HPP
#define NULL_NAME_SPACE_HPP

#include <boost/channel/name.hpp>

namespace boost {
  namespace channel {
    /**
     *     null_name_space:
     *         convenience class for "unnamed" entities;
     *         no real functionalities; just for transferring type info
     */
    template<typename idtype, typename executor_type, typename synchpolicy,
             bool exact_match = true>
    class null_name_space {
    public:
      typedef idtype id_type;
      typedef id_trait<id_type> id_trait;
      typedef synchpolicy synch_policy;
      typedef executor_type executor;
      typedef	typename synch_policy::platform platform;
      //we dont care about In/Out operations here, just names and bindings
      typedef name<id_type,executor_type,synch_policy> name;

      void unbind_named_out(name *) {}
      void unbind_named_in(name *) {}
    };
  }
}

#endif
