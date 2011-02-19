/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <boost/config/warning_disable.hpp>
#include <iostream>
#include <stdio.h>
#include <boost/thread.hpp> 
#include <boost/validate/driver/abelian_monoid_driver.hpp>

using namespace std;
using namespace boost;
using namespace boost::icl;


void wait(int seconds) 
{ 
  boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
} 

boost::mutex g_Mutex; 

void test_abelian_monoid(int size)
{
    abelian_monoid_driver model_tester;
    g_Mutex.lock();
    cout << 
    ">> ------------------------------------------------------ <<\n"
    ">> -------- Law based test automaton 'LaBatea' ---------- <<\n"
    ">> Output will be generated in a few seconds\n"
    ">> terminate by typing <CTRL>C\n"
    ">> ------------------------------------------------------ <<\n";
    GentorProfileSgl::it()->set_std_profile(size,1);
    GentorProfileSgl::it()->report_profile();
    g_Mutex.unlock();
    wait(1);

    model_tester.terminate_at_law_count(18, 100);

    model_tester.validate();
}

void tread_abelian_monoid_1()
{
    test_abelian_monoid(16);
}

void tread_abelian_monoid_2()
{
    test_abelian_monoid(77);
}

void thread_() 
{ 
  for (int i = 0; i < 5; ++i) 
  { 
    wait(1); 
    g_Mutex.lock(); 
    std::cout << "Thread " << boost::this_thread::get_id() << ": " << i << std::endl; 
    g_Mutex.unlock(); 
  } 
} 

int main() 
{ 
  boost::thread t1(tread_abelian_monoid_1); 
  wait(2);
  boost::thread t2(tread_abelian_monoid_2); 
  t1.join(); 
  t2.join(); 
} 


/*CL
int main()
{
    abelian_monoid_driver model_tester;
    cout << 
    ">> ------------------------------------------------------ <<\n"
    ">> -------- Law based test automaton 'LaBatea' ---------- <<\n"
    ">> Output will be generated in a few seconds\n"
    ">> terminate by typing <CTRL>C\n"
    ">> ------------------------------------------------------ <<\n";
    GentorProfileSgl::it()->set_std_profile(16,1);
    GentorProfileSgl::it()->report_profile();
    model_tester.validate();
    return 0;
}
*/

