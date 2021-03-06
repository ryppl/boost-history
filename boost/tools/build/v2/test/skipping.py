#!/usr/bin/python

# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

#  Test that V2 does not fail gracelessy when any target is skipped.
from BoostBuild import Tester, List

# Create a temporary working directory
t = Tester()
t.write("a.cpp", """ 
int main() {} 
""")

t.write("b.cpp", """ 
int main() {} 
""")

t.write("c.cpp", """ 
int main() {} 
""")

t.write("Jamfile", """ 
import feature : feature ;

feature foo : 1 2 : link-incompatible ;

exe a : a.cpp : <foo>1 ;
exe b : b.cpp : <foo>2 ;
exe c : c.cpp ; 
""")

t.write("project-root.jam", """ 
""")

t.run_build_system("foo=1")

t.cleanup()

