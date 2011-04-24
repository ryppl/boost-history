
import sys
import time
import os

cmd = ""
for arg in sys.argv[1:]:
    cmd += str(arg) + " "

start = time.time()
ret = os.system(cmd)
sec = time.time() - start

if (ret == 0): print "\n" + str(sec) + "s"

