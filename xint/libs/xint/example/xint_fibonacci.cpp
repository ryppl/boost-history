// xint_fibonacci.cpp

// Copyright Chad Nelson 2010
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
\file     
\brief Simple demo of xint sending the fibonacci sequence to stream output.
\details

Fibonacci numbers are a sequence of numbers, starting with the numbers 0 and 1, where each successive number is the sum of the previous two. The first few Fibonacci numbers are:

    0 1 1 2 3 5 8 13 21 34 55 89 144...

You might be curious what the first Fibonacci number is that's too big to calculate using the standard C++ unsigned long type. A brute-force method of calculating this would be problematic, since it might not be possible to tell when it overflows. But with the XInt library, it's child's play.

If you're not intimately familiar with the Fibonacci sequence,
and how fast the numbers in it grow, the answer might shock you.

As you can see, using the XInt library is just like using the native integer types 
-- except that an xint::integer won't overflow on you
until you completely exhaust your system's memory. :-)

**/
#include <iostream>
#include <limits>

#include <boost/xint/xint.hpp>

int main() {
    using xint::integer;
    using std::cout;
    using std::endl;
    using std::numeric_limits;

//[xint_fibonacci_snippet_1

// Select our limit, in this case the largest number 
// that will fit into an unsigned long:
  integer limit=(std::numeric_limits<unsigned long>::max)();

  // Set the first two numbers in the sequence:
  integer n, n1=0, n2=1;
//] [xint_fibonacci_snippet_1]

    // Now count the items in the sequence as we iterate over them,
    //  until we come to the first one greater than the limit we've set.
    size_t counter=0;
    cout << counter++ << ": " << n1 << endl;
    cout << counter++ << ": " << n2 << endl;

    while (true) {
        n = n1 + n2;
        if (n > limit) break;
        // That one didn't do it, go back for another try.
        cout << counter++ << ": " << n << endl;

        n1=n2;
        n2=n;
    }

    cout << "The answer is " << counter << " (" << n << ")." << endl;
} // int main()


/*
//[xint_fibonacci_output_1



//] [xint_fibonacci_output_1]
*/

