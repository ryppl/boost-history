# Copyright David Abrahams 2006. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

from projects import projects
from os import system
for p,s,m in projects:
    system('svn mkdir '+p)
    system('svn mkdir '+p+'/branches')
    system('svn mkdir '+p+'/tags')
    system('svn mkdir '+p+'/trunk')
