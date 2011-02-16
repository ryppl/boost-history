/*-----------------------------------------------------------------------------+
Interval Container Library
Author: Joachim Faulhaber
Copyright (c) 2007-2011: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <iostream>

using namespace std;
//using namespace boost::posix_time;
//using namespace boost::icl;

int foo(int x)
{
	int bar(int x)
	{
		return x+x;
	}

	return bar(x);
}

int main()
{
	cout << foo(1) << endl;
    return 0;
}

