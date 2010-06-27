//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_HAS_COMPLEMENT_OPERATOR_HPP
#define BOOST_INTRO_DETAIL_HAS_COMPLEMENT_OPERATOR_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace boost{
namespace intro{
namespace detail{

template<class T>
struct has_complement_operator_helper : T{
    operator int();
};

typedef char complement_yes;
typedef int complement_no;

template<class T>
complement_yes complement_yesno(T const &);
complement_no complement_yesno(int);


template<class T>
struct has_complement_operator
    : mpl::bool_<
        sizeof(complement_yesno(~(*((has_complement_operator_helper<T> *)0))))
            == sizeof(complement_yes)
    >{};

}
}
}


#endif

