//  match.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_STATIC_STRING_MATCH_HPP
#define BOOST_STATIC_STRING_MATCH_HPP

#include <boost/static_string/static_string/basic_str.hpp>
#include <boost/static_string/static_string/ptree.hpp>
#include <boost/mpl/integral_c.hpp>

#include <ios>

namespace boost {
namespace static_string {

//template <typename Forest, typename InputIterator>
//int
//match(Forest f, InputIterator& b, InputIterator e, std::ios_base::iostate& err);

namespace static_string_detail {

template <typename InputIterator>
int
match(nil, InputIterator&, InputIterator)
{
    return -1;
}

template <typename CharT, CharT Head, typename Index, typename Forest, typename InputIterator>
int
match(basic_ptree<mpl::integral_c<CharT, Head>, Index, Forest>, InputIterator& i, InputIterator e)
{
    // an empty range doesn't match a tree
    if (i == e) return -1;
    // if the heads are the same 
    if (*i == Head) {
        // consume a character
        i++;
        // if we reached the end of the range return the Index of sub-tree 
        if (i == e) return Index::value;
        // if the next charecter is a white space
        if (*i == ' ') { 
            // consume it and return the Index of the sub-tree
            i++;
            return Index::value; 
        } else {
            int res=match(Forest(), i, e);
            if (res==-1)  return -2;
            else {
                // otherwise return the match
                return res;
            }
        }
    } else { 
        // otherwise, return no match and no character consumed
        return -1;
    }
}


// int match(T, InputIterator& i, InputIterator e);
// the result either identifies the match, or is 
// -1 meaning no match and no character were consumed
// -2 meaning no match and some character were consumed

template <typename Tree, typename Forest, typename InputIterator>
int
match(forest<Tree, Forest>, InputIterator& i, InputIterator e)
{
    // an empty range doesn't match a forest
    if (i == e) return -1;
    int res=match(Tree(), i, e);
    // if the range matches the first tree return the match
    if (res>=0) return res;
    // if the range doesn't matches the first tree and 
    // there were no consumer characters try with the other trees
    if (res==-1) return match(Forest(), i, e);
    else { 
        // otherwise, i.e. there were some character consumed return this
        return res;
    }
}




} // namespace static_string_detail

template <typename Forest, typename InputIterator>
int
match(Forest f, InputIterator& b, InputIterator e, std::ios_base::iostate& err)
{
   
    int res=static_string_detail::match(f, b, e);
    if (b == e)
        err |= std::ios_base::eofbit;
    if (res>=0) return res;
    err |= std::ios_base::failbit;
    return -1;
}

} // namespace static_string
} // namespace boost

#endif // BOOST_STATIC_STRING_MATCH_HPP
