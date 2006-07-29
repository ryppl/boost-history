#ifndef BOOST_COROUTINE_DETAIL_FUTURE_IMPL_HPP_20060728
#define BOOST_COROUTINE_DETAIL_FUTURE_IMPL_HPP_20060728
#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/coroutine/detail/make_tuple_traits.hpp>
#include <boost/coroutine/detail/signal.hpp>

namespace boost { namespace coroutines { namespace detail {
  template<typename TypeList>
  class future_impl {
    friend class detail::wait_gateway;
    typedef TypeList type_list;
  public:
    typedef BOOST_DEDUCED_TYPENAME make_tuple_traits<type_list>::type
    tuple_traits_type;

    typedef BOOST_DEDUCED_TYPENAME tuple_traits_type::as_tuple tuple_type;
    typedef tuple_type value_type;

  private:
    template<typename T>
    struct opt_impl : public boost::optional<T> {
      opt_impl() : waited(false) {}
      opt_impl(const T& val) :
	boost::optional<T>(val),
	waited(false) {}
      bool waited;
    };

    typedef boost::shared_ptr<opt_impl<value_type> > 
    optional_pointer;

  public:
    future_impl() :
      m_ptr(new opt_impl<value_type>()) {}

    future_impl(const future_impl& rhs) :
      m_ptr(rhs.m_ptr) {}

    bool is_valid() const {
      BOOST_ASSERT(m_ptr);
      return *m_ptr;
    }

    value_type& operator *() {
      BOOST_ASSERT(m_ptr);
      BOOST_ASSERT(*m_ptr);
      return **m_ptr;
    }

    const value_type& operator *() const{
      BOOST_ASSERT(m_ptr);
      BOOST_ASSERT(*m_ptr);
      return **m_ptr;
    }

    future_impl&
    operator=(const value_type& rhs) {
      *m_ptr = rhs;
      return *this;
    }

  private:
    void mark_wait(bool how) {
      m_ptr->waited = how;
    }

    bool waited() const {
      return m_ptr->waited;
    }

    void assign(const value_type& val) {
      *m_ptr = val;
    }

    optional_pointer m_ptr;    
  };

} } }
#endif
