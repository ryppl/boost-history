#ifndef BOOST_ALGORITHM_IS_POINTER_TO_HPP
#define BOOST_ALGORITHM_IS_POINTER_TO_HPP

namespace boost { namespace algorithm { namespace detail {
    
    template <class T, class U> struct is_pointer_to :
    boost::mpl::and_<
        typename boost::is_pointer<T>,
        typename boost::is_same<
        typename boost::remove_cv<typename boost::remove_pointer<T>::type>::type,
        U>
    > {};

} } }

#endif // BOOST_ALGORITHM_IS_POINTER_TO_HPP
