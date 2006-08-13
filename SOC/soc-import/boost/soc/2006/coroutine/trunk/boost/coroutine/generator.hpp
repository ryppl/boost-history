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

  // This simple class implement generators (a simple
  // subset of coroutines) in the form of an InputIterator
  // interface. It also implem
  template<typename ValueType>
  class generator : public std::iterator<std::input_iterator_tag, ValueType> {
    typedef void(generator::*safe_bool)();
    typedef ValueType internal_value_type;
  public:
    typedef shared_coroutine<internal_value_type()> coroutine_type;
    typedef BOOST_DEDUCED_TYPENAME 
    coroutine_type::result_type result_type;
    typedef result_type value_type;

    typedef BOOST_DEDUCED_TYPENAME coroutine_type::self self;

    generator() {}

    template<typename Functor>
    generator(Functor f) :
      m_coro(f),
      m_val(assign()){};

    generator(const generator& rhs) :
      m_coro(rhs.m_coro),
      m_val(rhs.m_val) {}

    value_type operator*() {
      return *m_val;
    }

    generator& operator++() {
      m_val = assign();
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
      return *(*this)++;
    }
  private:
    void safe_bool_true () {};

    boost::optional<value_type> assign() {
      return m_coro? m_coro(std::nothrow) : boost::none;
    }

    coroutine_type m_coro;
    boost::optional<value_type> m_val;
  };

} }
#endif
