//
// boost/join/port_sig.hpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_PORT_SIG_HPP
#define BOOST_JOIN_PORT_SIG_HPP

#include <boost/join/base/port.hpp>
#include <boost/type_traits/function_traits.hpp>

namespace boost {
  namespace join {

    template <typename Signature>
    class async_o;

    template <typename Signature>
    class synch_o;

    namespace detail {

      //---- argument wrappers for async<>/synch<> methods
      template <typename T1>
      class args1 {
      public:
        T1 arg1;
        args1(T1 a1): arg1(a1) {}
        args1() {}
      };

      template <typename T1, typename T2>
      class args2 {
      public:
        T1 arg1;
        T2 arg2;
        args2(T1 a1, T2 a2): arg1(a1), arg2(a2) {}
        args2() {}
      };

      template <typename T1, typename T2, typename T3>
      class args3 {
      public:
        T1 arg1;
        T2 arg2;
        T3 arg3;
        args3(T1 a1, T2 a2, T3 a3): arg1(a1), arg2(a2), arg3(a3) {}
        args3() {}
      };

      template <typename T1, typename T2, typename T3, typename T4>
      class args4 {
      public:
        T1 arg1;
        T2 arg2;
        T3 arg3;
        T4 arg4;
        args4(T1 a1, T2 a2, T3 a3, T4 a4): arg1(a1), arg2(a2), arg3(a3), arg4(a4) {}
        args4() {}
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5>
      class args5 {
      public:
        T1 arg1;
        T2 arg2;
        T3 arg3;
        T4 arg4;
        T5 arg5;
        args5(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5): arg1(a1), arg2(a2), arg3(a3), arg4(a4), arg5(a5) {}
        args5() {}
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
      class args6 {
      public:
        T1 arg1;
        T2 arg2;
        T3 arg3;
        T4 arg4;
        T5 arg5;
        T6 arg6;
        args6(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6): arg1(a1), arg2(a2), arg3(a3), arg4(a4), arg5(a5), arg6(a6) {}
        args6() {}
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
      class args7 {
      public:
        T1 arg1;
        T2 arg2;
        T3 arg3;
        T4 arg4;
        T5 arg5;
        T6 arg6;
        T7 arg7;
        args7(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7): arg1(a1), arg2(a2), arg3(a3), arg4(a4), arg5(a5), arg6(a6), arg7(a7) {}
        args7() {}
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
      class args8 {
      public:
        T1 arg1;
        T2 arg2;
        T3 arg3;
        T4 arg4;
        T5 arg5;
        T6 arg6;
        T7 arg7;
        T8 arg8;
        args8(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8): arg1(a1), arg2(a2), arg3(a3), arg4(a4), arg5(a5), arg6(a6), arg7(a7), arg8(a8) {}
        args8() {}
      };


      //---- for async methods ----

      template <typename Signature>
      class async0 : public async_p<void> {
      public:
        typedef void arg1_type;
        typedef void args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<void> base_type;

        async0(size_t sz=0) : base_type(sz) {}
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T, typename Signature>
      class async1 : public async_p<args1<T> > {
      public:
        typedef T arg1_type;
        typedef args1<T> args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<args1<T> > base_type;

        async1(size_t sz=0) : base_type(sz) {}
        void operator()(T a1) {
          base_type::put(args_type(a1));
        }
        void put(T a1) {
          base_type::put(args_type(a1));
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T, typename Signature>
      class async1_v : public args1<T> {
      public:
        typedef args1<T> args_type;
        typedef async1<T, Signature> port_type;
        port_type *port_;
        async1_v(port_type *p): args_type(p->get().arg1), port_(p) {}
        async1_v() {}
        void operator()(T a1) {
          port_->put(a1);
        }
        operator T() {
          return this->arg1;
        }
        /* ???need redo for ref arg
        T &arg(void) {
          return this->arg1;
        }
        */
      };


      template <typename T1, typename T2, typename Signature>
      class async2 : public async_p<args2<T1,T2> > {
      public:
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef args2<T1,T2> args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<args2<T1,T2> > base_type;

        async2(size_t sz=0) : base_type(sz) {}
        void operator()(T1 a1, T2 a2) {
          base_type::put(args_type(a1,a2));
        }
        void put(T1 a1, T2 a2) {
          base_type::put(args_type(a1,a2));
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T1, typename T2, typename Signature>
      class async2_v : public args2<T1,T2> {
      public:
        typedef args2<T1,T2> args_type;
        typedef async2<T1, T2, Signature> port_type;
        port_type *port_;
        async2_v(port_type *p): args_type(p->get().arg1, p->get().arg2), port_(p) {}
        async2_v() {}
        void operator()(T1 a1, T2 a2) {
          port_->put(a1,a2);
        }
      };


      template <typename T1, typename T2, typename T3, typename Signature>
      class async3 : public async_p<args3<T1,T2,T3> > {
      public:
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef args3<T1,T2,T3> args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<args3<T1,T2,T3> > base_type;

        async3(size_t sz=0) : base_type(sz) {}
        void operator()(T1 a1, T2 a2, T3 a3) {
          base_type::put(args_type(a1,a2,a3));
        }
        void put(T1 a1, T2 a2, T3 a3) {
          base_type::put(args_type(a1,a2,a3));
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T1, typename T2, typename T3, typename Signature>
      class async3_v : public args3<T1,T2,T3> {
      public:
        typedef args3<T1,T2,T3> args_type;
        typedef async3<T1, T2, T3, Signature> port_type;
        port_type *port_;
        async3_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3), port_(p) {}
        async3_v() {}
        void operator()(T1 a1, T2 a2, T3 a3) {
          port_->put(a1,a2,a3);
        }
      };

      template <typename T1, typename T2, typename T3, typename T4, typename Signature>
      class async4 : public async_p<args4<T1,T2,T3,T4> > {
      public:
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef args4<T1,T2,T3,T4> args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<args4<T1,T2,T3,T4> > base_type;

        async4(size_t sz=0) : base_type(sz) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4) {
          base_type::put(args_type(a1,a2,a3,a4));
        }
        void put(T1 a1, T2 a2, T3 a3, T4 a4) {
          base_type::put(args_type(a1,a2,a3,a4));
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T1, typename T2, typename T3, typename T4, typename Signature>
      class async4_v : public args4<T1,T2,T3,T4> {
      public:
        typedef args4<T1,T2,T3,T4> args_type;
        typedef async4<T1, T2, T3, T4, Signature> port_type;
        port_type *port_;
        async4_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4), port_(p) {}
        async4_v() {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4) {
          port_->put(a1,a2,a3,a4);
        }
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename Signature>
      class async5 : public async_p<args5<T1,T2,T3,T4,T5> > {
      public:
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef T5 arg5_type;
        typedef args5<T1,T2,T3,T4,T5> args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<args5<T1,T2,T3,T4,T5> > base_type;

        async5(size_t sz=0) : base_type(sz) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
          base_type::put(args_type(a1,a2,a3,a4,a5));
        }
        void put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
          base_type::put(args_type(a1,a2,a3,a4,a5));
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename Signature>
      class async5_v : public args5<T1,T2,T3,T4,T5> {
      public:
        typedef args5<T1,T2,T3,T4,T5> args_type;
        typedef async5<T1, T2, T3, T4, T5, Signature> port_type;
        port_type *port_;
        async5_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5), port_(p) {}
        async5_v() {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
          port_->put(a1,a2,a3,a4,a5);
        }
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename Signature>
      class async6 : public async_p<args6<T1,T2,T3,T4,T5,T6> > {
      public:
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef T5 arg5_type;
        typedef T6 arg6_type;
        typedef args6<T1,T2,T3,T4,T5,T6> args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<args6<T1,T2,T3,T4,T5,T6> > base_type;

        async6(size_t sz=0) : base_type(sz) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
          base_type::put(args_type(a1,a2,a3,a4,a5,a6));
        }
        void put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
          base_type::put(args_type(a1,a2,a3,a4,a5,a6));
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename Signature>
      class async6_v : public args6<T1,T2,T3,T4,T5,T6> {
      public:
        typedef args6<T1,T2,T3,T4,T5,T6> args_type;
        typedef async6<T1, T2, T3, T4, T5, T6, Signature> port_type;
        port_type *port_;
        async6_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6), port_(p) {}
        async6_v() {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
          port_->put(a1,a2,a3,a4,a5,a6);
        }
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename Signature>
      class async7 : public async_p<args7<T1,T2,T3,T4,T5,T6,T7> > {
      public:
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef T5 arg5_type;
        typedef T6 arg6_type;
        typedef T7 arg7_type;
        typedef args7<T1,T2,T3,T4,T5,T6,T7> args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<args7<T1,T2,T3,T4,T5,T6,T7> > base_type;

        async7(size_t sz=0) : base_type(sz) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
          base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7));
        }
        void put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
          base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7));
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename Signature>
      class async7_v : public args7<T1,T2,T3,T4,T5,T6,T7> {
      public:
        typedef args7<T1,T2,T3,T4,T5,T6,T7> args_type;
        typedef async7<T1, T2, T3, T4, T5, T6, T7, Signature> port_type;
        port_type *port_;
        async7_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6, p->get().arg7), port_(p) {}
        async7_v() {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
          port_->put(a1,a2,a3,a4,a5,a6,a7);
        }
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename Signature>
      class async8 : public async_p<args8<T1,T2,T3,T4,T5,T6,T7,T8> > {
      public:
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef T5 arg5_type;
        typedef T6 arg6_type;
        typedef T7 arg7_type;
        typedef T8 arg8_type;
        typedef args8<T1,T2,T3,T4,T5,T6,T7,T8> args_type;
        typedef async_o<Signature> var_type;
        typedef async_p<args8<T1,T2,T3,T4,T5,T6,T7,T8> > base_type;

        async8(size_t sz=0) : base_type(sz) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
          base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7,a8));
        }
        void put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
          base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7,a8));
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename Signature>
      class async8_v : public args8<T1,T2,T3,T4,T5,T6,T7,T8> {
      public:
        typedef args8<T1,T2,T3,T4,T5,T6,T7,T8> args_type;
        typedef async8<T1, T2, T3, T4, T5, T6, T7, T8, Signature> port_type;
        port_type *port_;
        async8_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6, p->get().arg7, p->get().arg8), port_(p) {}
        async8_v() {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
          port_->put(a1,a2,a3,a4,a5,a6,a7,a8);
        }
      };


      //--- templates to help signature ---

      //--- for async
      template<int Arity, typename Signature>
      class real_get_async_impl;

      template<typename Signature>
      class real_get_async_impl<0, Signature>
      {
      public:
        typedef async0<Signature> type;
      };

      template<typename Signature>
      class real_get_async_impl<1, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async1<typename traits::arg1_type, Signature> type;
      };

      template<typename Signature>
      class real_get_async_impl<2, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async2<typename traits::arg1_type,
                       typename traits::arg2_type, 
                       Signature> type;
      };

      template<typename Signature>
      class real_get_async_impl<3, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async3<typename traits::arg1_type,
                       typename traits::arg2_type, 
                       typename traits::arg3_type, 
                       Signature> type;
      };

      template<typename Signature>
      class real_get_async_impl<4, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async4<typename traits::arg1_type,
                       typename traits::arg2_type, 
                       typename traits::arg3_type, 
                       typename traits::arg4_type, 
                       Signature> type;
      };

      template<typename Signature>
      class real_get_async_impl<5, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async5<typename traits::arg1_type,
                       typename traits::arg2_type, 
                       typename traits::arg3_type, 
                       typename traits::arg4_type, 
                       typename traits::arg5_type, 
                       Signature> type;
      };

      template<typename Signature>
      class real_get_async_impl<6, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async6<typename traits::arg1_type,
                       typename traits::arg2_type, 
                       typename traits::arg3_type, 
                       typename traits::arg4_type, 
                       typename traits::arg5_type, 
                       typename traits::arg6_type, 
                       Signature> type;
      };

      template<typename Signature>
      class real_get_async_impl<7, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async7<typename traits::arg1_type,
                       typename traits::arg2_type, 
                       typename traits::arg3_type, 
                       typename traits::arg4_type, 
                       typename traits::arg5_type, 
                       typename traits::arg6_type, 
                       typename traits::arg7_type, 
                       Signature> type;
      };

      template<typename Signature>
      class real_get_async_impl<8, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async8<typename traits::arg1_type,
                       typename traits::arg2_type, 
                       typename traits::arg3_type, 
                       typename traits::arg4_type, 
                       typename traits::arg5_type, 
                       typename traits::arg6_type, 
                       typename traits::arg7_type, 
                       typename traits::arg8_type, 
                       Signature> type;
      };


      template<typename Signature>
      struct get_async_impl :
        public real_get_async_impl<(function_traits<Signature>::arity),
                                   Signature>
      {
      };

      //--- for async_v
      template<int Arity, typename Signature>
      class real_get_async_v_impl;

      template<typename Signature>
      class real_get_async_v_impl<0, Signature>
      {
      public:
        typedef async_v<void> type;
      };

      template<typename Signature>
      class real_get_async_v_impl<1, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async1_v<typename traits::arg1_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_async_v_impl<2, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async2_v<typename traits::arg1_type,
                         typename traits::arg2_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_async_v_impl<3, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async3_v<typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_async_v_impl<4, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async4_v<typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_async_v_impl<5, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async5_v<typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_async_v_impl<6, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async6_v<typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         typename traits::arg6_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_async_v_impl<7, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async7_v<typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         typename traits::arg6_type,
                         typename traits::arg7_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_async_v_impl<8, Signature>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef async8_v<typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         typename traits::arg6_type,
                         typename traits::arg7_type,
                         typename traits::arg8_type,
                         Signature> type;
      };


      template<typename Signature>
      struct get_async_v_impl :
        public real_get_async_v_impl<(function_traits<Signature>::arity),
                                     Signature>
      {
      };


      //--- for synch methods ---

      template <typename R, typename Signature>
      class synch0 : public synch_p<R,void> {
      public:
        typedef R result_type;
        typedef void arg1_type;
        typedef void args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R,void> base_type;

        synch0(size_t sz=0) : base_type(sz) {}
        R operator()(void) {
          return base_type::put();
        }
        R put(void) {
          return base_type::put();
        }
        R operator()(const boost::xtime& timeout) {
          return base_type::put(timeout);
        }
        R put(const boost::xtime& timeout) {
          return base_type::put(timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T, typename Signature>
      class synch1 : public synch_p<R, T> {
      public:
        typedef R result_type;
        typedef T arg1_type;
        typedef T args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R, T> base_type;

        synch1(size_t sz=0) : base_type(sz) {}
        R operator()(T a1) {
          return base_type::put(a1);
        }
        R put(T a1) {
          return base_type::put(a1);
        }
        R operator()(T a1, const boost::xtime& timeout) {
          return base_type::put(a1, timeout);
        }
        R put(T a1, const boost::xtime& timeout) {
          return base_type::put(a1, timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T1, typename T2, typename Signature>
      class synch2 : public synch_p<R, args2<T1,T2> > {
      public:
        typedef R result_type;
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef args2<T1,T2> args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R, args2<T1,T2> > base_type;

        synch2(size_t sz=0) : base_type(sz) {}
        R operator()(T1 a1, T2 a2) {
          return base_type::put(args_type(a1,a2));
        }
        R put(T1 a1, T2 a2) {
          return base_type::put(args_type(a1,a2));
        }
        R operator()(T1 a1, T2 a2, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2), timeout);
        }
        R put(T1 a1, T2 a2, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2), timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T1, typename T2, typename Signature>
      class synch2_v : public args2<T1,T2> {
      public:
        typedef args2<T1,T2> args_type;
        typedef synch2<R, T1, T2, Signature> port_type;
        port_type *port_;
        bool ret_flag_;
        synch2_v(port_type *p): args_type(p->get().arg1, p->get().arg2), port_(p), ret_flag_(false) {}
        R operator()(T1 a1, T2 a2) {
          return port_->put(a1,a2);
        }
      };
      template <typename T1, typename T2, typename Signature>
      class synch2_v<void,T1,T2,Signature> : public args2<T1,T2> {
      public:
        typedef args2<T1,T2> args_type;
        typedef synch2<void, T1, T2, Signature> port_type;
        port_type *port_;
        synch2_v(port_type *p): args_type(p->get().arg1, p->get().arg2), port_(p) {}
        void operator()(T1 a1, T2 a2) {
          port_->put(a1,a2);
        }
      };

      template <typename R, typename T1, typename T2, typename T3, typename Signature>
      class synch3 : public synch_p<R, args3<T1,T2,T3> > {
      public:
        typedef R result_type;
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef args3<T1,T2,T3> args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R, args3<T1,T2,T3> > base_type;

        synch3(size_t sz=0) : base_type(sz) {}
        R operator()(T1 a1, T2 a2, T3 a3) {
          return base_type::put(args_type(a1,a2,a3));
        }
        R put(T1 a1, T2 a2, T3 a3) {
          return base_type::put(args_type(a1,a2,a3));
        }
        R operator()(T1 a1, T2 a2, T3 a3, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3), timeout);
        }
        R put(T1 a1, T2 a2, T3 a3, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3), timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T1, typename T2, typename T3, typename Signature>
      class synch3_v : public args3<T1,T2,T3> {
      public:
        typedef args3<T1,T2,T3> args_type;
        typedef synch3<R, T1, T2, T3, Signature> port_type;
        port_type *port_;
        bool ret_flag_;
        synch3_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3), port_(p), ret_flag_(false) {}
        R operator()(T1 a1, T2 a2, T3 a3) {
          return port_->put(a1,a2,a3);
        }
      };
      template <typename T1, typename T2, typename T3, typename Signature>
      class synch3_v<void,T1,T2,T3,Signature> : public args3<T1,T2,T3> {
      public:
        typedef args3<T1,T2,T3> args_type;
        typedef synch3<void, T1, T2, T3, Signature> port_type;
        port_type *port_;
        synch3_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3), port_(p) {}
        void operator()(T1 a1, T2 a2, T3 a3) {
          port_->put(a1,a2,a3);
        }
      };

      template <typename R, typename T1, typename T2, typename T3, typename T4, typename Signature>
      class synch4 : public synch_p<R, args4<T1,T2,T3,T4> > {
      public:
        typedef R result_type;
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef args4<T1,T2,T3,T4> args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R, args4<T1,T2,T3,T4> > base_type;

        synch4(size_t sz=0) : base_type(sz) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4) {
          return base_type::put(args_type(a1,a2,a3,a4));
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4) {
          return base_type::put(args_type(a1,a2,a3,a4));
        }
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4), timeout);
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4), timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T1, typename T2, typename T3, typename T4, typename Signature>
      class synch4_v : public args4<T1,T2,T3,T4> {
      public:
        typedef args4<T1,T2,T3,T4> args_type;
        typedef synch4<R, T1, T2, T3, T4, Signature> port_type;
        port_type *port_;
        bool ret_flag_;
        synch4_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4), port_(p), ret_flag_(false) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4) {
          return port_->put(a1,a2,a3,a4);
        }
      };
      template <typename T1, typename T2, typename T3, typename T4, typename Signature>
      class synch4_v<void,T1,T2,T3,T4,Signature> : public args4<T1,T2,T3,T4> {
      public:
        typedef args4<T1,T2,T3,T4> args_type;
        typedef synch4<void, T1, T2, T3, T4, Signature> port_type;
        port_type *port_;
        synch4_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4), port_(p) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4) {
          port_->put(a1,a2,a3,a4);
        }
      };


      template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename Signature>
      class synch5 : public synch_p<R, args5<T1,T2,T3,T4,T5> > {
      public:
        typedef R result_type;
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef T5 arg5_type;
        typedef args5<T1,T2,T3,T4,T5> args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R, args5<T1,T2,T3,T4,T5> > base_type;

        synch5(size_t sz=0) : base_type(sz) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
          return base_type::put(args_type(a1,a2,a3,a4,a5));
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
          return base_type::put(args_type(a1,a2,a3,a4,a5));
        }
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4,a5), timeout);
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4,a5), timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename Signature>
      class synch5_v : public args5<T1,T2,T3,T4,T5> {
      public:
        typedef args5<T1,T2,T3,T4,T5> args_type;
        typedef synch5<R, T1, T2, T3, T4, T5, Signature> port_type;
        port_type *port_;
        bool ret_flag_;
        synch5_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5), port_(p), ret_flag_(false) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
          return port_->put(a1,a2,a3,a4,a5);
        }
      };
      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename Signature>
      class synch5_v<void,T1,T2,T3,T4,T5,Signature> : public args5<T1,T2,T3,T4,T5> {
      public:
        typedef args5<T1,T2,T3,T4,T5> args_type;
        typedef synch5<void, T1, T2, T3, T4, T5, Signature> port_type;
        port_type *port_;
        synch5_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5), port_(p) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
          port_->put(a1,a2,a3,a4,a5);
        }
      };

      template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename Signature>
      class synch6 : public synch_p<R, args6<T1,T2,T3,T4,T5,T6> > {
      public:
        typedef R result_type;
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef T5 arg5_type;
        typedef T6 arg6_type;
        typedef args6<T1,T2,T3,T4,T5,T6> args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R, args6<T1,T2,T3,T4,T5,T6> > base_type;

        synch6(size_t sz=0) : base_type(sz) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6));
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6));
        }
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6), timeout);
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6), timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename Signature>
      class synch6_v : public args6<T1,T2,T3,T4,T5,T6> {
      public:
        typedef args6<T1,T2,T3,T4,T5,T6> args_type;
        typedef synch6<R, T1, T2, T3, T4, T5, T6, Signature> port_type;
        port_type *port_;
        bool ret_flag_;
        synch6_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6), port_(p), ret_flag_(false) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
          return port_->put(a1,a2,a3,a4,a5,a6);
        }
      };
      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename Signature>
      class synch6_v<void,T1,T2,T3,T4,T5,T6,Signature> : public args6<T1,T2,T3,T4,T5,T6> {
      public:
        typedef args6<T1,T2,T3,T4,T5,T6> args_type;
        typedef synch6<void, T1, T2, T3, T4, T5, T6, Signature> port_type;
        port_type *port_;
        synch6_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6), port_(p) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
          port_->put(a1,a2,a3,a4,a5,a6);
        }
      };


      template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename Signature>
      class synch7 : public synch_p<R, args7<T1,T2,T3,T4,T5,T6,T7> > {
      public:
        typedef R result_type;
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef T5 arg5_type;
        typedef T6 arg6_type;
        typedef T7 arg7_type;
        typedef args7<T1,T2,T3,T4,T5,T6,T7> args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R, args7<T1,T2,T3,T4,T5,T6,T7> > base_type;

        synch7(size_t sz=0) : base_type(sz) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7));
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7));
        }
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7), timeout);
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7), timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename Signature>
      class synch7_v : public args7<T1,T2,T3,T4,T5,T6,T7> {
      public:
        typedef args7<T1,T2,T3,T4,T5,T6,T7> args_type;
        typedef synch7<R, T1, T2, T3, T4, T5, T6, T7, Signature> port_type;
        port_type *port_;
        bool ret_flag_;
        synch7_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6, p->get().arg7), port_(p), ret_flag_(false) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
          return port_->put(a1,a2,a3,a4,a5,a6,a7);
        }
      };
      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename Signature>
      class synch7_v<void,T1,T2,T3,T4,T5,T6,T7,Signature> : public args7<T1,T2,T3,T4,T5,T6,T7> {
      public:
        typedef args7<T1,T2,T3,T4,T5,T6,T7> args_type;
        typedef synch7<void, T1, T2, T3, T4, T5, T6, T7, Signature> port_type;
        port_type *port_;
        synch7_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6, p->get().arg7), port_(p) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
          port_->put(a1,a2,a3,a4,a5,a6,a7);
        }
      };

      template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename Signature>
      class synch8 : public synch_p<R, args8<T1,T2,T3,T4,T5,T6,T7,T8> > {
      public:
        typedef R result_type;
        typedef T1 arg1_type;
        typedef T2 arg2_type;
        typedef T3 arg3_type;
        typedef T4 arg4_type;
        typedef T5 arg5_type;
        typedef T6 arg6_type;
        typedef T7 arg7_type;
        typedef T8 arg8_type;
        typedef args8<T1,T2,T3,T4,T5,T6,T7,T8> args_type;
        typedef synch_o<Signature> var_type;
        typedef synch_p<R, args8<T1,T2,T3,T4,T5,T6,T7,T8> > base_type;

        synch8(size_t sz=0) : base_type(sz) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7,a8));
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7,a8));
        }
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7,a8), timeout);
        }
        R put(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, const boost::xtime& timeout) {
          return base_type::put(args_type(a1,a2,a3,a4,a5,a6,a7,a8), timeout);
        }
        var_type top(void) {
          return var_type(this);
        }      
      };

      template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename Signature>
      class synch8_v : public args8<T1,T2,T3,T4,T5,T6,T7,T8> {
      public:
        typedef args8<T1,T2,T3,T4,T5,T6,T7,T8> args_type;
        typedef synch8<R, T1, T2, T3, T4, T5, T6, T7, T8, Signature> port_type;
        port_type *port_;
        bool ret_flag_;
        synch8_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6, p->get().arg7, p->get().arg8), port_(p), ret_flag_(false) {}
        R operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
          return port_->put(a1,a2,a3,a4,a5,a6,a7,a8);
        }
      };
      template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename Signature>
      class synch8_v<void,T1,T2,T3,T4,T5,T6,T7,T8,Signature> : public args8<T1,T2,T3,T4,T5,T6,T7,T8> {
      public:
        typedef args8<T1,T2,T3,T4,T5,T6,T7,T8> args_type;
        typedef synch8<void, T1, T2, T3, T4, T5, T6, T7, T8, Signature> port_type;
        port_type *port_;
        synch8_v(port_type *p): args_type(p->get().arg1, p->get().arg2, p->get().arg3, p->get().arg4, p->get().arg5, p->get().arg6, p->get().arg7, p->get().arg8), port_(p) {}
        void operator()(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
          port_->put(a1,a2,a3,a4,a5,a6,a7,a8);
        }
      };


      //--- templates to help signature ---

      //--- for synch
      template<int Arity, typename Signature>
      class real_get_synch_impl;

      template<typename Signature>
      class real_get_synch_impl<0, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch0<typename traits::result_type, Signature> type;
      };

      template<typename Signature>
      class real_get_synch_impl<1, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch1<typename traits::result_type, 
                       typename traits::arg1_type,
                       Signature> type;
      };

      template<typename Signature>
      class real_get_synch_impl<2, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch2<typename traits::result_type, 
                       typename traits::arg1_type,
                       typename traits::arg2_type,
                       Signature> type;
      };

      template<typename Signature>
      class real_get_synch_impl<3, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch3<typename traits::result_type, 
                       typename traits::arg1_type,
                       typename traits::arg2_type,
                       typename traits::arg3_type,
                       Signature> type;
      };

      template<typename Signature>
      class real_get_synch_impl<4, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch4<typename traits::result_type, 
                       typename traits::arg1_type,
                       typename traits::arg2_type,
                       typename traits::arg3_type,
                       typename traits::arg4_type,
                       Signature> type;
      };

      template<typename Signature>
      class real_get_synch_impl<5, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch5<typename traits::result_type, 
                       typename traits::arg1_type,
                       typename traits::arg2_type,
                       typename traits::arg3_type,
                       typename traits::arg4_type,
                       typename traits::arg5_type,
                       Signature> type;
      };

      template<typename Signature>
      class real_get_synch_impl<6, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch6<typename traits::result_type, 
                       typename traits::arg1_type,
                       typename traits::arg2_type,
                       typename traits::arg3_type,
                       typename traits::arg4_type,
                       typename traits::arg5_type,
                       typename traits::arg6_type,
                       Signature> type;
      };

      template<typename Signature>
      class real_get_synch_impl<7, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch7<typename traits::result_type, 
                       typename traits::arg1_type,
                       typename traits::arg2_type,
                       typename traits::arg3_type,
                       typename traits::arg4_type,
                       typename traits::arg5_type,
                       typename traits::arg6_type,
                       typename traits::arg7_type,
                       Signature> type;
      };

      template<typename Signature>
      class real_get_synch_impl<8, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch8<typename traits::result_type, 
                       typename traits::arg1_type,
                       typename traits::arg2_type,
                       typename traits::arg3_type,
                       typename traits::arg4_type,
                       typename traits::arg5_type,
                       typename traits::arg6_type,
                       typename traits::arg7_type,
                       typename traits::arg8_type,
                       Signature> type;
      };


      template<typename Signature>
      struct get_synch_impl :
        public real_get_synch_impl<(function_traits<Signature>::arity),
                                   Signature>
      {
      };

      //--- for synch_v
      template<int Arity, typename Signature>
      class real_get_synch_v_impl;

      template<typename Signature>
      class real_get_synch_v_impl<0, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch_v<typename traits::result_type, void> type;
      };

      template<typename Signature>
      class real_get_synch_v_impl<1, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch_v<typename traits::result_type, 
                        typename traits::arg1_type> type;
      };

      template<typename Signature>
      class real_get_synch_v_impl<2, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch2_v<typename traits::result_type, 
                         typename traits::arg1_type,
                         typename traits::arg2_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_synch_v_impl<3, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch3_v<typename traits::result_type, 
                         typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_synch_v_impl<4, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch4_v<typename traits::result_type, 
                         typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         Signature> type;
      };


      template<typename Signature>
      class real_get_synch_v_impl<5, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch5_v<typename traits::result_type, 
                         typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_synch_v_impl<6, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch6_v<typename traits::result_type, 
                         typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         typename traits::arg6_type,
                         Signature> type;
      };


      template<typename Signature>
      class real_get_synch_v_impl<7, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch7_v<typename traits::result_type, 
                         typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         typename traits::arg6_type,
                         typename traits::arg7_type,
                         Signature> type;
      };

      template<typename Signature>
      class real_get_synch_v_impl<8, Signature>
      {
        typedef function_traits<Signature> traits;
      public:
        typedef synch8_v<typename traits::result_type, 
                         typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         typename traits::arg6_type,
                         typename traits::arg7_type,
                         typename traits::arg8_type,
                         Signature> type;
      };


      template<typename Signature>
      struct get_synch_v_impl :
        public real_get_synch_v_impl<(function_traits<Signature>::arity),
                                     Signature>
      {
      };

    }

    template<typename Signature>
    class synch :
      public detail::get_synch_impl<Signature>::type
    {
      typedef typename detail::get_synch_impl<Signature>::type base_type;

    public:
      explicit synch(size_t sz=0) : base_type(sz) {}
    };

    template<typename Signature>
    class synch_o :
      public detail::get_synch_v_impl<Signature>::type
    {
      typedef typename detail::get_synch_v_impl<Signature>::type base_type;
      typedef typename detail::get_synch_impl<Signature>::type port_type;

    public:
      explicit synch_o(port_type *p) : base_type(p) {}
    };

    template<typename Signature>
    class async :
      public detail::get_async_impl<Signature>::type
    {
      typedef typename detail::get_async_impl<Signature>::type base_type;

    public:
      explicit async(size_t sz=0) : base_type(sz) {}
    };

    template<typename Signature>
    class async_o :
      public detail::get_async_v_impl<Signature>::type
    {
      typedef typename detail::get_async_v_impl<Signature>::type base_type;
      typedef typename detail::get_async_impl<Signature>::type port_type;

    public:
      explicit async_o(port_type *p) : base_type(p) {}
      explicit async_o() : base_type() {}
    };


  }
}


#endif
