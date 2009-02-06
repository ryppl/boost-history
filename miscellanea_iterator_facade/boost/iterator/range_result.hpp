//////////////////////////////////////////////////////////////////////////////
// range_result.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_RANGE_RESULT_HPP_ER_2009
#define BOOST_ITERATOR_RANGE_RESULT_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/type_traits.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{

struct range_result{

    template<class Signature>
    struct result {
        typedef boost::function_traits<Signature> fun_traits;
        typedef typename fun_traits::arg1_type              arg1_type;

        typedef typename boost::remove_reference<
        arg1_type>::type                                    arg_type;
        typedef sub_range<arg_type>                         type;
    };
};

}
#endif
