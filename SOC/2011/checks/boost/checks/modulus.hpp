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
 * \tparam Iterator with at least the caracteristics of an input iterator. It represents the beginning or the ending of a sequence of character. 
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
  // If the number of digits isn't given (equal to 0), we count these.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return false if there is no digit in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return false;
	}
  }
  int sum = 0,v;
  /* We start with the leftmost digit and we multiply by two 
   * if the total number of digit is even. We alternate the multiplication
   * for all the digits. We subtract 9 from the digit greater than 9 
   * after the multiplication. */
  while(nbr_digits > 0 && begin != end)
  {
    if(*begin >= '0' && *begin <= '9')
		sum += (v= (*begin & 15) << (--nbr_digits&1)) - 9 * (v>9);
	++begin;
  }
  // Return true if the number of digit is equal to the number specified and the sum is valid.
  return !nbr_digits && !(sum % 10) ;
}

/** Calculate the check digit of the number provided with the Luhn algorithm.
 * \tparam Iterator with at least the caracteristics of an input iterator. It represents the beginning or the ending of a sequence of character. 
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the Luhn algorithm will operate. If the size is < 1, the luhn algorithm will calculate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the last digit encountered plus one if the expression provided is correct, otherwise is equal to end.
 * \result 0 is returned if the expression given have not nbr_digits (or no digit if nbr_digits is equal to 0). Otherwise the ASCII character of the check digit is returned.
 */
template <typename Out, typename In>
inline Out compute_luhn(In &begin, const In &end, unsigned int nbr_digits)
{
  // If the number of digits isn't given (equal to 0), we count these.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return 0 if there is no digit in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return 0;
	}
  }
  int sum = 0,v;
  /* We start with the leftmost digit and we multiply by two 
   * if the total number of digit is even. We alternate the multiplication
   * for all the digits. We subtract 9 from the digit greater than 9 
   * after the multiplication. */
  while(nbr_digits > 0 && begin != end)
  {
    if(*begin >= '0' && *begin <= '9')
		sum += (v= (*begin & 15) << (nbr_digits--&1)) - 9 * (v>9);
	++begin;
  }
  return ((10 - sum % 10) % 10) | '0';
}

template <typename In>
inline char compute_luhn(In &begin, const In &end, unsigned int nbr_digits)
{
  // If the number of digits isn't given (equal to 0), we count these.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return 0 if there is no digit in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return 0;
	}
  }
  int sum = 0,v;
  /* We start with the leftmost digit and we multiply by two 
   * if the total number of digit is odd. We alternate the multiplication
   * for all the digits. We subtract 9 from the digit greater than 9 
   * after the multiplication. */
  while(nbr_digits > 0 && begin != end)
  {
    if(*begin >= '0' && *begin <= '9')
		sum += (v= (*begin & 15) << (nbr_digits--&1)) - 9 * (v>9);
	++begin;
  }
  return (10 - sum % 10) % 10 | '0';
}
/** Calculate the check digit of the number provided with the modulus 11 algorithm.
 * \tparam Iterator with at least the caracteristics of an input iterator. It represents the beginning or the ending of a sequence of character. 
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the MOD11 algorithm will operate. If the size is < 1, the MOD11 algorithm will calculate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the last digit encountered plus one if the expression provided is correct, otherwise is equal to end.
 * \result 0 is returned if the expression given have not nbr_digits (or no digit if nbr_digits is equal to 0). Otherwise the ASCII character of the check digit is returned.
*/
template <typename In>
inline bool check_mod11(In &begin, const In &end, unsigned int nbr_digits)
{
  // If the number of digits isn't given (equal to 0), we count these.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return false if there is no digit in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return false;
	}
  }
  int sum = 0;
  // Sum the n-1 first digit with a weigth of n. (The weigth is the contribution
  // of a digit to the final sum).
  while(nbr_digits > 1 && iter != end)
  {
    if(*begin >= '0' && *begin <= '9')
	  sum += *begin & 15 * nbr_digits--;
	++begin;
  }
  // Add the check digit to the sum (add 10 if the check digit equals 'x' or 'X').
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
  return !nbr_digits && !(sum % 11);
}
/** Validate the check digit of the number provided with the modulus 11 algorithm.
 * \tparam Iterator with at least the caracteristics of an input iterator. It represents the beginning or the ending of a sequence of character. 
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the Luhn algorithm will operate. If the size is < 1, the luhn algorithm will validate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the check digit plus one if the expression provided is correct, otherwise is equal to end.
 * \result true is returned if the expression given have a valid check digit and have nbr_digits (or more than 0 digit if nbr_digits is equal to 0).
 */
template <typename Out, typename In>
inline Out compute_mod11(In &begin, const In &end, unsigned int nbr_digits)
{
  // If the number of digits isn't given (equal to 0), we count these.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return 0 if there is no digit in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return 0;
	}
  }
  int sum = 0;
  // Sum the digits with a weigth of nbr_digits. (The weigth is the contribution
  // of a digit to the final sum).
  ++nbr_digits; // Adding the unknown check digit to the size...
  while(nbr_digits > 1 && iter != end)
  {
    if(*begin >= '0' && *begin <= '9')
	  sum += *begin & 15 * nbr_digits-- ;
	++begin;
  }
  if(nbr_digits > 1)
    return 0;
  
  // Compute the check digit.
  int check_digit = (11 - sum % 11) %11;
  if(check_digit == 10)
    return 'X';
  return check_digit;
}

/** Validate the check digit of the number provided with the modulus 10 algorithm.
 * \tparam Iterator with at least the caracteristics of an input iterator. It represents the beginning or the ending of a sequence of character. 
 * \tparam unsigned int[] which represents an array of weight.
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int\[\]] weight Represents the weight pattern of the sequence starting on the left of the expression. If weight is null, the algorithm will apply a weight of 1 on the sequence.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the modulus 10 algorithm will operate. If the size is < 1, the modulus 10 algorithm will validate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the check digit plus one if the expression provided is correct, otherwise is equal to end. 
 * \result true is returned if the expression given have a valid check digit and have nbr_digits (or more than 0 digit if nbr_digits is equal to 0).
 */

template <typename In>
inline bool check_mod10(In &begin, const In &end, unsigned int weight[], unsigned int nbr_digits)
{
  // If the weight is null or have a size of zero, we apply a weight of 1 on each digit.
  if(weight == NULL)
  {
    unsigned int weight_tmp[] = {1};
    weight = weight_tmp;
  }
  int weight_size = sizeof(weight) / sizeof(weight[0]);
  // If the number of digits isn't given (equal to 0), we count these.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return false if there is no digit in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return false;
	}
  }
  int sum = 0;
  /* We start with the leftmost digit and we multiply by the weight indicated.
   * The sum of all digits is computed. */
  for(int i=0; nbr_digits > 0 && begin != end; ++i)
  {
    if(*begin >= '0' && *begin <= '9')
		sum += (*begin & 15) * weight[i % weight_size] ;
	++begin;
  }
  // Return true if the number of digit is equal to the number specified and the sum is valid.
  return !nbr_digits && !(sum % 10) ;
}

/** Compute the check digit of the number provided with the modulus 10 algorithm.
 * \tparam Iterator with at least the caracteristics of an input iterator. It represents the beginning or the ending of a sequence of character. 
 * \tparam unsigned int[] which represents an array of weight.
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int\[\]] weight Represents the weight pattern of the sequence starting on the left of the expression. If weight is null, the algorithm will apply a weight of 1 on the sequence.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the modulus 10 algorithm will operate. If the size is < 1, the modulus 10 algorithm will calculate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the last digit encountered plus one if the expression provided is correct, otherwise is equal to end.
 * \result 0 is returned if the expression given have not nbr_digits (or no digit if nbr_digits is equal to 0). Otherwise the ASCII character of the check digit is returned.
 */

template <typename In>
inline char compute_mod10(In &begin, const In &end, unsigned int weight[], unsigned int nbr_digits)
{
  // If the weight is null we apply a weight of 1 on each digit.
  if(weight == NULL)
  {
    unsigned int weight_tmp[] = {1};
    weight = weight_tmp;
  }
  int weight_size = sizeof(weight) / sizeof(weight[0]);
  // If the number of digits isn't given (equal to 0), we count these.
  if(!nbr_digits)
  {
	In iter = begin;
	while(iter != end)
	{
      if(*iter >= '0' && *iter <= '9')
        ++nbr_digits;
	  ++iter;
	}
	// Return false if there is no digit in the expression given.
	if(!nbr_digits)
	{
	  begin = end;
	  return 0;
	}
  }
  int sum = 0;
  /* We start with the leftmost digit and we multiply by the weight indicated.
   * The sum of all digits is computed. */
  for(int i=0; nbr_digits > 0 && begin != end; ++i)
  {
    if(*begin >= '0' && *begin <= '9')
		sum += (*begin & 15) * weight[i % weight_size] ;
	++begin;
  }
  if(nbr_digits > 0)
  {
    begin = end;
    return 0;
  }
  return (10 - sum % 10) % 10 | '0' ;
}

/*!!!
The modulus 97 algorithm wait for a sequence of numbers only ! and will not do anything else that sum the digits and calculate the modulus of this sum.
If you need to check an IBAN use the algorithms in iban.hpp */

/** Validate the check digit of the number provided with the modulus 97 algorithm.
 * \tparam Iterator with at least the caracteristics of an input iterator. It represents the beginning or the ending of a sequence of character. 
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the modulus 97 algorithm will operate. If the size is < 1, the modulus 97 algorithm will validate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the check digits plus one if the expression provided is correct, otherwise is equal to end. 
 * \result true is returned if the expression given have a valid check digit and have nbr_digits (or more than 0 digit if nbr_digits is equal to 0).
 */

template <typename In>
inline bool check_mod97(In &begin, const In &end, unsigned int nbr_digit)
{
  return false;
}

/** Compute the check digit of the number provided with the modulus 97 algorithm.
 * \tparam Iterator with at least the caracteristics of an input iterator. It represents the beginning or the ending of a sequence of character. 
 * \tparam unsigned int which represents a size.
 * \param [in] begin Represents the beginning of the sequence to check.
 * \param [in] end Represents one off the limit of the sequence to check.
 * \param [unsigned int] nbr_digits Represents the number of digits on which the modulus 97 algorithm will operate. If the size is < 1, the modulus 97 algorithm will calculate the check digit with all the digit encountered.
 * \pre begin and end are valid initialized iterators. They represent a sequence of character encoded in big-endian mode in a format compatible with the 7 bits ASCII.
 * \post begin is equal to the position of the last digit encountered plus one if the expression provided is correct, otherwise is equal to end.
 * \result 0 is returned if the expression given have not nbr_digits (or no digit if nbr_digits is equal to 0). Otherwise the ASCII characters of the check digits are returned.
 */

template <typename In>
inline char compute_mod97(In &begin, const In &end, unsigned int nbr_digit)
{
  return false;
}

} // namespace checks
} // namespace boost

#endif // #define BOOST_CHECK_MOD_INCLUDED

