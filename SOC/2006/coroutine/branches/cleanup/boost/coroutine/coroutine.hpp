//  Copyright (c) 2006, Giovanni P. Deretta
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINE_COROUTINE_HPP_20060512
#define BOOST_COROUTINE_COROUTINE_HPP_20060512
#include <cstddef>
#include "move.hpp"
#include "detail/coroutine_impl.hpp"
#include "detail/default_context_impl.hpp"
#include "detail/is_callable.hpp"
#include "detail/coroutine_accessor.hpp"
#include "detail/self.hpp"
#include "detail/coroutine.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
namespace boost { namespace coroutines {

  template<typename R, typename P, typename C>
  class coroutine;
  
  namespace detail {

    template<typename R, typename F, typename C>
    class call;

    template<typename R, typename F, typename C>
    coroutine<R,F,C>& derived(call<R,F,C> *this_) {
	return *static_cast<typename T::derived*>(this_);
    }

    template<typename R, typename P, typename C>
    class call {
      typedef coroutine<R, P, C> derived;
      typedef BOOST_DEDUCED_TYPENAME derived::arg1_type arg1_type;
      typedef BOOST_DEDUCED_TYPENAME derived::result_type result_type;
    public:
      result_type
      operator()(arg1_type x) const {
        boost::optional<result_type> result;
        derived(this)->bind_arg(x);
        derived(this)->bind_result(result);
        derived(this)->call();
        
        if(result) return *result;
        else { throw }; //XXX complete
      }

      boost::optional<result_type>
      operator()(arg1_type x, std::notrow_t) const {
        boost::optional<result_type> result;
        derived(this)->bind_arg(x);
        derived(this)->bind_result(result);
        derived(this)->call();        
        return result;
      }
    };
 
    template<typename R, typename C>
    class call<R, void, C> {
      typedef BOOST_DEDUCED_TYPENAME derived::result_type result_type;
    public:
     result_type
      operator()() const {
        boost::optional<result_type> result;
        derived(this)->bind_result(result);
        derived(this)->call();
        
        if(result) return *result;
        else { throw }; //XXX complete
      }

      boost::optional<result_type>
      operator()(arg1_type x, std::notrow_t) const {
        boost::optional<result_type> result;
        derived(this)->bind_arg(x);
        derived(this)->bind_result(result);
        derived(this)->call();        
        return result;
      }
 

    };

    template<typename C>
    class call<void, void, C> {
      typedef coroutine<void, void, C> derived;
    public:
      void
      operator()() const {
	static_cast<const derived*>(this)
	  ->call_impl<true, void_, void_>();
      }

      bool
      operator()(std::nothrow_t) const {
	static_cast<const derived*>(this)
	  ->call_impl<false, bool_, void>();
      }

    private:
    };

  }

  
  /// Returns a boolean <tt>Integral Constant c</tt> such that
  /// <tt>c::value == true</tt> if and only if @c X is a coroutine type.
  template<typename T>
  struct is_coroutine : boost::mpl::false_{};
  
  template<typename R, typename P, typename C>
  struct is_coroutine< coroutine<R,P,C> > : boost::mpl::true_{};
  /// }@

  /**
   * Make a stackful coroutine from a function object.
   * 
   * @tparam Result result type of the coroutine. It models  @c Copyable
   * or is @c void or is a reference.
   *
   * @tparam Param the coroutine will accept parameters of this
   * type. Models @c Copyable or @c void (Defaults to @c void)
   */
  template<typename Result, typename Parm = void, 
           typename ContextImpl = detail::default_context_impl>
  class coroutine
    : public movable<coroutine<Signature, Parm, ContextImpl> > 
    , detail::call<Result, Parm, ContextImpl>
  {
    detail::call<Result, Parm, ContextImpl> super;
    friend struct detail::coroutine_accessor;

    typedef detail::coroutine_impl<coroutine, ContextImpl> impl_type;

    /// @internal
    /// type of coroutine parameter.
    using BOOST_DEDUCED_TYPENAME super::arg_slot_type;
    using BOOST_DEDUCED_TYPENAME super::result_slot_type;
  public:
    using super::operator();

    /// argument type of coroutine @c operator().
    typedef BOOST_DEDUCED_TYPENAME boost::call_traits<parm_type>::type arg1_type;

    /// actual continuation implementation.
    typedef ContextImpl context_type;

    /// Result type of coroutine @c operator().
    typedef Result result_type;

    /// Type of 
    typedef detail::yelder_impl<coroutine> yelder;

    /// Construct an empty coroutine.
    coroutine() : m_pimpl() {}

    /// Move construct coroutine from @c src.
    /// @param src is an coroutine rvalue.
    ///
    /// @post @p src continuation (if any) is moved into @this; 
    /// <tt> src.empty() == true</tt>.
    coroutine(move_from<coroutine> src) 
      : m_pimpl(src->m_pimpl) {
      src->m_pimpl = 0;
    }

    /// Construct a coroutine over @p f.
    ///
    /// @param f models @c Callable. In addition
    /// <tt>result_of<F(yelder_type&)>::type</tt> must be a valid
    /// expression.
    ///
    /// @note the result of @p f is ignored, thus it can be of any
    /// type.
    /// 
    template<typename Functor>
    explicit
    coroutine ( 
      Functor f
    , std::ptrdiff_t stack_size = impl_type::default_stack_size
    , BOOST_DEDUCED_TYPENAME boost::enable_if<
    boost::mpl::and_<
    detail::is_callable<Functor>
    , boost::mpl::not_<is_coroutine<Functor> >
    > >::type * = 0)
      : m_pimpl(impl_type::create(f, stack_size)) 
    {}
 

    /// Move assign coroutine from @c rhs
    /// @param rhs is a coroutine rvalue
    ///
    /// @post @p rhs continuation (if any) is moved into @this; 
    /// <tt> rhs.empty() == true</tt>.   
    coroutine& operator=(move_from<coroutine> rhs) {
      coroutine(rhs).swap(*this);
      return *this;
    }

    /// Member swap.
    coroutine& swap(coroutine& rhs) {
      std::swap(m_pimpl, rhs.m_pimpl);
      return *this;
    }

    /// Free function swap.
    friend
    void swap(coroutine& lhs, coroutine& rhs) {
      lhs.swap(rhs);
    }

    typedef void(coroutine::*bool_type)();
    operator bool_type() const {
      return good()? &coroutine::bool_type_f: 0;
    }

    bool operator==(const coroutine& rhs) {
      return m_pimpl == rhs.m_pimpl;
    }

    void exit() {
      BOOST_ASSERT(m_pimpl);
      m_pimpl->exit();
    }

    bool exited() const {
      BOOST_ASSERT(m_pimpl);
      return m_pimpl->exited();
    }

    bool empty() const {
      return m_pimpl == 0;
    }
  protected:

    // The second parameter is used to avoid calling this constructor
    // by mistake from other member functions (specifically operator=).
    coroutine(impl_type * pimpl, detail::init_from_impl_tag) :
      m_pimpl(pimpl) {}

    void bool_type_f() {}

    bool good() const  {
      return !empty() && !exited() && !waiting();
    }

    result_type call_impl(arg_slot_type args) {
      BOOST_ASSERT(m_pimpl);
      m_pimpl->bind_args(&args);
      result_slot_type * ptr;
      m_pimpl->bind_result_pointer(&ptr);
      m_pimpl->invoke();

      return detail::fix_result<result_slot_traits>(*m_pimpl->result());
    }

    BOOST_DEDUCED_TYPENAME
    detail::optional_result_type<result_type>::type 
    call_impl_nothrow(arg_slot_type args) {
      BOOST_ASSERT(m_pimpl);
      m_pimpl->bind_args(&args);
      result_slot_type * ptr;
      m_pimpl->bind_result_pointer(&ptr);
      if(!m_pimpl->wake_up())
        return detail::optional_result<result_type>();

      return detail::fix_result<result_slot_traits>(*m_pimpl->result());
    }

    pimpl_type m_pimpl;

  };
} }
#endif
