//  Boost checks/upc.hpp header file  ------------------------------------//
//  (C) Copyright Pierre Talbot 2011
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_UPC_INCLUDED
#define BOOST_UPC_INCLUDED

namespace boost {
    namespace checks{

/** Check the validity of the Universal Product Code category A (UPC-A) provided.
 * \tparam Iterator which represents the beginning or the ending of a sequence of character
 * \param [in] upc_begin Represents the beginning of the UPC sequence to check.
 * \param [in] upc_end] Represents one off the end of the UPC sequence to check.
 * \pre upc_begin and upc_end are valid initialized iterators. The length of the sequence should be of size 12 and the sequence should contains only digits.
 * \post upc_begin and upc_end are unchanged.
 * \result true if the UPC sequence is valid which it means that the check digit is equals to the last character. Otherwise false.
 */
template <class In>
inline bool Is_upca(In upc_begin, In upc_end)
{
  In iter = upc_begin;
  unsigned short checksum = 0, i;
  for(i = 11; i>0; ++iter, --i)
  {
          if(iter == upc_end  || !isdigit(*iter))     return false;
    else    checksum += (*iter % 2 == 0) ? *iter * 3 : *iter;
  }
  if( i != 0 || iter == upc_end) return false;
  return (*(++iter) == (10 - checksum % 10) %10) && iter == end;
}

/** Compute the check digit of the Universal Product Code category A (UPC-A) provided
 * /tparam Iterator which represents the beginning or the ending of a sequence of character.
 * /param [in] upc_begin Represents the beginning of the UPC sequence to check.
 * /param [in] upc_end Represents one off the end of the UPC sequence to check.
 * /pre upc_begin and upc_end are valid initialized iterators. The length of the sequence should be of size 11 and the sequence should contains only digits.
 * /post upc_begin and upc_end are unchanged.
 * /result 0 if the check digit couldn't be calculated (Exemple : wrong size, ...). Otherwise, the check digit character between '0' and '9'.
 */
template <class In>
inline char upca_check_digit(In upc_begin, In upc_end)
{
  In iter = upc_begin;
  unsigned short checksum = 0, i;
  for(i = 11; i>0; ++iter, --i)
  {
    if(iter == upc_end  || !isdigit(*iter)) return false;
    else  checksum += (*iter % 2 == 0) ? *iter * 3 : *iter;
  }
  if( i != 0 || iter != upc_end) return false;
  return (10 - checksum % 10) %10;
}

} // namespace checks
} // namespace boost

#endif // #define BOOST_UPC_INCLUDED

