
// Copyright Roland Richter 2001-2004.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_VIEW_PERMUTATION_VIEW_HPP
#define BOOST_VIEW_PERMUTATION_VIEW_HPP

#include <boost/iterator/permutation_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include "detail/ownership_detail.hpp"


namespace boost {
  namespace view {
  
/**
 * @brief A view which applies a reindexing scheme to the elements of the underlying container.
 *
 * <h2>Template paramters</h2>
 * @param ContainerT The type of the underlying container.
 * @param ReindexT   Another container type containing the reindexing scheme.
 */
template<class ContainerT, class ReindexT>
class permutation_view
{
public:
  /// The view's own type.
  typedef permutation_view<ContainerT,ReindexT> self_type;
          
  /// The type of the underlying container.
  typedef ownership::wrap<ContainerT>::domain domain_type;

  /// The type of the reindexing scheme.
  typedef ReindexT reindex_type;
          
  /// @name The traits types visible to the public.
  //@{           

  typedef boost::permutation_iterator<
              typename ownership::wrap<ContainerT>::domain::iterator
            , typename ReindexT::iterator
                                     > iterator;
    
  typedef typename boost::iterator_value<iterator>::type      value_type;
  typedef typename boost::iterator_reference<iterator>::type  reference;
  typedef typename boost::iterator_pointer<iterator>::type    pointer;
  typedef typename boost::iterator_difference<iterator>::type difference_type;

  typedef boost::permutation_iterator<
              typename ownership::wrap<ContainerT>::domain::const_iterator
            , typename ReindexT::const_iterator
                                     > const_iterator;
    
  typedef typename boost::iterator_reference<const_iterator>::type  const_reference;
  typedef typename boost::iterator_pointer<const_iterator>::type    const_pointer;

  typedef typename ReindexT::size_type                       size_type;
  typedef typename ReindexT::size_type                       index_type;
  typedef typename boost::iterator_reference<iterator>::type data_type;
  //@}          


  /// Creates a view of the given container and the given reindexing scheme.
  permutation_view( const domain_type& theData, const reindex_type& theReindex )
    : data( theData ), reindex( theReindex )
  { }

  /// The copy constructor.
  permutation_view( const permutation_view& rhs )
    : data( rhs.data ), reindex( rhs.reindex )
  { }

  /// The destructor.
  ~permutation_view()
  { }

  /// Returns true iff the view's size is 0.
  bool      empty() const { return reindex.empty(); }

  /// Returns the size of the view.
  size_type size()  const { return reindex.size(); }

  /// Returns a \c const_iterator pointing to the begin of the view.
  const_iterator begin() const { return const_iterator( data->begin(), reindex.begin() ); }
  /// Returns an \c iterator pointing to the begin of the view.
  iterator       begin()       { return iterator( data->begin(), reindex.begin() ); }

  /// Returns a \c const_iterator pointing to the end of the view.
  const_iterator end() const { return const_iterator( data->begin(), reindex.end() ); }
  /// Returns an \c iterator pointing to the end of the view.
  iterator       end()       { return iterator( data->begin(), reindex.end() ); }

  /// Returns a const_reference for the first element of the view.
  const_reference front() const { return *(begin()); }
  /// Returns a reference for the first element of the view.
  reference       front()       { return *(begin()); }
  /// Returns a const_reference for the last element in the view.
  const_reference back()  const { const_iterator tmp = end(); return *(--tmp); }
  /// Returns a reference for the last element in the view.
  reference       back()        { iterator tmp = end();       return *(--tmp); }

  /// @name Only supported if both container and reindex scheme are random access:
  //@{
  /// Returns the n'th element of the view.
  const data_type operator[](index_type n) const { return (*data)[ reindex[n] ]; }
  /// Returns the n'th element of the view.
        data_type operator[](index_type n)       { return (*data)[ reindex[n] ]; }
  //@}

private:
  ownership::wrap<ContainerT>::type data;
  reindex_type                      reindex;
};

  } // namespace view
} // namespace boost

#endif
