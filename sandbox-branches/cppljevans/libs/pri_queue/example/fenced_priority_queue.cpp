/* fenced_priority_queue.cpp source file
 *
 * Copyright Cromwell D. Enage 2004.  Use, modification, and distribution are
 * subject to the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * Defines the std::ios class and std::cout, its global output instance.
 */
#include <iostream>

/*
 * Defines the boost::fenced_priority_queue class template.
 */
#include <boost/pending/fenced_priority_queue.hpp>

int main()
{
    typedef boost::fenced_priority_queue<unsigned int> FPQ;

    const unsigned int sample[] = {7, 13, 11, 32, 24, 48, 67, 84, 72};
    FPQ Q;

    Q.push(sample[0]);
    std::cout << "Inserted " << sample[0] << "." << std::endl;
    Q.push(sample[1]);
    std::cout << "Inserted " << sample[1] << "." << std::endl;
    Q.push(sample[2]);
    std::cout << "Inserted " << sample[2] << "." << std::endl;

    /*
     * Implicit fence.
     */
    std::cout << "Q.top = " << Q.top() << "." << std::endl;

    Q.push(sample[3]);
    std::cout << "Inserted " << sample[3] << "." << std::endl;
    Q.push(sample[4]);
    std::cout << "Inserted " << sample[4] << "." << std::endl;
    Q.push(sample[5]);
    std::cout << "Inserted " << sample[5] << "." << std::endl;

    /*
     * Implicit fence.
     */
    std::cout << "Q.top = " << Q.top() << "." << std::endl;

    Q.push(sample[6]);
    std::cout << "Inserted " << sample[6] << "." << std::endl;
    Q.push(sample[7]);
    std::cout << "Inserted " << sample[7] << "." << std::endl;
    Q.push(sample[8]);
    std::cout << "Inserted " << sample[8] << "." << std::endl;

    /*
     * Implicit fence.
     */
    std::cout << "Q.top = " << Q.top() << "." << std::endl;

    /*
     * Output.
     */
    std::cout << "Q =";

    while (!Q.empty())
    {
        std::cout << " " << Q.top();
        Q.pop();
    }

    std::cout << std::endl;

    return 0;
}

