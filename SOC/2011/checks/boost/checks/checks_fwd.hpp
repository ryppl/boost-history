//  Boost checks/checks_fwd.hpp header file //

//  (C) Copyright Pierre Talbot 2011

//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.

// Provides a (perhaps temporary) mainpage for standalone Doxygen.
// This seems a convenient place to put this, but it could go somewhere else?

/*!
\file
\brief Boost.Checks forward declaration of function signatures.
\details This file can be used to copy a function signature,
but is mainly provided for testing purposes.

\mainpage

\b Boost.Checks

This is the standalone Doxygen front page of the Boost.Checks library.

This library provides a collection of functions for validating and creating check digits.

Most are primarily for checking the accuracy of short strings of typed input
(though obviously also provides against a mis-scan
by a device like a bar code or card reader, or transmission error).

The well-known ISBN is a typical example.
All single altered digits, most double altered digits,
and all transpositions of two digits are caught,
and the input rejected as an invalid ISBN.

See Boost.Checks HTML Manual at

  https://svn.boost.org/svn/boost/sandbox/SOC/2011/checks/doc/html/index.html

and/or equivalent PDF Manual at:

  https://svn.boost.org/svn/boost/sandbox/SOC/2011/checks/doc/checks.pdf

Examples are in folder:

  https://svn.boost.org/svn/boost/sandbox/SOC/2011/checks/libs/checks/example/

and C++ include files are in folder:

  https://svn.boost.org/svn/boost/sandbox/SOC/2011/checks/boost/checks/

*/

/*

DELETE THIS AFTER READING!

It is redundant for production of html and pdf output,
but useful to allow just Doxygen manuals (no Quickbook text or indexing).

Standalone Doxygen can provide a more convenient compact index of
the classes, files, namespaces and functions.
They provide the Doxygen comments, but not any text from Quickbook.

These manuals are quickly produced using the Doxywizard GUI frontend.
A suitable doxyfile holding the list of files to include etc
is conveniently placed in a /doc/doxygen sub-folder.
Selecting this as the working directory from which doxygen will run as Step 1,
and the run tab, and "run doxygen' in Step 2 should generate
an html with index.html in the /doxygen sub-folder.

The mainpage should give a pointer to the full html and/or pdf versions
that provide the full Quickbook generated information.

I:/boost-sandbox/tools/quick_auto_dox_index/libs/quick_auto_dox_index/doc/html/index.html
QuickBook Auto Doxygen Indexed Manual

*/

namespace boost{
  namespace checks{

// Provides forward declaration of all Boost.Checks functions:

    template <class In>
    bool Is_isbn10(In isbn_begin, In isbn_end);

    template <class In>
    char isbn10_check_digit(In isbn_begin, In isbn_end);

    template <typename In>
    inline bool check_luhn(In &begin, const In &end, unsigned int nbr_digits=0);
	
    template <typename In>
    inline bool check_mod11(In &begin, const In end, unsigned int nbr_digits=0);

  }
}