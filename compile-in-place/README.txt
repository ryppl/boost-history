[$Id]

Note: below is a copy of a mail from 2008-04-29 which started this feature
branch. This is intended as information for people that want to try this
feature. Further information follows.

TODO:
------

- I replaced UINTMAX_C(~0) with static_cast<boost::uintmax_t>(-1) in once.cpp
but I'm not sure why this was used in the first place. The problem I solved is
that this doesn't require #define __STDC_CONSTANT_MACROS for the whole program
using UINTMAX_C(~0).

- The thread example currently uses thread, mutex and condition. Some tests for
once and TSS are still missing.

- Check if any changes to the Python support for embedding should be done for
the test file.

- Check if any of the changes to the filesystem lib affect the example code.

- IOStreams depend on zlib and bzip2. However, the core functions should be
available even without them, so we should try to make it possible to compile
without them. There is also one filter for gzip, what about that? Also,
looking at detail/config/zlib.hpp and bzip2.hpp it seems to link to two more
libs boost_zlib and boost_bzip2. Maybe this should all be split somehow...

- Boost.Regex can be configured to use ICU, test that setup.

- boost/regex/v4/instances.hpp actually contains code that seems to be part of
the actual library code. Check what is going on there and whether it shouldn't
be moved from the headers to the library sources.

- Under MS VC++8 (and others as well, I guess) you need to define a macro to
tell the .cpp sources that you are compiling them and one to the header files
to tell them that you don't want autolinking. Currently, the headers only
support BOOST_foo_STATIC_LIB and BOOST_foo_DYNAMIC_LIB (or something akin) to
switch between static and dynamic linking. These patches here should provide a
third way.

==============================================================================
Content of mail follows...

I have in the past sometimes[1] described another alternative way to linking 
and quite a few people also seemed interested. Basically, it works by 
including the whole library code into the application that links to it. 
However, as opposed to a header-only library, the code is only included once 
in a translation unit of the linking program instead of inline via the header 
files.


Example:
---------

   // lib.hpp:
   void foo();

   // lib.cpp:
   void foo() {
     frobnicate(bar);
   }

   // program1.cpp:
   #include <lib.hpp>

   // program2.cpp:
   #include <lib.hpp>

   // program_main.cpp:
   #include <lib.hpp>
   #include <lib.cpp>

   int main() {
     foo();
   }

As you see, lib.cpp is included into exactly one translation unit of the final 
executable (it's up to the user to make sure that it's exactly done once).


Why bother?
------------

The very simple reason is simplicity.

* Even on a system where I don't have admin access, I can easily download and 
compile against Boost, without having to first compile shared/static libs or 
having to modify paths in order for the runtime linker to find DLLs.

* Even if I had admin access, I would still be reluctant to installing Boost's 
trunk.

* When I'm testing different compiler settings trying to get another few 
percent of performance, I don't have to compile another Boost version for 
each compiler setting.

* I don't have to know which library to link to, which lowers entry barriers 
for new Boost users.

* I can easily compile and distribute programs without having to worry about 
required shared libraries.

* I have a guarantee that the library's ABI actually conforms to the ABI that 
my program expects, simply because I compiled it with the exact same compiler 
settings.


Drawbacks:
-----------

The main drawback is that you can't guarantee that library implementation code 
is compiled with e.g. NOMINMAX defined. If an incompatible environment is 
detected, I would simply use #error to alert the user.

Further, 'private' symbols (static functions or functions in anonymous 
namespaces) can't be reused by different sourcefiles because they are all 
compiled into the same translation units. If this happens, I would simply try 
to rename things until the clash is resolved or maybe move them to a separate 
namespace.


Tasks to completion:
---------------------

* Add one central file compile_inplace.cpp for each library. This would mean 
that for Boost.Thread we then include "boost/thread/compile_inplace.cpp" from 
exactly one translation unit in order to get it to work.

* Fix some code that doesn't lend itself to inclusion in user code. One 
particular example is in Boost.Thread, where two sourcefiles include another 
file that contains utility functions in an anonymous namespace. If both 
sourcefiles are compiled by the same TU, the anonymous namespace is also 
compiled twice leading to multiple definitions. Just adding an include guard 
helps.

* Prepare import/export code. This is mainly a win32 task which means 
programming the macros that control __declspec(dllexport) or 
__declspec(dllimport). Basically this means switching to static linking 
(where neither of the above are generated) but also turning off autolinking 
(because there is no according library that the linker should link to).

* Add error checking. In particular, you must not e.g. include 
compile_inplace.cpp if you are doing other kinds linking.

* Documentation updates. This mostly means adding this way of linking to the 
pages that describe which library to link against as an alternative way. I 
would even use this form in examples for the various libs exactly because it 
is the easiest way to link against a library.


What do I need:
----------------

What I need is a folder on SVN for the branch and according write permissions 
so I can actually work on it. Of course, I could also create a repository 
somewhere and just branch Boost there, but this has the disadvantage that it 
makes it harder to merge the changes back (SVN isn't particularly good on 
inter-repository merges) and also it would make it easier for people to test 
the branch (you could 'svn switch' to it and it would only transfer the 
changes). Lastly, I do need access from two locations, one at work where I 
could test win32 versions and one at home where I have a Linux/PPC for 
testing.


thanks

Uli


[1]
http://lists.boost.org/Archives/boost/2006/03/102525.php
http://lists.boost.org/Archives/boost/2007/04/119622.php
