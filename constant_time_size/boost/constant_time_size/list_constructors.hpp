#ifndef BOOST_CONSTANT_TIME_SIZE_LIST_CONSTRUCTORS_HPP_
#define BOOST_CONSTANT_TIME_SIZE_LIST_CONSTRUCTORS_HPP_


#define BOOST_CONSTANT_TIME_SIZE_CONSTRUCTORS(list, super_type, container) \
    explicit                                                                \
    list(const allocator_type& __a = allocator_type())                      \
    : super_type(__a) { }                                                   \
                                                                            \
    list(size_type __n, const value_type& __value,                          \
       const allocator_type& __a = allocator_type())                        \
    : super_type(__n, __value, __a)                                         \
    { }                                                                     \
                                                                            \
    explicit                                                                \
    list(size_type __n)                                                     \
    : super_type(__n)                                                       \
    { }                                                                     \
                                                                            \
    template <typename C>                                                   \
    explicit                                                                \
    list(const C& __x, int***)                                                      \
    : super_type(__x)                                                       \
    {  }                                                                    \
                                                                            \
    template<typename _InputIterator>                                       \
    list(_InputIterator __first, _InputIterator __last,                     \
       const allocator_type& __a = allocator_type())                        \
    : super_type(__first, __last, __a)                                      \
    { }                                                                     \
                                                                            \
    template <typename C>                                                   \
    list&                                                                   \
    operator=(const C& __x)                                                 \
    {                                                                       \
           super_type::operator=(__x);                                      \
        return *this;                                                       \
    }                                                                       \
                                                                            \
    list(const container& __x)                                              \
    : super_type(__x)                                                       \
    {  }                                                                    \
                                                                            \
    list(const container& __x, size_type __d)                               \
    : super_type(__x, __d)                                                  \
    {  }                                                                    \
                                                                            \
    template<typename _InputIterator>                                       \
    list(_InputIterator __first, _InputIterator __last, size_type __d,      \
       const allocator_type& __a = allocator_type())                        \
    : super_type(__first, __last, __d, __a)                                 \
    { }                                                                     \


#endif // BOOST_CONSTANT_SIZE_LIST_CONSTRUCTORS_HPP_
