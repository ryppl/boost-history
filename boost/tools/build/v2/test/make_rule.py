#!/usr/bin/python

# Test the 'make' rule

from BoostBuild import Tester
from string import find

t = Tester()

t.write("project-root.jam", "")
t.write("Jamfile", """

rule creator ( string targets * : sources * : * )
{
    STRING on $(targets) = $(string) ;
    creator2 $(targets) : $(sources) ;
}

actions creator2
{
    echo $(STRING) > $(<)
}

make foo.bar : : creator foobar ;
""")

t.run_build_system()
t.expect_addition("bin/debug/foo.bar")
t.fail_test(find(t.read("bin/debug/foo.bar"), "foobar") == -1)


t.cleanup()
