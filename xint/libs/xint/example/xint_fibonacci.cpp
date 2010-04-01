// xint_fibonacci.cpp

// Copyright Chad Nelson 2010
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*! 
  \file     
  \brief Simple demo of xint sending the fibonacci sequence to stream output.
  \details

  Fibonacci numbers are a sequence of numbers, starting with the numbers 0 and 1,
  where each successive number is the sum of the previous two.

  http://en.wikipedia.org/wiki/Fibonacci_number 

The first few Fibonacci numbers are:

    0 1 1 2 3 5 8 13 21 34 55 89 144...

You might be curious what the first Fibonacci number is that's too big to calculate
using the standard C++ unsigned long int type. 
A brute-force method of calculating this would be problematic, 
since it might not be possible to tell when it overflows.
But with the XInt library, it's child's play.

If you're not intimately familiar with the Fibonacci sequence,
and how fast the numbers in it grow, the answer might shock you.

As you can see, using the XInt library is just like using the native integer types 
-- except that an xint::integer won't overflow on you
until you completely exhaust your system's memory. :-)

  Output:
  \verbatim
 
0: 0
1: 1
2: 1
3: 2
4: 3
5: 5
6: 8
7: 13
8: 21
9: 34
10: 55
11: 89
12: 144
13: 233
14: 377
15: 610
16: 987
17: 1597
18: 2584
19: 4181
20: 6765
21: 10946
22: 17711
23: 28657
24: 46368
25: 75025
26: 121393
27: 196418
28: 317811
29: 514229
30: 832040
31: 1346269
32: 2178309
33: 3524578
34: 5702887
35: 9227465
36: 14930352
37: 24157817
38: 39088169
39: 63245986
40: 102334155
41: 165580141
42: 267914296
43: 433494437
44: 701408733
45: 1134903170
46: 1836311903
47: 2971215073
The 1st Fibonacci member > 4294967295 is number 48, with value 4807526976.

  \endverbatim


*/
#include <iostream>
#include <limits>

#include <boost/xint/xint.hpp>
// <boost/xint.hpp> ???

int main()
{
    using xint::integer;
    using std::cout;
    using std::endl;
    using std::numeric_limits;

/*
    //[xint_fibonacci_snippet_1

  [@http://en.wikipedia.org/wiki/Fibonacci_number  Fibonacci numbers]
are a sequence of numbers, starting with the numbers 0 and 1,
  where each successive number is the sum of the previous two.

The first few Fibonacci numbers are:

    0 1 1 2 3 5 8 13 21 34 55 89 144...

You might be curious what the first Fibonacci number is that's too big to calculate
using the standard C++ unsigned long int type. 
A brute-force method of calculating this would be problematic, 
since it might not be possible to tell when it overflows.
But with the XInt library, it's child's play.

If you're not intimately familiar with the Fibonacci sequence,
and how fast the numbers in it grow, the answer might shock you.

As you can see, using the XInt library is just like using the native integer types 
-- except that an xint::integer won't overflow on you
until you completely exhaust your system's memory. :-)

//] [xint_fibonacci_snippet_1]
*/

  // Select our limit, in this case the largest number 
  // that will fit into an unsigned long:
  integer limit = (std::numeric_limits<unsigned long>::max)();

  // Set the first two numbers in the sequence:
  integer n; //   (= 0 by default).
  integer n1=0; // to hold the previous Fibonacci number.
  integer n2=1; // to hold the new Fibonacci number.


    // Now count the items in the sequence as we iterate over them,
    // until we come to the first one greater than the limit we've set.
    size_t counter=0;
    cout << counter++ << ": " << n1 << endl;
    cout << counter++ << ": " << n2 << endl;

    while (true)
    {
        n = n1 + n2; // 
        if (n > limit) break;
        // That one didn't do it, go back for another try.
        cout << counter++ << ": " << n << endl;

        n1 = n2; 
        n2 = n;
    }
//[xint_fibonacci_snippet_2
//` Finally we print the one that we could not compute using a unsigned long int.
    cout << "The 1st Fibonacci member > " << limit << " is number " << counter << ", with value " << n << "." << endl;
//] [xint_fibonacci_snippet_2]

} // int main()


/*
//[xint_fibonacci_output_1

0: 0
1: 1
2: 1
3: 2
4: 3
5: 5
6: 8
7: 13
8: 21
9: 34
10: 55
11: 89
12: 144
13: 233
14: 377
15: 610
16: 987
17: 1597
18: 2584
19: 4181
20: 6765
21: 10946
22: 17711
23: 28657
24: 46368
25: 75025
26: 121393
27: 196418
28: 317811
29: 514229
30: 832040
31: 1346269
32: 2178309
33: 3524578
34: 5702887
35: 9227465
36: 14930352
37: 24157817
38: 39088169
39: 63245986
40: 102334155
41: 165580141
42: 267914296
43: 433494437
44: 701408733
45: 1134903170
46: 1836311903
47: 2971215073
The 1st Fibonacci member > 4294967295 is number 48, with value 4807526976.

//] [xint_fibonacci_output_1]
*/

