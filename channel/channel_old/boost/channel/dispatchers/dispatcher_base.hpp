//
// dispatcher_base.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


//// dispatcher_base:
//// base handler definitions; should we use boost::function instead?
////

#ifndef DISPATCHER_BASE_HPP
#define DISPATCHER_BASE_HPP

#include <map>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>

namespace boost {
  namespace channel {

    namespace detail {

      //message handlers; should i use boost::function instead of
      //duplicating all these code?
      template<typename id_type>
      class msg_handler_base {
      public:
	void invoke(id_type id, boost::shared_ptr<void> msg) {
          invoke_func_(this, id, msg);
	}

	void destroy() {
          destroy_func_(this);
	}

      protected:
	typedef void (*invoke_func_type)(msg_handler_base*, id_type id,
                                         boost::shared_ptr<void> msg);
	typedef void (*destroy_func_type)(msg_handler_base*);

	msg_handler_base(invoke_func_type invoke_func,
                         destroy_func_type destroy_func) :
          invoke_func_(invoke_func), destroy_func_(destroy_func) {
	}

	~msg_handler_base() {
	}

      private:
	invoke_func_type invoke_func_;
	destroy_func_type destroy_func_;
      };

      template<typename id_type, typename handler_type>
      class msg_handler: public msg_handler_base<id_type> {
	typedef msg_handler_base<id_type> msg_handler_base;
      public:
	msg_handler(handler_type handler) :
          msg_handler_base(&msg_handler<id_type, handler_type>::invoke_handler,
                           &msg_handler<id_type, handler_type>::destroy_handler),
          handler_(handler) {
	}

	static void invoke_handler(msg_handler_base* base, id_type id,
                                   boost::shared_ptr<void> msg) {
          static_cast<msg_handler<id_type, handler_type>*> (base)->handler_(id,
                                                                            msg);
	}

	static void destroy_handler(msg_handler_base* base) {
          delete static_cast<msg_handler<id_type, handler_type>*> (base);
	}

      private:
	handler_type handler_;
      };

      template<typename id_type>
      class msg_handler_base2 {
      public:
	void invoke(id_type id) {
          invoke_func_(this, id);
	}

	void destroy() {
          destroy_func_(this);
	}

      protected:
	typedef void (*invoke_func_type)(msg_handler_base2*, id_type id);
	typedef void (*destroy_func_type)(msg_handler_base2*);

	msg_handler_base2(invoke_func_type invoke_func,
                          destroy_func_type destroy_func) :
          invoke_func_(invoke_func), destroy_func_(destroy_func) {
	}

	~msg_handler_base2() {
	}

      private:
	invoke_func_type invoke_func_;
	destroy_func_type destroy_func_;
      };

      template<typename id_type, typename handler_type>
      class msg_handler2: public msg_handler_base2<id_type> {
	typedef msg_handler_base2<id_type> msg_handler_base2;
      public:
	msg_handler2(handler_type handler) :
          msg_handler_base2(&msg_handler2<id_type, handler_type>::invoke_handler,
                            &msg_handler2<id_type, handler_type>::destroy_handler),
          handler_(handler) {
	}

	static void invoke_handler(msg_handler_base2* base, id_type id) {
          static_cast<msg_handler2<id_type, handler_type>*> (base)->handler_(id);
	}

	static void destroy_handler(msg_handler_base2* base) {
          delete static_cast<msg_handler2<id_type, handler_type>*> (base);
	}

      private:
	handler_type handler_;
      };

    }

  }
}

#endif
