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
 * \tparam Iterator which represents the beginning or the ending of a sequence of character.<br> unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the Luhn algorithm will operate. If the size is < 1, the luhn algorithm will validate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin and end are unchanged.
 * \result end is returned if the number of digits found is less or equals to 1 or not equals to nbr_digits.<br> begin is returned if the check digit of the sequence is not valid.<br> An iterator on the check digit is returned if the sequence is valid.
 */
template <class In>
inline In check_luhn(const In &begin, const In &end, unsigned int nbr_digits)
{
  In iter = begin;
  // If the number of digits isn't given, we compute it.
  if(!nbr_digits)
  {
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return end if the number of digit is equals to 0.
	if(!nbr_digits)
	  return end;
	iter = begin;
  }
  int sum = 0,v;
  while(nbr_digits > 0 && iter != end)
  {
    if(*iter >= '0' && *iter <= '9')
		sum += (v= (*iter & 15) << (--nbr_digits&1)) - 9 * (v>9);
	++iter;
  }
  // Return end if the number of digit isn't equal to the number specified.
  if(nbr_digits)
    return end;
  // Return begin if the sum is invalid
  // Return iter if the sum is valid
  return (sum % 10) ? begin : --iter ;
}

/** Validate the check digit of the number provided with the modulus 11 algorithm.
 * \tparam Iterator which represents the beginning or the ending of a sequence of character.<br> Int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [int] nbr_digits Represents the number of digits on which the modulus 11 algorithm will operate. If the size is < 1, the modulus 11 algorithm will validate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin and end are unchanged.
 * \result end is returned if the number of digits found is less or equals to 1 or not equals to nbr_digits.<br> begin is returned if the check digit of the sequence is not valid.<br> An iterator on the check digit is returned if the sequence is valid.
 */
template <class In>
inline In check_mod11(In begin, In end, unsigned int nbr_digits)
{
  In iter = begin;
  // If the number of digits isn't given, we compute it.
  if(nbr_digits == 0)
  {
	while(iter != end)
      if(*iter++ >= '0' && *iter <= '9')
        ++nbr_digits;
	iter = begin;
  }
  int sum = 0;
  // Sum the n-1 first digit with a weigth of n
  while(nbr_digits > 1 && iter != end)
  {
    if(*iter >= '0' && *iter <= '9')
	  sum += *iter & 15 * nbr_digits--;
	++iter;
  }
  // Add the check digit to the sum (add 10 if the check digit equals 'x' or 'X')
  while(nbr_digits && iter != end)
  {
    if(*iter >= '0' && *iter <= '9')
	{
	  sum += *iter & 15;
	  --nbr_digits;
    }
	else if(*iter == 'x' || *iter == 'X')
	{
	  sum += 10;
	  --nbr_digits;
	}
	else
	  ++iter;
  }
  // Return end if the number of digit isn't equal to the number specified.
  if(nbr_digits)
    return end;
  // Return begin if the sum is invalid
  // Return iter if the sum is valid
  return (sum % 11) ? begin : --iter ;
}

template <class In>
inline In check_mod10(In begin, In end, unsigned int nbr_digits, unsigned int weight)
{
  return begin;
}

} // namespace checks
} // namespace boost

#endif // #define BOOST_CHECK_MOD_INCLUDED

