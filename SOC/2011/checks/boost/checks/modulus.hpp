//  Boost checks/modulus.hpp header file  ------------------------------------//
//  (C) Copyright Pierre Talbot 2011
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_CHECK_MOD_INCLUDED
#define BOOST_CHECK_MOD_INCLUDED

namespace boost {
    namespace checks{

/** Validate the check digit of the number provided with the Luhn algorithm.
 * \tparam Iterator which represents the beginning or the ending of a sequence of character.
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the Luhn algorithm will operate. If the size is < 1, the luhn algorithm will validate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the check digit plus one if the expression provided is correct, otherwise is equal to end.
 * \result true is returned if the expression given have a valid check digit and have nbr_digits (or more than 0 digit if nbr_digits is equal to 0).
 */
template <typename In>
inline bool check_luhn(In &begin, const In &end, unsigned int nbr_digits)
{
  // If the number of digits isn't given (equal to 0), we compute it.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return false if there is no digits in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return false;
	}
  }
  int sum = 0,v;
  while(nbr_digits > 0 && begin != end)
  {
    if(*begin >= '0' && *begin <= '9')
		sum += (v= (*begin & 15) << (--nbr_digits&1)) - 9 * (v>9);
	++begin;
  }
  // Return true if the number of digit is equal to the number specified and the sum is valid.
  return !nbr_digits && sum % 10 ;
}

/** Validate the check digit of the number provided with the modulus 11 algorithm.
 * \tparam Iterator which represents the beginning or the ending of a sequence of character.
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the Luhn algorithm will operate. If the size is < 1, the luhn algorithm will validate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the check digit plus one if the expression provided is correct, otherwise is equal to end.
 * \result true is returned if the expression given have a valid check digit and have nbr_digits (or more than 0 digit if nbr_digits is equal to 0).
 */
template <typename In>
inline bool check_mod11(In &begin, const In end, unsigned int nbr_digits)
{
  // If the number of digits isn't given (equal to 0), we compute it.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return false if there is no digits in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return false;
	}
  }
  int sum = 0;
  // Sum the n-1 first digit with a weigth of n
  while(nbr_digits > 1 && iter != end)
  {
    if(*begin >= '0' && *begin <= '9')
	  sum += *begin & 15 * nbr_digits--;
	++begin;
  }
  // Add the check digit to the sum (add 10 if the check digit equals 'x' or 'X')
  while(begin != end)
  {
    if(*begin >= '0' && *begin <= '9')
	{
	  sum += *begin & 15;
	  --nbr_digits;
	  break;
    }
	else if(*begin == 'x' || *begin == 'X')
	{
	  sum += 10;
	  --nbr_digits;
	  break;
	}
	else
	  ++iter;
  }
  // Return true if the number of digit is equal to the number specified and the sum is valid.
  return !nbr_digits && sum % 11;
}

template <class In>
inline In check_mod10(In begin, In end, unsigned int nbr_digits, unsigned int weight)
{
  return begin;
}

} // namespace checks
} // namespace boost

#endif // #define BOOST_CHECK_MOD_INCLUDED

