//  Boost checks/ean.hpp header file  ------------------------------------//
//  (C) Copyright Pierre Talbot 2011
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_EAN_INCLUDED
#define BOOST_EAN_INCLUDED

namespace boost {
    namespace checks{

/** Check the validity of the European Article Numbering of size 8 (EAN-8) provided.
*/
template <class In>
inline bool Is_ean8(In ean_begin, In ean_end){}

/** Compute the check digit of the European Article Numbering of size 8 (EAN-8) provided.
*/
template <class In>
inline char ean8_check_digit(In ean_begin, In ean_end){}

/** Check the validity of the European Article Numbering of size 13 (EAN-13) provided.
 * \tparam In Iterator which represents the bound of a sequence of character.
 * \tparam Prefix Iterator which represents the bound of a sequence of EAN prefixes (GS1 country codes).
 * \param [in] ean_begin Represents the beginning of the EAN sequence to check.
 * \param [in] ean_end Represents one off the end of the EAN sequence to check.
 * \param [in] ean_prefix_begin Represents the beginning of the prefixes sequence to allow. Default : null, all the prefixes are allowed.
 * \param [in] ean_prefix_end Represents the ending of the prefixes sequence to allow. Default : null, all the prefixes are allowed.
 * \pre ean_begin and ean_end are valid initialized iterators. If ean_prefix_begin and ean_prefix_end are passed as arguments, they must be valid initialized iterators.
 * \post ean_begin, ean_end, ean_prefix_begin, and ean_prefix_end are unchanged.
 * \return True if the EAN delimited by ean_begin and ean_end is a valid EAN of size 13 with a prefix
 */
template <class In, class Prefix>
inline bool Is_ean13(In ean_begin, In ean_end, Prefix ean_prefix_begin = null, Prefix ean_prefix_end = null){}

/** Compute the check digit of the European Article Numbering of size 13 (EAN-13) provided.
*/
template <class In>
inline char ean13_check_digit(In ean_begin, In ean_end){}

} // namespace checks
} // namespace boost

#endif
