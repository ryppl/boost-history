// Copyright David Abrahams 2004. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BINARY_DWA2004620_HPP
# define BINARY_DWA2004620_HPP

template <unsigned long N> 
struct binary
{
    static unsigned const value
       = binary<N/10>::value << 1   // prepend higher bits
         | N%10;                    // to lowest bit
}; 

template <>                           // specialization
struct binary<0>                      // terminates recursion
{
    static unsigned const value = 0;
};


#endif // BINARY_DWA2004620_HPP
