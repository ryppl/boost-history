/*!
  \file quick_auto_dox_index.hpp
  \brief Template for documentation.
  \details Example file for creating Boost documentation using Quickbook, Doxygen and Auto-Indexing.
    Example code with comment taken from /doxygen/html/docblocks.html using Boost naming conventions.
    \n
    The style of this example takes a lot of lines, so other Boost-style examples are much more compact.
    They do rely on syntax coloring to make the comments stand clear
    and not obscure the real C++ code.

  \date Mar 2011
  \author  Paul A. Bristow
*/

// Copyright Paul A. Bristow 2011

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_QUICK_AUTO_DOX_INDEX_HPP
#define BOOST_QUICK_AUTO_DOX_INDEX_HPP

/*! \mainpage QuickBook Auto Doxygen Indexed Main page.
This is the standalone Doxygen front page of the QuickBook Auto Doxygen Indexed
example of producing Boost documentation.

\n It is redundant for production of html and pdf output,
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

 //! \brief A test class - a comment description that preceeds the class.
class test
{ /*! \details \class test More on a class by comment that is tucked in under the class (to reduce the line count).
  BUT you must explicitly link to the class or it will be applied to the next member.
  Using a < doesn't work for classes as it does for members.
*/
  public:
    //! Example of documenting an enum.
    /*! More detailed enum description.
       This description needs more than one line,
       so convenient to use C style comment markers.
    */
    enum test_enum {
                 test_enum_val1, //!< Enum value TVal1 (Note the < to link to the same line).
                 test_enum_val2, //!< Enum value TVal2. (Using C++ // comment markers, and < to link to the same line.
                 test_enum_val3  /*!< Enum value TVal3. (using C style comment markers).*/
               };

    //! A constructor.
    /*!
      A more elaborate description of the constructor.
    */
    test();

    //! A destructor.
    /*!
      A more elaborate description of the destructor.
      \pre No preconditions.
      \post A test object constructed, with no side effects.
      \warning This descructor may blow up?
    */
   ~test();

    //! A normal member function taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \returns The test result.
      \pre No preconditions.
      \post No side effects.
      \sa Test(), ~Test(), testMeToo() and publicVar()
    */
    int test_me(int a, const char *s);

    /*! A pure virtual member with descriptions of parameters.
        And a 'see also' reference to another version of the function.

      \see test_me()
      \deprecated Will be removed in the next but one version.  Use test_me() instead.
      \param c1 the first argument.
      \param c2 the second argument.
      \returns The test result.

    */
    virtual void test_me_too(char c1, char c2) = 0;

    //! A public variable.
    /*!
      Details about the variable.
    */
    int public_var;

    //! A function variable.
    /*!
      Details about what the function does.
      \remark This is an implementation detail and not for user code.
      \param a 1st Selector.
      \param b 2nd Selector.
      \returns result of calling handler.
    */
    int (*handler)(int a,int b);
}; // class test

#endif // BOOST_QUICK_AUTO_DOX_INDEX_HPP
