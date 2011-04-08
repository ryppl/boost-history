/*  Boost IndexList library domain_index_list.hpp header file.
 *
 *                    Copyright Pierre-Andre Noel 2011.
 *         Distributed under the Boost Software License, Version 1.0.
 *            (See accompanying file LICENSE_1_0.txt or copy at
 *                  http://www.boost.org/LICENSE_1_0.txt)
 */
/** @file    domain_index_list.hpp
 *  @brief   An IndexList that knows if it is_valid.
 *  @author  Pierre-Andre Noel
 *  @since   2011-03-24
 *  @version $Id$
 *  @note    See http://www.boost.org/libs/index_list for library home page.
 */

#ifndef BOOST_INDEX_LIST_DOMAIN_INDEX_LIST_HPP
#define BOOST_INDEX_LIST_DOMAIN_INDEX_LIST_HPP

#include <boost/assert.hpp>

namespace boost
{
  template< typename Domain >
  class domain_index_list : public Domain::index_list_type
  {
    public:
      //  *** Repeat the domain types ***
      typedef Domain                                  domain_type;
      typedef typename Domain::dimension_type         dimension_type;
      typedef typename Domain::index_type             index_type;
      typedef typename Domain::width_type             width_type;
      typedef typename Domain::index_list_type        index_list_type;
    private:
      typedef index_list_type                         super;
      typedef domain_index_list< Domain >             self;
    public:
      //  *** Repeat the base class types ***
      typedef typename super::value_type              value_type;
      typedef typename super::iterator                iterator;
      typedef typename super::const_iterator          const_iterator;
      typedef typename super::reverse_iterator        reverse_iterator;
      typedef typename super::const_reverse_iterator  const_reverse_iterator;
      typedef typename super::reference               reference;
      typedef typename super::const_reference         const_reference;
      typedef typename super::size_type               size_type;
      typedef typename super::difference_type         difference_type;

      //  *** Constructors ***
      /// Constructor: receiving indices through an iterator.
      template< typename Iterator >
      domain_index_list(const domain_type& domain__, Iterator indices_iterator)
      : domain_(domain__)
      {
        ensure_indices_size();
        for (iterator i(this->begin()); i < this->end(); ++i,++indices_iterator)
          *i = *indices_iterator;
      }

      /// Constructor: setting indices to either first or past_the_last value.
      domain_index_list(const domain_type& domain__, bool first = true)
      : domain_(domain__)
      {
        // ensure_indices_size(); // Not required, see that function for why.
        if (first) set_to_first(); else set_to_past_the_last();
      }

      //  *** Queries ***
      /// Query: check if the current indices are valid.
      bool is_valid(void) const { return domain_.is_valid(*this); }

      /// Query: reduce the index_list to a single index.
      index_type reduce(void) const { return domain_.reduce(*this); }

      /// Query: number of different valid values.
      index_type num_elements(void) const { return domain_.num_elements(); }

      //  *** Modifiers ***
      /// Modifier: set the index_list using a reduced form single index.
      self& operator()(const index_type reduced)
      { *static_cast< super* >(this) = domain_.expand(reduced); return *this; }

      /// Modifier: set the index_list to its first value.
      self& set_to_first(void) { return (*this)(0); }

      /// Modifier: set the index_list to its last value.
      self& set_to_last(void){ return (*this)(domain_.num_elements()-1); }

      /// Modifier: set the index_list to one value past its last.
      self& set_to_past_the_last(void) {return (*this)(domain_.num_elements());}

      /// Modifier: pre-incrementation operator.
      self& operator++(void) { return (*this)(this->reduce() + 1); }

      /// Modifier: pre-decrementation operator.
      self& operator--(void) { return (*this)(this->reduce() - 1); }

      /// Modifier: post-incrementation operator.
      const self operator++(int) { self tmp(*this); ++(*this); return tmp; }

      /// Modifier: post-decrementation operator.
      const self operator--(int) { self tmp(*this); --(*this); return tmp; }

    private:
      //  *** Data members  ***
      const domain_type domain_;

      //  *** Factory functions ***
      void ensure_indices_size(void)
      {
        // Since set_to_first calls the operator= of the collection, this will
        // effectively resize the collection, if required.
        // Rationale: the Domain's index_collection_maker, if any, is private
        // and should remain so.
        set_to_first();
      }
  }; //  class domain_index_list

  //  *** Comparisons ***
  /// Comparison: less than.
  template< typename Domain >
  bool operator<(const domain_index_list< Domain >& x,
                 const domain_index_list< Domain >& y)
  { return x.reduce() < y.reduce(); }

  /// Comparison: greater than.
  template< typename Domain >
  bool operator>(const domain_index_list< Domain >& x,
                 const domain_index_list< Domain >& y) { return y < x; }

  /// Comparison: less or equal.
  template< typename Domain >
  bool operator<=(const domain_index_list< Domain >& x,
                  const domain_index_list< Domain >& y) { return !(y < x); }

  /// Comparison: greater or equal.
  template< typename Domain >
  bool operator>=(const domain_index_list< Domain >& x,
                  const domain_index_list< Domain >& y) { return !(x < y); }

} //  namespace boost

#endif //  BOOST_INDEX_LIST_DOMAIN_INDEX_LIST_HPP
