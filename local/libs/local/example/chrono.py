
# Copyright (C) 2009-2011 Lorenzo Caminiti
# Use, modification, and distribution is subject to the Boost Software
# License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
# copy at http://www.boost.org/LICENSE_1_0.txt).

import sys
import time
import os

cmd = ""
for arg in sys.argv[1:]: cmd += str(arg) + " "

start = time.time()
ret = os.system(cmd)
sec = time.time() - start

if (ret == 0): print "\n" + str(sec) + "s"

