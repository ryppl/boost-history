Installation instructions:
* Unpack all files to the $(BOOST_ROOT)/libs/xint directory.
* Copy xint.hpp to the $(BOOST_ROOT)/boost directory.
* Change to the $(BOOST_ROOT)/libs/xint/build directory and run bjam with your
favorite commands (under Linux, I use "bjam release install").

Notes:

The code has been fully tested under Windows (MSVC 8 SP1) and Linux (GCC 4.4.1).

Auto-linking and Windows DLLs are not yet supported. The code will compile under
Windows with no problem, but I haven't tried it through the bjam system there
yet.

There is a test suite, but I'm not sure how it would fit into the Boost test
system yet.

I'm also not sure that I've got the Jamfile correct. For whatever reason, it
won't create a static version of the library for me, or maybe I just don't know
how to ask it to do so.

See the very thorough documentation for any other questions, or ask me on the
Boost developer's mailing list. Please put [xint] at the beginning of the
subject line so I'll notice it faster.


