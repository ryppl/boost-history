Installation instructions

* Place the xint directory directly under the $(BOOST_ROOT)/libs directory.
* Copy (or link) the boost/xint directory directly under $(BOOST_ROOT)/boost.
* Change to the $(BOOST_ROOT)/libs/xint/build directory and run bjam with your
favorite commands (under Linux, I use "bjam release install").

Notes:

The code has been fully tested under Windows (MSVC 8 SP1) and Linux (GCC 4.4.1).

Auto-linking and Windows DLLs are not yet supported. The code will compile under
Windows with no problem, but I haven't tried it through the bjam system there
yet.

See the very thorough documentation for any other questions, or ask me on the
Boost developer's mailing list. Please put [xint] at the beginning of the
subject line so I'll notice it faster.


