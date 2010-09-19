//  complex.cpp  ----------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes
//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/type_traits/common_type.hpp>
#include <iostream>
#include <complex>

template <class T, class U>
typename boost::common_type<std::complex<T>, std::complex<U> >::type
operator+(std::complex<T>, std::complex<U>);

template <class T, class U>
std::complex<typename boost::common_type<T, U>::type>
operator-(std::complex<T>, std::complex<U>);


int main()
{
    
  return 0;
}
