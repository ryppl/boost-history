//  Boost string_algo library is_pointer_to.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

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
