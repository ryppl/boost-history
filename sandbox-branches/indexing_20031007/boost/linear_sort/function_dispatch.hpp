#ifndef BOOST_LINEAR_SORT_FUNCTION_DISPATCH
#define BOOST_LINEAR_SORT_FUNCTION_DISPATCH

namespace boost {
  namespace detail {

  //==============================
  // dispatch utility
  //==============================
  // Explanation: we need to guess the value type of the iterator in
  // order to bypass BOOST_NO_STD_ITERATOR_TRAITS, but it is not possible
  // to use partial specialization with functions, and in any case it is
  // possible to avoid partial specialization so we shouldn't use it
  // (to avoid workarounds for buggy compilers).
  // Instead we will rely on function overloading and  the fact that
  // it's always possible to dereference [first,last) to obtain the
  // value type

    
    // in order to specialize on the type of the argument
    // we could simply use (T const&), but that would involve a
    // default constructor of T, with possible side-effects
    // so we wrap T into an empty structure
    template <class T> struct linear_sort_function_dispatch {};

    template <class T>
    linear_sort_function_dispatch<T>
    make_linear_sort_function_dispatch(T const&) {
      return linear_sort_function_dispatch<T>();
    }

  } // namespace detail
} // namespace boost

#endif // BOOST_LINEAR_SORT_FUNCTION_DISPATCH
