//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_CHECKING_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_CHECKING_ER_2010_HPP
#include <boost/assign/v2/ref/aux_/convert_traits/reference.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace convert_traits{
namespace checking{

    template<typename T1, typename T2, typename T3>
    struct tester{

        typedef typename convert_traits::reference<T1, T2>::type result_;

        static void test()
        {
            BOOST_MPL_ASSERT((boost::is_same<result_,T3>));
        }

   };

namespace same_value_type{

    template<typename T>
    void do_check()
    {

        tester<const T &, const T &, const T &>::test();
        tester<const T &,       T &, const T &>::test();
        tester<      T &, const T &, const T &>::test();
        tester<      T &,       T &,       T &>::test();

        tester<const T &, const T  , const T  >::test();
        tester<const T &,       T  , const T  >::test();
        tester<      T &, const T  , const T  >::test();
        tester<      T &,       T  ,       T  >::test();

        tester<const T ,  const T &, const T  >::test();
        tester<const T ,        T &, const T  >::test();
        tester<      T ,  const T &, const T  >::test();
        tester<      T ,        T &,       T  >::test();

        tester<const T ,  const T  , const T  >::test();
        tester<const T ,        T  , const T  >::test();
        tester<      T ,  const T  , const T  >::test();
        tester<      T ,        T  ,       T  >::test();

    }

}// twin_values


}// checking
}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_CHECKING_ER_2010_HPP
