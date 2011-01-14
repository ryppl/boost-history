RELEASE NOTES:
Version 4.0.1. (Inclusion version for Boost 1.46.0 update 1)
+ Improved portability due to tests with the boost testing systems.
+ Tested and improved usability for intervals of pointers and durations.
+ Bugfixes for interval_bitset. Addition of functions and tests.
+ Corrections and improvements of the documentation.

File icl_plus_4_0_1_standalone.7z:
+ icl_plus_4_0_1_standalone is for users of msvc (visual studio) who 
  do not have a version of the boost libraries installed and do not 
  want to get one or are so busy that they think they do not have time 
  to do so ;)
+ icl_plus_4_0_1_standalone contains all parts of boost that are needed 
  to run icl code, examples and tests under a ms windows os (vista, 
  windows7) using msvc-9. Msvc-9 is specifically supported by sln and 
  vcproj files. Other msvc version work as well, but the use has convert 
  project and solution files or use bjam

File icl_plus_4_0_1.7z  or
File icl_plus_4_0_1.tar.gz:
+ icl_plus_4_0_1 is for users of different platforms that have a 
  version of the boost libraries and the build tool bjam installed.
+ You can copy icl_plus/boost/{icl,icl_xt,validate} to the boost 
  directory and icl_plus/libs/{icl,icl_xt,validate} to the libs 
  directory of your boost installation and then run all examples and 
  tests using bjam.

