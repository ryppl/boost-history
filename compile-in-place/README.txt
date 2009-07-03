[$Id$]

Note: below is a copy of a mail from 2008-04-29 which started this feature
branch. This is intended as information for people that want to try this
feature. Further information follows.

FAQ:
-----

Q: Why not let the user add the sources to a library into their project
instead?
A: A single, separate file can remain constant between library releases even
if the internal organisation of the library changes. It thus isolates the user
from the implementation.

Q: Why use #include to compile the library sourcecode instead of letting
people add the library sourcecode to their project?
A: In order to use Boost, you must at the very least adjust the path to the
includes. Exploiting this mechanism allows compiling in the libraries without
any further adjustments [barring necessary compiler switches].

Q: Why include a .cpp file? You should only include .hpp files!
A: A .hpp file is a header file that declares an interface. In addition, it
can contain implementations inline. In any case, it can be included more than
once, both in each translation unit and the overall program. Similarly, using
.inl as extension is out, because the file doesn't contain inline code
definitions. Instead, the file (indirectly) includes plain implementation
code, which must only be compiled once per binary. If someone is puzzled by
this, that is actually good, because that means that they will first think and
do some research before making any conclusions. The idea is that it is better
to be not understood than to be misunderstood.


Comparison between different forms of linking:
-----------------------------------------------

There are basically four different ways to use a library in a program:
1. Using inline (template) code.
2. Using an in-place compiled library.
3. Using a statically linked library.
4. Using a shared library (.so/.dll).

The order of these is BTW not incidental. Rather, the in-place compiled
library can be placed between a completely inline library and a statically
compiled library.


The inline (template) code has following characteristics:
- The code is compiled into the binary, so it is fixed at compile time.
- No separate compilation is necessary.
- All code is inline, which can lead to a bloated size of the resulting binary
and increased compile time.
- Using the library only requires the user to include header files and, in
rare cases, to configure the compiler (compiler flags).


The in-place compiled code has following characteristics:
- The code is compiled into the binary, so it is fixed at compile time.
- No separate compilation is necessary.
- The whole library code is compiled into the program. This means that you
need the time for compiling the code and that you then rely on your toolchain
to remove unused parts or live with the bloat.
- In order to use the library, the user needs to include header files, in rare
cases to configure the compiler (compiler flags) and include a source file in
exactly one translation unit.
- Adjusting compiler flags is often only necessary for the library sources,
which have to be repeated in the using project.


The static library has following characteristics:
- The code is compiled when the library is created, the used implementation is
selected when the program is linked.
- The library needs to be compiled separately.
- The whole library code is linked into the program. You have the advantage in
speed of having precompiled code. You still rely on the toolchain to remove
parts you don't actually need.
- In order to use the library, the user needs to include header files, in rare
cases to configure the compiler (compiler flags) and add a library file to the
linker input.
- Adjusting compiler flags is often only necessary for the library sources,
which is already done by the library maintainer.


The shared library has following characteristics:
- The code is compiled when the library is created, the used implementation is
selected when the program is started.
- The library needs to be compiled separately.
- The program only contains references into the shared library, so the library
doesn't add much to the program size. At runtime, the whole code is loaded,
though it can be shared with other programs using the library.
- In order to use the library, the user needs to include header files, in rare
cases to configure the compiler (compiler flags) and add a library file to the
linker input.
- Adjusting compiler flags is often only necessary for the library sources,
which is already done by the library maintainer.



Notes:
-------

- Compiling often generates a large amount of warnings, but they can be ignored.

- This is recorded in TRAC ticket #3103.


TODO:
------

- MS Windows needs further work. The problem there is that Borland and MSC do
auto linking, i.e. select the linked lib during compilation, which conflicts
with compiling the libraries into the code itself. The goal is to make the
macro BOOST_<whatever>_COMPILE_IN_PLACE take that part.

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
