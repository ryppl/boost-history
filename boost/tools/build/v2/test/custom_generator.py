#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2003. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.


from BoostBuild import Tester, List


t = Tester()

# Attempt to declare a generator for creating OBJ from RC files.
# That generator should be considered together with standard
# CPP->OBJ generators and successfully create the target.
# Since we don't have RC compiler everywhere, we fake the action.
# The resulting OBJ will be unusable, but it must be created.

t.write("project-root.jam", """ 
import rcc ; 
""")

t.write("rcc.jam", """ 
import type ;
import generators ;
import print ;

# Use 'RCC' to avoid conflicts with definitions in
# the standard rc.jam and msvc.jam
type.register RCC : rcc ;

rule resource-compile ( targets * : sources * : properties * )
{
    print.output $(targets[1]) ;
    print.text "rc-object" ;
}

generators.register-standard rcc.resource-compile : RCC : OBJ ;

""")

t.write("Jamfile", """ 
obj r : r.rcc ; 
""")

t.write("r.rcc", """ 
""")

t.run_build_system()
t.expect_content("bin/$toolset/debug/r.obj", "rc-object")

t.cleanup()



