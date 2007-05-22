# Copyright 2007 Stjepan Rajko.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

$template_start$
$template_python_copyright$

import testing ;

project
    : requirements 
      <include>../../..
      <library>/boost/test//boost_unit_test_framework/<link>static
      <define>BOOST_ALL_NO_LIB=1
    ;
    
test-suite $template_library$
    : [ run test.cpp ]
    ;
