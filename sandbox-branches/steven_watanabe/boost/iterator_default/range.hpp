#ifndef BOOST_ITERATOR_DEFAULT_HPP_LJE20031017
#define BOOST_ITERATOR_DEFAULT_HPP_LJE20031017
#include <vector> 
#include <cstddef>

namespace boost
{
namespace iterator_default
  //WHAT:
  //  provide class similar to boost::half_open_range
  //  but which has default CTOR and operator++
  //WHY:
  //  default CTOR needed by several "iterator"
  //  classes in managed_ptr namespace.
  //WHY_HERE:
  //  the classes provided could be used outside
  //  of the managed_ptr namespace; hence,
  //  it just seemed better that they be put
  //  in another namespace.
{
  template
    < typename Value
    >
  struct ptr_adaptor
    /**@class ptr_adaptor
     * @brief 
     *  Adapt a pointer to appear as iterator
     *
     *  (Useful for arg lists which include:
     *    iterator const &
     *  which T* isn't).
     */
    {
          typedef
        Value
      value_type
        ;
          typedef
        value_type&
      reference
        ;
          typedef
        ptr_adaptor<value_type>
      my_type
        ;
      ptr_adaptor
        ( value_type* a_ptr=0
        )
        : m_ptr(a_ptr)
        {}
        reference
      deref(void)const
        { return *m_ptr
        ;}
        void
      increment(void)
        { ++m_ptr
        ;}
        reference
      operator*(void)const
        { return deref()
        ;}
        void
      operator++(void)
        { increment()
        ;}
        my_type const&
      operator+(unsigned n)
        { while(n>0)
          { increment()
          ; --n
          ;}
        ; return *this
        ;}
        bool
      operator==(my_type const& a_iter)const
        { return m_ptr == a_iter.m_ptr
        ;}
        bool
      operator!=(my_type const& a_iter)const
        { return !(operator==(a_iter))
        ;}
    private:
        value_type*
      m_ptr
        ;
    };//end ptr_adaptor struct
  template
    < typename Iterator
    >
  struct range
    /**@class range
     * @brief 
     *  iterator over a "range" of elements
     *
     *  end-of-range indicated by empty method.
     */
    {
          typedef
        Iterator
      iterator_type
        ;
          typedef
        range<iterator_type>
      my_type
        ;
          typedef
        typename std::iterator_traits<iterator_type>::value_type
      value_type
         ;
          typedef
        typename std::iterator_traits<iterator_type>::reference
      reference
        ;
      range
        ( iterator_type const& a_beg
        , iterator_type const& a_end
        )
        : m_cur(a_beg)
        , m_end(a_end)
        {}
      range
        ( void
        )
        : m_cur()
        , m_end(m_cur)
        {}
        reference
      deref(void)const
        { return *m_cur
        ;}
        void
      increment(void)
        { ++m_cur
        ;}
        std::size_t
      size(void)const
        { my_type mt(*this)
        ; std::size_t n=0
        ; for(;!mt.empty(); mt.increment(),++n)
        ; return n
        ;}
        bool
      empty(void)const
        { return m_cur == m_end
        ;}
        void
      cur_set(iterator_type const& a_cur)
        { m_cur = a_cur
        ;}
    private:
        iterator_type
      m_cur
        ;
        iterator_type
      m_end
        ;
    };//end range<Iterator> general template
    
}//exit iterator_default namespace
}//exit boost namespace
#endif
