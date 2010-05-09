/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <boost/itl/ptime.hpp>
#include <boost/itl/interval_set.hpp>
#include <boost/itl_xt/list.hpp>
#include <boost/validate/gentor/gentorprofile.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/laws/monoid.hpp>

using namespace std;
using namespace boost;
using namespace boost::itl;
using namespace boost::posix_time;

int main()
{
    typedef InplaceCommutativity<int> TestLawT;
    LawValidater<TestLawT> law_validator;

    //-----------------------------------------------------------------------------
    int test_count = 10000;
    ptime start, stop;

    GentorProfileSgl::it()->set_std_profile(8,1);
    law_validator.set_trials_count(test_count);

    cout << "Testing " << TestLawT().typeString()  << endl
         << "trials_count = " << test_count << endl
         << "Start ...\n";
    start = ptime(microsec_clock::local_time());
    law_validator.run();
    stop = ptime(microsec_clock::local_time());
    cout << "Stop. Time elapsed: " << stop - start << endl;

    if(law_validator.violation_count() == 0)
        cout << "Law successfully validated\n";

    return 0;
}
