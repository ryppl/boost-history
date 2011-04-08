/*  Boost IndexList library multi_array_utility.hpp header file.
 *
 *                    Copyright Pierre-Andre Noel 2011.
 *         Distributed under the Boost Software License, Version 1.0.
 *            (See accompanying file LICENSE_1_0.txt or copy at
 *                  http://www.boost.org/LICENSE_1_0.txt)
 */
/** @file    multi_array_utility.hpp
 *  @brief   Utilities for use with Boost.MultiArray.
 *  @author  Pierre-Andre Noel
 *  @since   2011-04-01
 *  @version $Id$
 *  @note    See http://www.boost.org/libs/index_list for library home page.
 */

#ifndef BOOST_INDEX_LIST_MULTI_ARRAY_UTILITY_HPP
#define BOOST_INDEX_LIST_MULTI_ARRAY_UTILITY_HPP

#include <boost/iterator/iterator_facade.hpp>

#include <boost/collection_maker/array.hpp>
#include <boost/index_list/domain.hpp>
#include <boost/index_list/domain_index_list.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>

namespace boost
{
  namespace index_list
  {
    namespace details
    {
      // Iterator class for accessing a MultiArray as specified in a Domain.
      template< typename MultiArray, typename Domain, typename Element >
      class multi_array_iter
      : public boost::iterator_facade
      <
        multi_array_iter< MultiArray,Domain,Element >,
        Element,
        random_access_traversal_tag
      >
      {
        private:
          typedef typename Domain::index_type index_type;
          struct enabler {};

        public:
          typedef typename Domain::index_list_type index_list_type;

          //  *** Constructors ***
          // Constructor: default (Domain is default-constructible).
          multi_array_iter() : p_multi_array_(0), pos_(0) {}

          // Constructor: begin or end (Domain initializable from a MultiArray).
          multi_array_iter(MultiArray& multi_array__, const bool begin__ = true)
          : p_multi_array_(&multi_array__), domain_(multi_array__)
          { if (begin__) pos_ = 0; else pos_ = domain_.num_elements(); }

          // Constructor: begin or end.
          multi_array_iter(MultiArray& multi_array__, const Domain& domain__,
                           const bool begin__ = true)
          : p_multi_array_(&multi_array__), domain_(domain__)
          { if (begin__) pos_ = 0; else pos_ = domain_.num_elements(); }

          // Constructor: interoperability.
          template< typename MultiArray_, typename Element_ >
          multi_array_iter
          (
            const multi_array_iter<MultiArray_,Domain,Element_>& other,
            typename boost::enable_if
            <
              boost::mpl::and_
              <
                boost::is_convertible< MultiArray_*, MultiArray* >,
                boost::is_convertible< Element_*, Element* >
              >,
              enabler
            >::type = enabler()
          )
          : p_multi_array_(other.p_multi_array_), domain_(other.domain_),
            pos_(other.pos_) {}

          // Query: get the indices corresponding to the current element.
          index_list_type indices() const { return domain_.expand(pos_); }

          template< typename MultiArray_, typename Element_ >
          typename boost::enable_if
          <
            boost::mpl::and_
            <
              boost::is_convertible< MultiArray_*, MultiArray* >,
              boost::is_convertible< Element_*, Element* >
            >,
            bool
          >::type
          operator<(const multi_array_iter<MultiArray_,Domain,Element_>& other)
          const { return this->pos_ < other.pos_; }

          template< typename MultiArray_, typename Element_ >
          typename boost::enable_if
          <
            boost::mpl::and_
            <
              boost::is_convertible< MultiArray_*, MultiArray* >,
              boost::is_convertible< Element_*, Element* >
            >,
            bool
          >::type
          operator>(const multi_array_iter<MultiArray_,Domain,Element_>& other)
          const { return this->pos_ > other.pos_; }

          template< typename MultiArray_, typename Element_ >
          typename boost::enable_if
          <
            boost::mpl::and_
            <
              boost::is_convertible< MultiArray_*, MultiArray* >,
              boost::is_convertible< Element_*, Element* >
            >,
            bool
          >::type
          operator<=(const multi_array_iter<MultiArray_,Domain,Element_>& other)
          const { return this->pos_ <= other.pos_; }

          template< typename MultiArray_, typename Element_ >
          typename boost::enable_if
          <
            boost::mpl::and_
            <
              boost::is_convertible< MultiArray_*, MultiArray* >,
              boost::is_convertible< Element_*, Element* >
            >,
            bool
          >::type
          operator>=(const multi_array_iter<MultiArray_,Domain,Element_>& other)
          const { return this->pos_ >= other.pos_; }

        private:
          //  *** Members ***
          MultiArray* p_multi_array_;
          Domain domain_;
          index_type pos_;

          //  *** Core operations ***
          friend class boost::iterator_core_access;

          Element& dereference() const
          { return (*p_multi_array_)(domain_.expand(pos_)); }

          template< typename MultiArray_, typename Element_ >
          typename boost::enable_if
          <
            boost::mpl::and_
            <
              boost::is_convertible< MultiArray_*, MultiArray* >,
              boost::is_convertible< Element_*, Element* >
            >,
            bool
          >::type
          equal(const multi_array_iter<MultiArray_,Domain,Element_>& other) const
          { return this->pos_ == other.pos_; }

          void increment() { ++pos_; }

          void decrement() { --pos_; }

          void advance(const index_type n) { pos_ += n; }

          template< typename MultiArray_, typename Element_ >
          typename boost::enable_if
          <
            boost::mpl::and_
            <
              boost::is_convertible< MultiArray_*, MultiArray* >,
              boost::is_convertible< Element_*, Element* >
            >,
            bool
          >::index_type
          distance_to(const multi_array_iter<MultiArray_,Domain,Element_>& other)
          const { return other.pos_ - this->pos_; }
      }; //  class multi_array_iter
    } //  namespace details

//  **************************************************************************

    /// Expose some useful MultiArray-related types.
    template< typename MultiArray >
    class multi_array_types
    {
      private:
        typedef boost::collection_maker::array< MultiArray::dimensionality >
          collection_maker_type;

      public:
        typedef MultiArray multi_array_type;
        typedef typename MultiArray::index index_type;
        typedef typename MultiArray::size_type dimension_type;

        template< typename Domain >
        class other
        {
          public:
            typedef Domain domain_type;
            typedef domain_index_list<domain_type> domain_index_list_type;
            typedef details::multi_array_iter
              <MultiArray,domain_type,typename MultiArray::element> iterator;
            typedef details::multi_array_iter
              <MultiArray const,domain_type,typename MultiArray::element const>
              const_iterator;

            static iterator begin(MultiArray& m) { return iterator(m,true); }
            static iterator begin(MultiArray& m, const Domain& d)
            { return iterator(m,d,true); }

            static iterator end(MultiArray& m) { return iterator(m,false); }
            static iterator end(MultiArray& m, const Domain& d)
            { return iterator(m,d,false); }

            static const_iterator begin(const MultiArray& m)
            { return const_iterator(m,true); }
            static const_iterator begin(const MultiArray& m, const Domain& d)
            { return const_iterator(m,d,true); }

            static const_iterator end(const MultiArray& m)
            { return const_iterator(m,false); }
            static const_iterator end(const MultiArray& m, const Domain& d)
            { return const_iterator(m,d,false); }
        };

        typedef other
          < box_domain<collection_maker_type,index_type,dimension_type> > box;

        typedef other
          < simplex_domain<collection_maker_type,index_type,dimension_type> >
          simplex;

    };

  }  //  namespace index_list
} //  namespace boost

#endif //  BOOST_INDEX_LIST_MULTI_ARRAY_UTILITY_HPP
