#ifndef BOOST_COROUTINE_DETAIL_FUTURE_IMPL_HPP_20060728
#define BOOST_COROUTINE_DETAIL_FUTURE_IMPL_HPP_20060728
#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/coroutine/detail/make_tuple.hpp>

namespace boost { namespace coroutines { namespace detail {
  template<typename TypeList>
  class future_impl {
    typedef TypeList type_list;
  public:
    typedef BOOST_DEDUCED_TYPENAME make_tuple<type_list>::type
    value_type;

  private:
    typedef boost::shared_ptr<boost::optional<value_type> > 
    optional_pointer;
    typedef void (future_impl::*safe_bool)();

  public:
    future_impl(const future_impl& rhs) :
      m_ptr(rhs.m_ptr) {}

    operator safe_bool() const {
      BOOST_ASSERT(m_tptr);
      return *m_ptr? &safe_bool_true, 0;
    }

    value_type& operator *() {
      BOOST_ASSERT(m_tptr);
      BOOST_ASSERT(m_tptr->get<0>());
      return **m_tptr;
    }

    const value_type& operator *() const{
      BOOST_ASSERT(m_tptr);
      BOOST_ASSERT(m_tptr->get<0>());
      return **m_tptr;
    }


  private:
    void assign(const value_type& val) {
      *m_ptr = val;
    }

    void safe_bool_true() {}

    optional_pointer m_ptr;    
  };

} } }
#endif
