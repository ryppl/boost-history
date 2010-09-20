//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//  Adapted by Vicente J. Botet Escriba test Boost.Chrono
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//
//===----------------------------------------------------------------------===//

// <chrono>

// template <class Rep1, class Period1, class Rep2, class Period2>
// struct common_type<chrono::duration<Rep1, Period1>, chrono::duration<Rep2, Period2>>
// {
//     typedef chrono::duration<typename common_type<Rep1, Rep2>::type, see below }> type;
// };

#include <boost/chrono.hpp>
#include <boost/type_traits/is_same.hpp>

#if !defined(BOOST_NO_STATIC_ASSERT) 
#define NOTHING ""
#endif

template <class D1, class D2, class De>
void
test()
{
    typedef typename boost::common_type<D1, D2>::type Dc;
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<Dc, De>::value), NOTHING, ());
}

int main()
{
    test<boost::chrono::duration<int, boost::ratio<1, 100> >,
         boost::chrono::duration<long, boost::ratio<1, 1000> >,
         boost::chrono::duration<long, boost::ratio<1, 1000> > >();
    test<boost::chrono::duration<long, boost::ratio<1, 100> >,
         boost::chrono::duration<int, boost::ratio<1, 1000> >,
         boost::chrono::duration<long, boost::ratio<1, 1000> > >();
    test<boost::chrono::duration<char, boost::ratio<1, 30> >,
         boost::chrono::duration<short, boost::ratio<1, 1000> >,
         boost::chrono::duration<int, boost::ratio<1, 3000> > >();
    test<boost::chrono::duration<double, boost::ratio<21, 1> >,
         boost::chrono::duration<short, boost::ratio<15, 1> >,
         boost::chrono::duration<double, boost::ratio<3, 1> > >();
}
