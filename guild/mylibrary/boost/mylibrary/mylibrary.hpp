// Copyright Paul A. Bristow 2011

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_MYLIBRARY_HPP
#define BOOST_MYLIBRARY_HPP

#include <boost/config.hpp>  // for workarounds.

/*!
  \file
  \brief Template for Boost documentation.
  \details Header file for use by example.
   Also using Quickbook, Doxygen, and indexed by AutoIndex.

 \mainpage QuickBook Auto Doxygen Indexed mainpage.
This is the front page of the QuickBook Auto Doxygen Indexed
example of producing Boost documentation.

\n It is redundant for production of html and pdf output,
but useful to allow just Doxygen manuals (no Quickbook or indexing).

These can provide a convenient compact index of the classes, files, namespaces and functions.
They provide the Doxygen comments, but not the text from Quickbook.

These manuals are quickly produced using the Doxywizard GUI frontend.
A suitable Doxyfile holding the list of files to include etc
is conveniently placed in a /doc/doxygen sub-folder.
Selecting this as the working directory from which doxygen will run as Step 1,
and the run tab, and "run doxygen' in Step 2 should generate an html in the /doxygen sub-folder.

The mainpage should give a hyperlink to the full html and pdf versions
that provide the full Quickbook generated information, for example:

http://www.boost.org/doc/libs/my_library/index.html My Library Manual

*/

namespace boost
{
  namespace mylibrary
  {
   //! \brief A test class - a comment description that preceeds the class.
  class myclass
  { /*! \details \class myclass
    More details on a class by comment that is tucked in under the class
    (to reduce the line count).
    BUT you must explicitly link to the class or it will be applied to the next member.
    using a < doesn't work for classes as it does for members.
    If the Doxygen comment goes over a few lines,
    it is more convenient to use C style.
  */
    public:
      //! An fully useless enum with 3 values.
      /*! More detailed enum description,
         needing more than one line,
         so using C style comment markers.
      */
      enum test_enum {
                   test_enum_val1, //!< Enum value TVal1 (Note: the use of < to link this Doxygen comment to the same line).
                   test_enum_val2, //!< Enum value TVal2.
                   test_enum_val3  /*!< Enum value TVal3. (Note: using C style comment markers).*/
                 };

      //! A constructor.
      /*!
        A more elaborate description of the constructor.
        \warning This constructor does nothing much.
      */
      myclass();

      //! A destructor.
      /*!
        A more detailed description of the destructor.
        \warning Warning! This destructor may explode in your face!
        (The doxygen command warning will NOT be found by the index term warning,
        but words in the warning, like explode, WILL be found).
      */
     ~myclass();

      //! A normal member function taking two arguments and returning an integer value.
      /*!
        \param a an integer argument.
        \param s a constant character pointer.
        \return The test result.
        \sa Test(), ~Test(), testMeToo() and publicVar()
      */
      int test_me(int a, const char *s);

      //! A pure virtual member with descriptions of parameters.
      //! And a 'see also' reference to another version of the function.
      /*!
        \see test_me()
        \param c1 the first argument.
        \param c2 the second argument.
      */
      virtual void test_me_too(char c1, char c2) = 0;

      //! A public variable.
      /*!
        Details about the variable.
      */
      int mypublic_var; //!< My public class variable.


      private:
       int myprivate_var; //!< My private class variable.

  }; // class myclass

  int donowt(int i)
  { /*!
       Non-member free function that does nowt useful at all.
      \param i is an argument that is ignored completely.
      \returns -1 always.
      \pre No preconditions.
      \post No side effects.
  */
    return -1;
  } //   int donowt(int i)



} // namespace mylibrary

} // namespace boost

#endif // #define BOOST_MYLIBRARY_HPP
