//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_GENERATOR_HPP_20060812
#define BOOST_COROUTINE_GENERATOR_HPP_20060812
#include <iterator>
#include <boost/optional.hpp>
#include <boost/none.hpp>
#include <boost/coroutine/shared_coroutine.hpp>
namespace boost { namespace coroutines {

  namespace detail {
    template<typename T>
    class tag {};

    class empty {};

    template<typename T1, typename T2>
    struct make_std_iterator {
      typedef std::iterator<T1, T2> type;
    };
  }

  // This simple class implement generators (a simple
  // subset of coroutines) in the form of an InputIterator
  // interface. It also implem
  template<typename ValueType, 
	   typename Coroutine = 
	   shared_coroutine<ValueType()> >
  class generator : public boost::mpl::eval_if<boost::is_same<ValueType, void>,
					   boost::mpl::identity<detail::empty>,
					   detail::make_std_iterator<std::input_iterator_tag, ValueType> >::type{
    typedef void(generator::*safe_bool)();
    typedef ValueType internal_value_type;
  public:
    typedef Coroutine coroutine_type;
    typedef BOOST_DEDUCED_TYPENAME 
    coroutine_type::result_type result_type;
    typedef result_type value_type;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type::self self;

    generator() {}

    template<typename Functor>
    generator(Functor f) :
      m_coro(f),
      m_val(assign(detail::tag<result_type>())){};

    generator(const generator& rhs) :
      m_coro(rhs.m_coro),
      m_val(rhs.m_val) {}

    value_type operator*() {
      return *m_val;
    }

    generator& operator++() {
      m_val = assign(detail::tag<result_type>());
      return *this;
    }

    generator operator++(int) {
      generator t(*this);
      ++(*this);
      return t;
    }

    friend
    bool operator==(const generator& lhs, const generator& rhs) {
      if(lhs.m_val) return lhs.m_val == rhs.m_val;
      else if(rhs.m_val) return false;
      else return true;
    }

    friend 
    bool operator != (const generator& lhs, const generator & rhs) {
      return !(lhs == rhs);
    }

    operator safe_bool () const {
      return m_val? &generator::safe_bool_true: 0;
    }

    value_type operator()() {
      (*this)++;
      return *m_val;
    }

  private:
    void safe_bool_true () {};

    // hack to handle correctly void result types.
    struct optional_void {
      optional_void() : m_result(true) {}
      optional_void(boost::none_t) : m_result(false) {}

      void operator*() const {}
      operator bool() const { return m_result; };
      bool m_result;
    };

    typedef BOOST_DEDUCED_TYPENAME
    boost::mpl::if_<boost::is_same<value_type, void>,
		    optional_void,
		    boost::optional<value_type> >::type optional_type;

    template<typename T>
    optional_type assign(detail::tag<T>) {
      return m_coro? m_coro(std::nothrow): boost::none;
    }

    optional_type assign(detail::tag<void>) {
      return m_coro? (m_coro(std::nothrow), optional_type()): boost::none;
    }

    // There is a possible EBO here. May be use compressed pair.
    coroutine_type m_coro;
    optional_type m_val;

  };

} }
#endif
