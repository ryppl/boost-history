#!/usr/bin/python

# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

#  Test that composite properties are handled correctly.
from BoostBuild import Tester, List

t = Tester()

t.write("project-root.jam", "")
t.write("Jamfile", """
exe hello : hello.cpp : <variant>release ;
""")
t.write("hello.cpp", """
int main()
{
    return 0;
}

""")

t.run_build_system()

t.expect_addition("bin/$toolset/release/hello.exe")


t.cleanup()
