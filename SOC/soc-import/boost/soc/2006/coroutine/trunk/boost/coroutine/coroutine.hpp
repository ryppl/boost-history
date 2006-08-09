//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_COROUTINE_HPP_20060512
#define BOOST_COROUTINE_COROUTINE_HPP_20060512
#include <cstddef>
#include <boost/preprocessor/repetition.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/coroutine/detail/arg_max.hpp>
#include <boost/coroutine/detail/coroutine_impl.hpp>
#include <boost/coroutine/detail/default_context_impl.hpp>
#include <boost/coroutine/detail/is_callable.hpp>
#include <boost/coroutine/detail/argument_packer.hpp>
#include <boost/coroutine/detail/argument_unpacker.hpp>
#include <boost/coroutine/detail/signature.hpp>
#include <boost/coroutine/detail/index.hpp>
#include <boost/coroutine/detail/coroutine_traits.hpp>
#include <boost/coroutine/detail/coroutine_accessor.hpp>
#include <boost/coroutine/move.hpp>
#include <boost/coroutine/detail/fix_result.hpp>
#include <boost/coroutine/detail/self.hpp>

namespace boost { namespace coroutines {
  template<typename Signature, typename Context>
  class coroutine;

  namespace detail {
    template<typename T>
    struct is_coroutine : boost::mpl::false_{};

    template<typename Sig, typename Con>
    struct is_coroutine<coroutine<Sig, Con> > : boost::mpl::true_{};
  }


  template<typename Signature, 
	   typename ContextImpl = detail::default_context_impl>
  class coroutine : public movable<coroutine<Signature, ContextImpl> > {
  public:
    typedef coroutine<Signature, ContextImpl> type;
    typedef ContextImpl context_impl;
    typedef Signature signature_type;
    friend class detail::coroutine_accessor;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::result_type result_type;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::result_slot_type result_slot_type;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::yield_result_type yield_result_type;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::result_slot_traits result_slot_traits;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::arg_slot_type arg_slot_type;

    typedef BOOST_DEDUCED_TYPENAME detail::coroutine_traits<signature_type>
    ::arg_slot_traits arg_slot_traits;
        
    typedef detail::coroutine_impl<type, context_impl> impl_type;
    typedef BOOST_DEDUCED_TYPENAME  impl_type::pointer impl_ptr;  
   
    typedef detail::coroutine_self<type> self;
    coroutine(): m_pimpl(0) {}

  private:
  public:

    
    template<typename Functor>
    explicit 
    coroutine (Functor f, 
	       std::ptrdiff_t stack_size = detail::default_stack_size,
	       BOOST_DEDUCED_TYPENAME boost::enable_if<
	       boost::mpl::and_<
	       detail::is_callable<Functor>, 
	       boost::mpl::not_<detail::is_coroutine<Functor> >
	       > >
	       ::type * = 0
	       ) :
      m_pimpl(impl_type::create(f, stack_size)) {}
 
    coroutine(move_from<coroutine> src) 
      : m_pimpl(src->m_pimpl) {}

    coroutine& operator=(move_from<coroutine> src) {
      coroutine(src).swap(*this);
      return *this;
    }

    coroutine& swap(coroutine& rhs) {
      std::swap(m_pimpl, rhs.m_pimpl);
      return *this;
    }

    friend
    inline 
    void swap(coroutine& lhs, coroutine& rhs) {
      lhs.swap(rhs);
    }

#   define BOOST_COROUTINE_generate_argument_n_type(z, n, traits_type) \
    typedef BOOST_DEDUCED_TYPENAME traits_type ::template at<n>::type  \
    BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type);                         \
    /**/

    BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		    BOOST_COROUTINE_generate_argument_n_type,
		    arg_slot_traits);

    static const int arity = arg_slot_traits::length;

    struct yield_traits {
      BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		      BOOST_COROUTINE_generate_argument_n_type,
		      result_slot_traits);
      static const int arity = result_slot_traits::length;
    };

#   undef BOOST_COROUTINE_generate_argument_n_type
   
#   define BOOST_COROUTINE_param_with_default(z, n, type_prefix)    \
    BOOST_DEDUCED_TYPENAME call_traits                              \
    <BOOST_PP_CAT(BOOST_PP_CAT(type_prefix, n), _type)>::param_type \
    BOOST_PP_CAT(arg, n) =                                          \
    BOOST_PP_CAT(BOOST_PP_CAT(type_prefix, n), _type)()             \
/**/

    result_type operator()
      (BOOST_PP_ENUM
       (BOOST_COROUTINE_ARG_MAX,
	BOOST_COROUTINE_param_with_default,
	arg)) {
      return call_impl
	(arg_slot_type(BOOST_PP_ENUM_PARAMS
	  (BOOST_COROUTINE_ARG_MAX, 
	   arg)));
      }
#undef BOOST_COROUTINE_param_typedef
#undef BOOST_COROUTINE_param_with_default

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
  private:
    //Disabled copy constructor and operator=. 
    //Coroutine is movable only.
    //coroutine(const coroutine& rhs);
    //coroutine& operator=(coroutine rhs);

    /**
     * The second parameter is used to avoid calling this constructor
     * by mistake from other member funcitons (specifically operator=).
     */
    coroutine(impl_type * pimpl, detail::init_from_impl_tag) :
      m_pimpl(pimpl) {}

    void bool_type_f() {}

    bool good() const  {
      return m_pimpl && !m_pimpl->exited();
    }

    result_type call_impl(arg_slot_type args) {
      BOOST_ASSERT(m_pimpl);

      result_slot_type result;
      m_pimpl->bind_args(&args);
      m_pimpl->bind_result(&result);
      m_pimpl->invoke();
      return detail::fix_result<result_slot_traits>(result);
    }

    impl_ptr m_pimpl;

    void acquire() {
      m_pimpl->acquire();
    }

    void release() {
      m_pimpl->release();
    }

    impl_type& get_impl() {
      return *m_pimpl;
    }
  };
} }
#endif
