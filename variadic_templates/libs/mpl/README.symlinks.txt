The software in this directory was developed with unix/linux symbolic
links "connecting" the this directory with the boost trunk directory.
This was necessary because using -I compiler flags, although it enables
"connecting" different header directories, does not enable connecting
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
tests with the Jamfile.  The following linux find output should help
with that:

-*- mode: compilation; default-directory: "~/prog_dev/boost-svn/ro/sandbox-ro/variadic_templates/libs/mpl/" -*-
Compilation started at Tue Nov 24 11:23:14

find . -type l -ls
11010127    0 lrwxrwxrwx   1 evansl   evansl         32 Nov 24 08:52 ./test/transform_view.cpp -> ../trunk-test/transform_view.cpp
11010218    0 lrwxrwxrwx   1 evansl   evansl         23 Nov 24 08:52 ./test/front.cpp -> ../trunk-test/front.cpp
11010330    0 lrwxrwxrwx   1 evansl   evansl         24 Nov 24 08:52 ./test/unique.cpp -> ../trunk-test/unique.cpp
11010339    0 lrwxrwxrwx   1 evansl   evansl         28 Nov 24 08:52 ./test/CMakeLists.txt -> ../trunk-test/CMakeLists.txt
11010348    0 lrwxrwxrwx   1 evansl   evansl         23 Nov 24 08:52 ./test/print.cpp -> ../trunk-test/print.cpp
11010369    0 lrwxrwxrwx   1 evansl   evansl         31 Nov 24 08:52 ./test/iterator_tags.cpp -> ../trunk-test/iterator_tags.cpp
11010385    0 lrwxrwxrwx   1 evansl   evansl         28 Nov 24 08:52 ./test/integral_c.cpp -> ../trunk-test/integral_c.cpp
11010400    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/replace.cpp -> ../trunk-test/replace.cpp
11010438    0 lrwxrwxrwx   1 evansl   evansl         22 Nov 24 08:52 ./test/sort.cpp -> ../trunk-test/sort.cpp
11010453    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/lower_bound.cpp -> ../trunk-test/lower_bound.cpp
11010477    0 lrwxrwxrwx   1 evansl   evansl         22 Nov 24 08:52 ./test/find.cpp -> ../trunk-test/find.cpp
11010526    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/has_xxx.cpp -> ../trunk-test/has_xxx.cpp
11010532    0 lrwxrwxrwx   1 evansl   evansl         24 Nov 24 08:52 ./test/size_t.cpp -> ../trunk-test/size_t.cpp
11010547    0 lrwxrwxrwx   1 evansl   evansl         23 Nov 24 08:52 ./test/quote.cpp -> ../trunk-test/quote.cpp
11010564    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/find_if.cpp -> ../trunk-test/find_if.cpp
11010579    0 lrwxrwxrwx   1 evansl   evansl         27 Nov 24 08:52 ./test/pop_front.cpp -> ../trunk-test/pop_front.cpp
11010648    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/advance.cpp -> ../trunk-test/advance.cpp
11010661    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/as_sequence.cpp -> ../trunk-test/as_sequence.cpp
11010669    0 lrwxrwxrwx   1 evansl   evansl         24 Nov 24 08:52 ./test/insert.cpp -> ../trunk-test/insert.cpp
11010673    0 lrwxrwxrwx   1 evansl   evansl         34 Nov 24 08:52 ./test/stable_partition.cpp -> ../trunk-test/stable_partition.cpp
11010688    0 lrwxrwxrwx   1 evansl   evansl         32 Nov 24 08:52 ./test/empty_sequence.cpp -> ../trunk-test/empty_sequence.cpp
11010694    0 lrwxrwxrwx   1 evansl   evansl         26 Nov 24 08:52 ./test/contains.cpp -> ../trunk-test/contains.cpp
11010703    0 lrwxrwxrwx   1 evansl   evansl         24 Nov 24 08:52 ./test/sizeof.cpp -> ../trunk-test/sizeof.cpp
11010706    0 lrwxrwxrwx   1 evansl   evansl         23 Nov 24 08:52 ./test/deque.cpp -> ../trunk-test/deque.cpp
11010709    0 lrwxrwxrwx   1 evansl   evansl         27 Nov 24 08:52 ./test/transform.cpp -> ../trunk-test/transform.cpp
11010712    0 lrwxrwxrwx   1 evansl   evansl         28 Nov 24 08:52 ./test/no_has_xxx.cpp -> ../trunk-test/no_has_xxx.cpp
11010715    0 lrwxrwxrwx   1 evansl   evansl         21 Nov 24 08:52 ./test/int.cpp -> ../trunk-test/int.cpp
11010718    0 lrwxrwxrwx   1 evansl   evansl         18 Nov 24 08:52 ./test/aux_ -> ../trunk-test/aux_
11010727    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/lambda_args.cpp -> ../trunk-test/lambda_args.cpp
11010730    0 lrwxrwxrwx   1 evansl   evansl         32 Nov 24 08:52 ./test/is_placeholder.cpp -> ../trunk-test/is_placeholder.cpp
11010733    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/filter_view.cpp -> ../trunk-test/filter_view.cpp
11010736    0 lrwxrwxrwx   1 evansl   evansl         22 Nov 24 08:52 ./test/fold.cpp -> ../trunk-test/fold.cpp
11010739    0 lrwxrwxrwx   1 evansl   evansl         20 Nov 24 08:52 ./test/if.cpp -> ../trunk-test/if.cpp
11010742    0 lrwxrwxrwx   1 evansl   evansl         23 Nov 24 08:52 ./test/erase.cpp -> ../trunk-test/erase.cpp
11010745    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/is_sequence.cpp -> ../trunk-test/is_sequence.cpp
11010748    0 lrwxrwxrwx   1 evansl   evansl         23 Nov 24 08:52 ./test/count.cpp -> ../trunk-test/count.cpp
11010751    0 lrwxrwxrwx   1 evansl   evansl         22 Nov 24 08:52 ./test/next.cpp -> ../trunk-test/next.cpp
11010752    0 lrwxrwxrwx   1 evansl   evansl         26 Nov 24 08:52 ./test/multiset.cpp -> ../trunk-test/multiset.cpp
11010753    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/reverse.cpp -> ../trunk-test/reverse.cpp
11010756    0 lrwxrwxrwx   1 evansl   evansl         26 Nov 24 08:52 ./test/count_if.cpp -> ../trunk-test/count_if.cpp
11010773    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/bitwise.cpp -> ../trunk-test/bitwise.cpp
11010781    0 lrwxrwxrwx   1 evansl   evansl         24 Nov 24 08:52 ./test/assert.cpp -> ../trunk-test/assert.cpp
11010782    0 lrwxrwxrwx   1 evansl   evansl         18 Nov 24 08:52 ./test/junk -> ../trunk-test/junk
11010783    0 lrwxrwxrwx   1 evansl   evansl         22 Nov 24 08:52 ./test/back.cpp -> ../trunk-test/back.cpp
11010784    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/erase_range.cpp -> ../trunk-test/erase_range.cpp
11010785    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/range_c.cpp -> ../trunk-test/range_c.cpp
11010786    0 lrwxrwxrwx   1 evansl   evansl         26 Nov 24 08:52 ./test/identity.cpp -> ../trunk-test/identity.cpp
11010787    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/min_max.cpp -> ../trunk-test/min_max.cpp
11010788    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/max_element.cpp -> ../trunk-test/max_element.cpp
11010789    0 lrwxrwxrwx   1 evansl   evansl         26 Nov 24 08:52 ./test/zip_view.cpp -> ../trunk-test/zip_view.cpp
11010790    0 lrwxrwxrwx   1 evansl   evansl         26 Nov 24 08:52 ./test/distance.cpp -> ../trunk-test/distance.cpp
11010791    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/single_view.cpp -> ../trunk-test/single_view.cpp
11010792    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/inherit.cpp -> ../trunk-test/inherit.cpp
11010793    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/numeric_ops.cpp -> ../trunk-test/numeric_ops.cpp
11010794    0 lrwxrwxrwx   1 evansl   evansl         22 Nov 24 08:52 ./test/bool.cpp -> ../trunk-test/bool.cpp
11010795    0 lrwxrwxrwx   1 evansl   evansl         22 Nov 24 08:52 ./test/size.cpp -> ../trunk-test/size.cpp
11010796    0 lrwxrwxrwx   1 evansl   evansl         23 Nov 24 08:52 ./test/equal.cpp -> ../trunk-test/equal.cpp
11010797    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/same_as.cpp -> ../trunk-test/same_as.cpp
11010798    0 lrwxrwxrwx   1 evansl   evansl         28 Nov 24 08:52 ./test/arithmetic.cpp -> ../trunk-test/arithmetic.cpp
11010799    0 lrwxrwxrwx   1 evansl   evansl         28 Nov 24 08:52 ./test/comparison.cpp -> ../trunk-test/comparison.cpp
11010800    0 lrwxrwxrwx   1 evansl   evansl         26 Nov 24 08:52 ./test/index_of.cpp -> ../trunk-test/index_of.cpp
11010801    0 lrwxrwxrwx   1 evansl   evansl         39 Nov 24 08:52 ./test/integral_wrapper_test.hpp -> ../trunk-test/integral_wrapper_test.hpp
11010802    0 lrwxrwxrwx   1 evansl   evansl         29 Nov 24 08:52 ./test/upper_bound.cpp -> ../trunk-test/upper_bound.cpp
11010803    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/eval_if.cpp -> ../trunk-test/eval_if.cpp
11010804    0 lrwxrwxrwx   1 evansl   evansl         27 Nov 24 08:52 ./test/partition.cpp -> ../trunk-test/partition.cpp
11010845    0 lrwxrwxrwx   1 evansl   evansl         25 Nov 24 08:52 ./test/logical.cpp -> ../trunk-test/logical.cpp
11010805    0 lrwxrwxrwx   1 evansl   evansl         60 Sep 11 23:34 ./trunk-test -> /home/evansl/prog_dev/boost-svn/ro/boost-trunk/libs/mpl/test

Compilation finished at Tue Nov 24 11:23:15
