The software in this directory was developed with unix/linux symbolic
links "connecting" the this directory with the boost trunk directory.
This was necessary because using -I compiler flags, although enable
"connecting" different header directories, do not enable connecting
different source directories.  IOW, in the test directory, several
tests had to be modified for variadic templates; however, several
others required no modifications.  Those unmodified tests are
connected from this development test directory to the boost trunk test
directory via symbolic links.  In this directory, that symlink is
trunk-test:

  /home/evansl/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/libs/mpl:
  total used in directory 16 available 106308828
  drwxr-xr-x 3 evansl evansl 4096 Aug 29 05:26 .
  drwxr-xr-x 3 evansl evansl 4096 Aug 29 05:04 ..
  -rw-r--r-- 1 evansl evansl   85 Aug 29 05:26 README.symlinks.txt
  drwxr-xr-x 2 evansl evansl 4096 Aug 29 05:04 test
  lrwxrwxrwx 1 evansl evansl   37 Aug 29 05:05 trunk-test -> ../../../../boost-trunk/libs/mpl/test
  
In the test directory, the unmodified tests are accessed indirectly
through trunk-test, e.g.:

  /home/evansl/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/libs/mpl/test:
  total used in directory 104 available 106324628
  drwxr-xr-x 2 evansl evansl  4096 Aug 29 05:04 .
  drwxr-xr-x 3 evansl evansl  4096 Aug 29 05:05 ..
  lrwxrwxrwx 1 evansl evansl    25 Aug 29 05:04 advance.cpp -> ../trunk-test/advance.cpp
  -r--r--r-- 1 evansl evansl  1151 May 18 21:53 always.cpp

Hopefully, windows and mac users have some equivalent to unix/linux
symlinks.  If not, they will have to copy the source test files from
the boost trunk directory to the test subdirectory to run all the
test.

    
