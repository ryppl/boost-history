//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/coroutine/coroutine.hpp"

namespace coro=boost::coroutines;

template <class Derived, typename Signature,
typename ContextImpl = detail::default_context_impl>
class coroutine_obj;

template <class Derived, typename Res, typename Param1, typename ContextImpl>
class coroutine_obj<Derived, Res(Param1), ContextImpl> {
    typedef coro::coroutine<Res(Param1), ContextImpl> coroutine_type;
    coroutine_type couroutine_;
public:
    coroutine_obj() : couroutine_(Derived::body_) {}
    Res operator()(Param1 val) {
        return couroutine_(val);
    }
};
template <class Derived, typename ContextImpl>
class coroutine_obj<Derived, void(), ContextImpl> {
    typedef coro::coroutine<void()> coroutine_type;
    coroutine_type couroutine_;
public:
    coroutine_obj() : couroutine_(Derived::body_) {}
    void operator()() {
        couroutine_();
    }
};

//[Acumulator
class Acumulator : public coroutine_obj<Acumulator, int(int)> {
    typedef coroutine_obj<Acumulator, int(int)> super_type;
    friend class super_type;
    static int body(coroutine_type::self& this_coroutine, int val) {
      while(true) {
        val += this_coroutine.yield(val);
      }
    }
};

//]

int main()
{
    Acumulator accumulator;
    for(int i = 0; i < 1000; ++i)
       std::cout << accumulator(i);
    return 0;
}


