#ifndef BOOST_CONSTANT_TIME_SIZE_WRAPPER_LIST_STL_H_
#define BOOST_CONSTANT_TIME_SIZE_WRAPPER_LIST_STL_H_


namespace boost {
namespace constant_time_size {

template < class List > 
class extension_list : public List 
{
    typedef List super_type;
      template<typename> friend struct boost::constant_time_size::non_const_cont;

public:
    typedef List                container;
    typedef typename  container::value_type               value_type;
    typedef typename  container::pointer                  pointer;
    typedef typename  container::const_pointer            const_pointer;
    typedef typename  container::reference                reference;
    typedef typename  container::const_reference          const_reference;
    typedef typename  container::iterator                 iterator;
    typedef typename  container::const_iterator           const_iterator;
    typedef typename  container::const_reverse_iterator   const_reverse_iterator;
    typedef typename  container::reverse_iterator         reverse_iterator;
    typedef typename  container::size_type                size_type;
    typedef typename  container::difference_type          difference_type;
    typedef typename  container::allocator_type           allocator_type;
    typedef boost::constant_time_size::non_const_cont<extension_list<List> >      non_const;

      operator non_const()
      {
          return non_const(*this);
      }

    explicit 
    extension_list(const allocator_type& __a = allocator_type())
    : super_type(__a) { }

    extension_list(size_type __n, const value_type& __value,
       const allocator_type& __a = allocator_type())
    : super_type(__n, __value, __a)
    {
        std::cout << "===22 "<<__LINE__ << " " <<  this->size() << std::endl;
    }

    explicit
    extension_list(size_type __n)
    : super_type(__n)
    { }

    template <typename C>
    explicit
    extension_list(const C& __x)
    : super_type(__x)
    {  }

    template<typename _InputIterator>
    extension_list(_InputIterator __first, _InputIterator __last,
       const allocator_type& __a = allocator_type())
    : super_type(__first, __last, __a)
    {
        std::cout << "===41 "<<__LINE__ << " " <<  this->size() << std::endl; 
    }

    extension_list(const container& __x)
    : super_type(__x)
    {  }

    template<typename _InputIterator>
    extension_list(_InputIterator __first, _InputIterator __last, size_type,
       const allocator_type& __a = allocator_type())
    : super_type(__first, __last, __a)
    { }

    extension_list(const container& __x, size_type)
    : super_type(__x)
    {  }

    template <typename C>
    extension_list&
    operator=(const C& __x)
    {
           super_type::operator=(__x);
        return *this;
    }

    template<typename _InputIterator>
      void
      insert(iterator __position, _InputIterator __first,
           _InputIterator __last, size_type __d)
      {
          super_type::insert(__position, __first, __last);
      }

      /**
       *  @brief  Insert range from another %list.
       *  @param  position  Iterator referencing the element to insert before.
       *  @param  x  Source list.
       *  @param  first  Iterator referencing the start of range in x.
       *  @param  last  Iterator referencing the end of range in x.
       *  @param  d  distance betwee first and last.
       *
       *  Removes elements in the range [first,last) and inserts them
       *  before @a position in constant time.
       *
       *  Undefined if @a position is in [first,last).
       */
      void
      splice(iterator __position, extension_list& __x, iterator __first, iterator __last, size_type __d)
      {
          super_type::splice(__position, __x, __first, __last);
      }

      void
      splice(iterator __position, container& __x)
      {
          super_type::splice(__position, __x);
      }
      void
      splice(iterator __position, container& __x, size_type __d)
      {
          super_type::splice(__position, __x);
      }
      
};


}
}
  
#endif
