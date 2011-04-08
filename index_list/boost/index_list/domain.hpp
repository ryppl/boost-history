/*  Boost IndexList library domain.hpp header file.
 *
 *                    Copyright Pierre-Andre Noel 2011.
 *         Distributed under the Boost Software License, Version 1.0.
 *            (See accompanying file LICENSE_1_0.txt or copy at
 *                  http://www.boost.org/LICENSE_1_0.txt)
 */
/** @file    domain.hpp
 *  @brief   Some common domains for IndexList.
 *  @author  Pierre-Andre Noel
 *  @since   2011-03-23
 *  @version $Id$
 *  @note    See http://www.boost.org/libs/index_list for library home page.
 */

#ifndef BOOST_INDEX_LIST_DOMAIN_HPP
#define BOOST_INDEX_LIST_DOMAIN_HPP

#include <boost/assert.hpp>
#include <algorithm>
#include <utility>
#include <vector>

namespace boost
{
  namespace index_list
  {
    /// Domain for indices contained within an (hyper)box.
    template
    <
      typename Collection_Maker,
      typename Index_Type = std::ptrdiff_t,
      typename Width_Type = std::size_t
    >
    class box_domain
    {
      public:
        //  *** Base types ***
        typedef std::size_t dimension_type;
        typedef Index_Type index_type;
        typedef Width_Type width_type;
        //  *** Related to the index_list_type returned by expand() ***
      private:
        typedef typename Collection_Maker::template collection_of<index_type>
          index_collection_maker;
      public:
        typedef typename index_collection_maker::type index_list_type;

        //  *** Constructors ***
        /// Constructor: default index_bases, default storage order.
        template< typename It >
        box_domain(const dimension_type dim, It shape__)
        : num_dimensions_(dim), ordering_(dim), ascending_(dim), shape_(dim),
          index_bases_(dim), index_tops_(dim), strides_(dim)
        {
          BOOST_ASSERT(dim > 0);
          set_shape_default_index_bases(shape__);
          set_c_storage_order();
        }

        /// Constructor: default storage order.
        template< typename It1, typename It2 >
        box_domain(const dimension_type dim, It1 shape__, It2 index_bases__)
        : num_dimensions_(dim), ordering_(dim), ascending_(dim), shape_(dim),
          index_bases_(dim), index_tops_(dim), strides_(dim)
        {
          BOOST_ASSERT(dim > 0);
          set_shape_index_bases(shape__,index_bases__);
          set_c_storage_order();
        }

        /// Constructor: non-default storage order.
        template< typename It1, typename It2, typename It3, typename It4 >
        box_domain(const dimension_type dim, It1 shape__, It2 index_bases__,
                   It3 ordering__, It4 ascending__)
        : num_dimensions_(dim), ordering_(dim), ascending_(dim), shape_(dim),
          index_bases_(dim), index_tops_(dim), strides_(dim)
        {
          BOOST_ASSERT(dim > 0);
          set_shape_index_bases(shape__,index_bases__);
          set_ordering_and_ascending(ordering__,ascending__);
        }

        /// Constructor: infer storage order from strides.
        template< typename It1, typename It2, typename It3 >
        box_domain(const dimension_type dim, It1 shape__, It2 index_bases__,
                   It3 strides__)
        : num_dimensions_(dim), ordering_(dim), ascending_(dim), shape_(dim),
          index_bases_(dim), index_tops_(dim), strides_(dim)
        {
          BOOST_ASSERT(dim > 0);
          set_shape_index_bases(shape__,index_bases__);
          set_strides(strides__);
        }

        /// Constructor: initialization from a MultiArray.
        template< typename MultiArray >
        box_domain(const MultiArray& multi_array__)
        : num_dimensions_(multi_array__.num_dimensions()),
          ordering_(num_dimensions_), ascending_(num_dimensions_),
          shape_(num_dimensions_), index_bases_(num_dimensions_),
          index_tops_(num_dimensions_), strides_(num_dimensions_)
        {
          BOOST_ASSERT(num_dimensions_ > 0);
          set_shape_index_bases(multi_array__.shape(),
                                multi_array__.index_bases());
          set_strides(multi_array__.strides());
        }

        //  *** Queries ***
        /// Query: check if an IndexCollection is within allowed range.
        template< typename IndexCollection >
        bool is_valid(const IndexCollection& x) const
        {
          BOOST_ASSERT(x.size() == num_dimensions_);
          for (dimension_type i(0); i < num_dimensions_; ++i)
          {
            if ( (x[i]<index_bases_[i]) || (index_tops_[i]<x[i]) ) return false;
          }
          return true;
        }

        /// Query: reduce an IndexCollection to a single index_type.
        template< typename IndexCollection >
        index_type reduce(const IndexCollection& x) const
        {
          BOOST_ASSERT(x.size() == num_dimensions_);
          index_type out(0);
          for (dimension_type i(0); i < num_dimensions_; ++i)
          {
            out += (ascending_[i] ? x[i]-index_bases_[i] : index_tops_[i]-x[i])
                    *strides_[i];
          }
          return out;
        }

        /// Query: expand an index_type into an index_list_type.
        index_list_type expand(index_type reduced) const
        {
          index_list_type out=index_collection_maker::make(num_dimensions_);
          index_type tmp;
          for (typename std::vector<dimension_type>::const_reverse_iterator
                 d_it(ordering_.rbegin()),d_rend(ordering_.rend());
                 d_it < d_rend; ++d_it)
          {
            tmp = reduced / strides_[*d_it];
            out[*d_it] = (ascending_[*d_it] ? index_bases_[*d_it] :
                                       index_tops_[*d_it]) + tmp;
            reduced -= tmp*strides_[*d_it];
          }
          return out;
        }

        /// Query: number of dimensions.
        dimension_type num_dimensions(void) const {return num_dimensions_;}

        /// Query: order of the dimensions (0 = LSD).
        dimension_type ordering(const dimension_type i) const {return ordering_[i];}

        /// Query: direction of change of each dimension (true = ascending).
        bool ascending(const dimension_type i) const { return ascending_[i]; }

        /// Query: shape ("lenght" of the box's sides).
        const width_type* shape(void) const { return &shape_.front(); }

        /// Query: lower value each index can take (a corner of the box).
        const index_type* index_bases(void) const {return &index_bases_.front();}

        /// Query: higher value each index can take (other corner of the box).
        const index_type* index_tops(void) const {return &index_tops_.front();}

        /// Query: strides (effect on "reduce()" of an increase of each index).
        const index_type* strides(void) const {return &strides_.front();}

        /// Query: total number of valid elements in the domain.
        index_type num_elements(void) const { return num_elements_; }

      private:
        //  *** Data members (should not change, except at construction) ***
        const dimension_type num_dimensions_;
        std::vector< dimension_type > ordering_;
        std::vector< bool > ascending_;
        std::vector< width_type > shape_;
        std::vector< index_type > index_bases_;
        std::vector< index_type > index_tops_;
        std::vector< index_type > strides_;
        index_type num_elements_;

        //  *** Factory functions ***
        template< typename It >
        void set_shape_default_index_bases(It shape__)
        {
          for (dimension_type i(0);i<num_dimensions_;++i,++shape__)
          {
            shape_[i] = *shape__;
            index_bases_[i] = 0;
            index_tops_[i] = static_cast<index_type>(*shape__) - 1;
          }
        }

        template< typename It1, typename It2 >
        void set_shape_index_bases(It1 shape__, It2 index_bases__)
        {
          for (dimension_type i(0);i<num_dimensions_;++i,++shape__,++index_bases__)
          {
            shape_[i] = *shape__;
            index_bases_[i] = *index_bases__;
            index_tops_[i] = *index_bases__ + *shape__ - 1;
          }
        }

        void set_c_storage_order(void)
        {
          for (dimension_type i(0);i<num_dimensions_;++i)
          {
            ordering_[i] = num_dimensions_ - 1 - i;
            ascending_[i] = true;
          }
          infer_strides_and_num_elements();
        }

        template< typename It1, typename It2 >
        void set_ordering_and_ascending(It1 ordering__, It2 ascending__)
        {
          for (dimension_type i(0);i<num_dimensions_;++i,++ordering__,++ascending__)
          {
            ordering_[i] = *ordering__;
            ascending_[i] = *ascending__;
          }
          infer_strides_and_num_elements();
        }

        void infer_strides_and_num_elements(void)
        {
          num_elements_ = 1;
          for (typename std::vector<dimension_type>::const_iterator
               d_it(ordering_.begin()),d_end(ordering_.end());
               d_it < d_end; ++d_it)
          {
            strides_[*d_it]=(ascending_[*d_it]?num_elements_:-num_elements_);
            num_elements_ *= shape_[*d_it];
          }
        }

        template< typename It >
        void set_strides(It strides__)
        {
          typedef std::pair< index_type, dimension_type > pair_type;
          typedef typename std::vector<pair_type> pair_vector_type;
          pair_vector_type pair_collection(num_dimensions_);

          num_elements_ = 1;
          for (dimension_type i(0); i < num_dimensions_; ++i, ++strides__)
          {
            strides_[i] = *strides__;
            ascending_[i] = (0 < *strides__);
            pair_collection[i].first = (ascending_[i]?*strides__:-*strides__);
            pair_collection[i].second = i;
          }
          std::sort(pair_collection.begin(),pair_collection.end());
          for (dimension_type i(0); i < num_dimensions_; ++i)
          {
            ordering_[i] = pair_collection[i].second;
            bool strides_ok(num_elements_==(ascending_[ordering_[i]] ?
                                            strides_[ordering_[i]]   :
                                            -strides_[ordering_[i]]));
            BOOST_ASSERT(strides_ok);
            num_elements_ *= shape_[ordering_[i]];
          }
        }
    };  //  class box_domain

//  ***************************************************************************

    /** Domain based on (hyper)planes defined by fixed Manhattan distances.
     *
     *  Assuming the ordering \f$\mathbf{x}=[x_{d-1},x_{d-2},\ldots x_1,x_0]\f$
     *  for the indices and defining \f$s_0, s_1, \ldots, s_{d-1}\f$ such that
     *  \f$s_i = \sum_{i = 0}^{d-1} x_i\f$, the effect of reduce() corresponds
     *  to the sum \f$\sum_{j=0}^d {s_j + j \choose j+1}\f$. Each possible
     *  binomial contribution is tabulated at construction. For reduce() alone,
     *  producing a table would not have been necessary. However, expand()
     *  would be very slow without it.
     *
     *  Alternatives and comments are welcomed.
     */
    template
    <
      typename Collection_Maker,
      typename Index_Type = std::ptrdiff_t,
      typename Width_Type = std::size_t
    >
    class simplex_domain
    {
      public:
        //  *** Base types ***
        typedef std::size_t dimension_type;
        typedef Index_Type index_type;
        typedef Width_Type width_type;
        //  *** Related to the index_list_type returned by expand() ***
      private:
        typedef typename Collection_Maker::template collection_of<index_type>
          index_collection_maker;
      public:
        typedef typename index_collection_maker::type index_list_type;

        //  *** Constructors ***
        /// Constructor: default index_bases, default order, default planes.
        simplex_domain(const dimension_type dim, const dimension_type edge__)
        : num_dimensions_(dim), ordering_(dim), ascending_(dim,true),
          shape_(dim,edge__), index_bases_(dim,0), index_tops_(dim,edge__-1),
          edge_(edge__), table_(dim), min_plane_(0), max_plane_(edge__-1)
        {
          basic_tests();
          set_default_ordering();
          make_table();
        }

        /// Constructor: default index_bases, default order.
        simplex_domain(const dimension_type dim, const dimension_type edge__,
                       const dimension_type min_plane__,
                       const dimension_type max_plane__)
        : num_dimensions_(dim), ordering_(dim), ascending_(dim,true),
          shape_(dim,edge__), index_bases_(dim,0), index_tops_(dim,edge__-1),
          edge_(edge__), table_(dim), min_plane_(min_plane__),
          max_plane_(max_plane__)
        {
          basic_tests();
          set_default_ordering();
          make_table();
        }

        /// Constructor: default order.
        template< typename It >
        simplex_domain(const dimension_type dim, const dimension_type edge__,
                       const dimension_type min_plane__,
                       const dimension_type max_plane__, It index_bases__)
        : num_dimensions_(dim), ordering_(dim), ascending_(dim,true),
          shape_(dim,edge__), index_bases_(dim), index_tops_(dim),
          edge_(edge__), table_(dim), min_plane_(min_plane__),
          max_plane_(max_plane__)
        {
          basic_tests();
          set_index_bases(index_bases__);
          set_default_ordering();
          make_table();
        }

        /// Constructor: non-default order.
        template< typename It1, typename It2, typename It3 >
        simplex_domain(const dimension_type dim, const dimension_type edge__,
                       const dimension_type min_plane__,
                       const dimension_type max_plane__, It1 index_bases__,
                       It2 ordering__, It3 ascending__)
        : num_dimensions_(dim), ordering_(dim), ascending_(dim,true),
          shape_(dim,edge__), index_bases_(dim), index_tops_(dim),
          edge_(edge__), table_(dim), min_plane_(min_plane__),
          max_plane_(max_plane__)
        {
          basic_tests();
          set_index_bases(index_bases__);
          set_ordering_and_ascending(ordering__,ascending__);
          make_table();
        }

        /// Constructor: initialization from a MultiArray.
        template< typename MultiArray >
        simplex_domain(const MultiArray& multi_array__)
        : num_dimensions_(multi_array__.num_dimensions()),
          ordering_(num_dimensions_), ascending_(num_dimensions_),
          shape_(num_dimensions_), index_bases_(num_dimensions_),
          index_tops_(num_dimensions_), edge_(multi_array__.shape()[0]),
          table_(num_dimensions_), min_plane_(0), max_plane_(edge_-1)
        {
          basic_tests();
          initialize_from_multi_array(multi_array__);
        }

        /// Constructor: initialization from a MultiArray (with planes).
        template< typename MultiArray >
        simplex_domain(const MultiArray& multi_array__,
                       const dimension_type min_plane__,
                       const dimension_type max_plane__)
        : num_dimensions_(multi_array__.num_dimensions()),
          ordering_(num_dimensions_), ascending_(num_dimensions_),
          shape_(num_dimensions_), index_bases_(num_dimensions_),
          index_tops_(num_dimensions_), edge_(multi_array__.shape()[0]),
          table_(num_dimensions_), min_plane_(min_plane__),
          max_plane_(max_plane__)
        {
          basic_tests();
          initialize_from_multi_array(multi_array__);
        }

        //  *** Domain queries ***
        /// Query: check if an IndexCollection is within allowed range.
        template< typename IndexCollection >
        bool is_valid(const IndexCollection& x) const
        {
          BOOST_ASSERT(x.size() == num_dimensions_);
          index_type s(0);
          for (dimension_type i(0); i < num_dimensions_; ++i)
          {
            if ( (x[i]<index_bases_[i]) || (index_tops_[i]<x[i]) ) return false;
            s += x[i];
          }
          if ( (s < static_cast< index_type >(min_plane_)) ||
               (static_cast< index_type >(max_plane_) < s) ) return false;
          return true;
        }

        /// Query: reduce an IndexCollection to a single index_type.
        template< typename IndexCollection >
        index_type reduce(const IndexCollection& x) const
        {
          BOOST_ASSERT(x.size() == num_dimensions_);
          index_type s(0);
          index_type out(-table_[0][min_plane_]);
          for (index_type i(num_dimensions_-1); i >= 0; --i)
          {
            s +=
            ((
              ascending_[ordering_[i]]                     ?
              x[ordering_[i]] - index_bases_[ordering_[i]] :
              index_tops_[ordering_[i]] - x[ordering_[i]]
            ));
            out += table_[i][s];
          }
          return out;
        }

        /// Query: expand an index_type into an index_list_type.
        index_list_type expand(index_type reduced) const
        {
          reduced += table_[0][min_plane_];
          index_list_type out=index_collection_maker::make(num_dimensions_);
          std::vector< index_type > s(num_dimensions_);
          for (dimension_type i(0); i < (num_dimensions_-1); ++i)
          {
            s[i] = static_cast< index_type >
            (
              std::upper_bound(table_[i].begin(),table_[i].end(),reduced)
              - table_[i].begin() - 1
            );
            reduced -= table_[i][s[i]];
          }
          s[num_dimensions_-1] = reduced;
          out[ordering_[num_dimensions_-1]] =
          (
            ascending_[ordering_[num_dimensions_-1]]             ?
            index_bases_[ordering_[num_dimensions_-1]] + reduced :
            index_tops_[ordering_[num_dimensions_-1]] - reduced
          );
          for (dimension_type i(0); i < (num_dimensions_-1); ++i)
          {
            out[ordering_[i]] =
            (
              ascending_[ordering_[i]] ?
              index_bases_[ordering_[i]] + s[i] - s[i+1] :
              index_tops_[ordering_[i]] - s[i] + s[i+1]
            );
          }
          return out;
        }

        //  *** Other queries ***
        /// Query: number of dimensions.
        dimension_type num_dimensions(void) const {return num_dimensions_;}

        /// Query: order of the dimensions.
        dimension_type ordering(const dimension_type i) const {return ordering_[i];}

        /// Query: direction of change of each dimension (true = ascending).
        bool ascending(const dimension_type i) const { return ascending_[i]; }

        /// Query: the lowest plane.
        dimension_type min_plane(void) const { return min_plane_; }

        /// Query: the highest plane.
        dimension_type max_plane(void) const { return max_plane_; }

        /// Query: shape ("lenght" of the box's sides).
        const width_type* shape(void) const { return &shape_.front(); }

        /// Query: lower value each index can take (a corner of the box).
        const index_type* index_bases(void) const {return &index_bases_.front();}

        /// Query: higher value each index can take (other corner of the box).
        const index_type* index_tops(void) const {return &index_tops_.front();}

        /// Query: total number of valid elements in the domain.
        index_type num_elements(void) const { return num_elements_; }

      private:
        //  *** Data members (should not change, except at construction) ***
        const dimension_type num_dimensions_;
        std::vector< dimension_type > ordering_;
        std::vector< bool > ascending_;
        std::vector< width_type > shape_;
        std::vector< index_type > index_bases_;
        std::vector< index_type > index_tops_;
        const dimension_type edge_;
        std::vector< std::vector< index_type > > table_;
        index_type num_elements_;
        const dimension_type min_plane_;
        const dimension_type max_plane_;

        //  *** Factory functions ***
        void basic_tests(void)
        {
          BOOST_ASSERT(0 < num_dimensions_);
          BOOST_ASSERT(0 < edge_);
          BOOST_ASSERT(0 <= min_plane_);
          BOOST_ASSERT(max_plane_ < edge_);
          BOOST_ASSERT(min_plane_ <= max_plane_);
        }

        void set_default_ordering(void)
        {
          for (dimension_type i(0); i < num_dimensions_; ++i)
            ordering_[i] = num_dimensions_ - 1 - i;
        }

        template< typename It >
        void set_index_bases(It index_bases__)
        {
          for (dimension_type i(0); i < num_dimensions_ ; ++i, ++index_bases__)
          {
            index_bases_[i] = *index_bases__;
            index_tops_[i] = *index_bases__ + edge_ - 1;
          }
        }

        template< typename It1, typename It2 >
        void set_ordering_and_ascending(It1 ordering__, It2 ascending__)
        {
          for (dimension_type i(0);i<num_dimensions_;++i,++ordering__,++ascending__)
          {
            ordering_[i] = *ordering__;
            ascending_[i] = *ascending__;
          }
        }

        void make_table(void)
        {
          for (dimension_type i(0); i < num_dimensions_; ++i)
          {
            std::vector< index_type >& current = table_[num_dimensions_-1-i];
            current.resize(edge_+1);
            current[1] = 1;
            for (index_type s(2); s <= static_cast< index_type >(edge_); ++s)
              current[s] = current[s-1] * (s+i) / (s-1);
          }
          num_elements_ = table_[0][max_plane_+1] - table_[0][min_plane_];
        }

        template< typename MultiArray >
        void initialize_from_multi_array(const MultiArray& multi_array__)
        {
          //  Ask a box_domain for conversions...
          box_domain<Collection_Maker,Index_Type,Width_Type> box(multi_array__);
          for (dimension_type i(0); i < num_dimensions_; ++i)
          {
            BOOST_ASSERT(box.shape()[i] == edge_);
            ordering_[i] = box.ordering(i);
            ascending_[i] = box.ascending(i);
            shape_[i] = box.shape()[i];
            index_bases_[i] = box.index_bases()[i];
            index_tops_[i] = box.index_tops()[i];
          }
          make_table();
        }

    };  //  class simplex_domain

  }  //  namespace index_list
} //  namespace boost

#endif //  BOOST_INDEX_LIST_DOMAIN_HPP
