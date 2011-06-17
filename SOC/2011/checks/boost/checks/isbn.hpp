//  Boost checks/isbn.hpp header file  ------------------------------------//
//  (C) Copyright Pierre Talbot 2011
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.


// Note on doxygen trip char.  It seems to be convention to use \param rather than @param.

// I can't fully remember the logic but Steven Watanabe did explain it to me.
// Steven is a Quickbook Doxygen guru!
// He also uses @ but for things like @c metafunction<_>  which just does typewirter font.



#ifndef BOOST_ISBN_INCLUDED
#define BOOST_ISBN_INCLUDED

namespace boost {
    namespace checks{


/** Check the validity of the International Standard Book Number (ISBN) of size 10.
 * \tparam In Iterator which represent the bound of a sequence of character.
 * \param [in] isbn_begin Represents the beginning of the ISBN sequence to check.
 * \param [in] isbn_end Represents one off the end of the ISBN sequence to check.
 * \pre isbn_begin and isbn_end are valid initialized iterators.The length of the sequence should be at least of size 10 and the sequence should contains only dash(es) and digits.
 * \post isbn_begin and isbn_end are inchanged.
 * \return true if the sequence is a valid ISBN of size 10, otherwise false.
 */
template <class In>
inline bool Is_isbn10(In isbn_begin, In isbn_end)
{
  if( isbn_begin == isbn_end ) return false;
  In iter = isbn_begin;
  int check = 0, i;
  for(i=10; i>0 && iter != isbn_end;)
  {
    if( *iter != '-')
    {
      if(!isdigit(*iter))
        if(i==1 && (*iter == 'x' || *iter == 'X'))  check += 10;
        else  return false;
      else
        check +=  i * (*iter - 48);
      --i;
    }
    ++iter;
  }
  return (i > 0 || iter != isbn_end) ? false : check % 11 == 0;
}

/** Compute the check digit of the International Standard Book Number (ISBN) of size 10.
 * \tparam In Iterator which represent the bound of a sequence of character.
 * \param [in] isbn_begin Represents the beginning of the ISBN sequence to check.
 * \param [in] isbn_end Represents one off the end of the ISBN sequence to check.
 * \pre isbn_begin and isbn_end are valid initialized iterators. The length of the sequence should be of size 9 and the sequence should contains only digits and dashes.
 * \post isbn_begin and isbn_end are inchanged.
 * \return The check digit of the ISBN of size 9 provided, which can be between '0' and '9' or 'X'. Otherwise 0 is returned if the ISBN of size 9 provided is not correct.
 */
template <class In>
inline char isbn10_check_digit(In isbn_begin, In isbn_end)
{
  if( isbn_begin == isbn_end ) return false;
  In iter = isbn_begin;
  int check = 0, i;
  for(i=9; i>0 && iter != isbn_end;)
  {
    if( *iter != '-')
    {
      if(!isdigit(*iter))
        if(i==1 && (*iter == 'x' || *iter == 'X'))  check += 10;
        else  return false;
      else
        check +=  i * (*iter - 48);
      --i;
    }
    ++iter;
  }
  if(i > 0 || iter != isbn_end) return false;
  check = 11 - check % 11;
  return (check == 10) ? 'X' : static_cast<char>(check + 48);
}


/** Check the validity of the International Standard Book Number (ISBN) of size 13. (It is a ISBN encapsulated into a EAN).
*/
template <class In>
inline bool Is_isbn13(In isbn_begin, In isbn_end)
{
  // Call Is_ean13 with limitation on the 3 first number (978 - 979).
}

/** Compute the check digit of the International Standard Book Number (ISBN) of size 13. (It is a ISBN encapulsed into a EAN).
*/
template <class In>
inline char isbn13_check_digit(In isbn_begin, In isbn_end)
{
  // Call ean13_check_digit with limitation on the 3 first number (978 - 979).
}

} // namespace checks
} // namespace boost

#endif
