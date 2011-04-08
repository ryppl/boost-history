/*  Example file example.cpp for the Boost IndexList library.
 *
 *                    Copyright Pierre-Andre Noel 2011.
 *         Distributed under the Boost Software License, Version 1.0.
 *            (See accompanying file LICENSE_1_0.txt or copy at
 *                  http://www.boost.org/LICENSE_1_0.txt)
 */
/** @file    example.cpp
 *  @brief   Examples for the Boost IndexList library.
 *  @author  Pierre-Andre Noel
 *  @since   2011-04-04
 *  @version $Id$
 *  @note    See http://www.boost.org/libs/index_list for library home page.
 */

#include <iostream>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include <boost/index_list/multi_array_utility.hpp>
#include <boost/collection_maker/vector.hpp>

using std::cout;
using std::endl;

//  Display indices in the form "(i,j,k)".
template< typename Collection >
void display_indices(const Collection& c)
{
  cout << "(";
  for (typename Collection::const_iterator it(c.begin());;)
  {
    cout << *it;
    ++it;
    if (it < c.end())
      cout << ",";
    else
      break;
  }
  cout << ")";
}

// ***************************************************************************
//  Boost.MultiArray examples without Boost.IndexList.

/*
 *  The elements of a MultiArray can all be directly accessed using the data()
 *  and num_elements() members. This method is useful when the task to be
 *  performed on/with the element does not depend on its position in the
 *  MultiArray (except perhaps for its location in memory). Note that this
 *  method of access is independent of the dimensionality of the MultiArray.
 */
template< typename MultiArray >
void multi_array_without_index_list_using_data_and_num_elements_anyD
  (const MultiArray& m)
{
  cout << "Without index_list using data() and num_elements()." << endl;
  typedef typename MultiArray::size_type size_type;
  typedef typename MultiArray::element element_type;

  for (size_type i(0); i < m.num_elements(); ++i)
  {
    const element_type& e( m.data()[i] );
    //  We could now use "e" to do whatever we want to do with the elements of
    //  the MultiArray. Sadly, the indices corresponding to the position of
    //  the current element in the MultiArray (although we do know their
    //  position in memory).

    //  Display the elements together with their position in memory.
    cout << "(" << i << "):" << e << " ";
  }
  cout << endl << endl;
}

/*
 *  A MultiArray provides iterators that grant access to a MultiArray with one
 *  dimension less (except the last one, which grant access to the element).
 *  Note that the dimensionality (here 3) of the MultiArray has to be hard-coded
 *  through the number of for-loops required.
 */
template< typename MultiArray >
void multi_array_without_index_list_using_iterators_3D(const MultiArray& m)
{
  cout << "Without index_list using iterators and 3 for-loops." << endl;
  typedef typename MultiArray::element element_type;
  typedef typename MultiArray::const_iterator it0_type;
  typedef typename MultiArray::value_type::const_iterator it1_type;
  typedef typename MultiArray::value_type::value_type::const_iterator it2_type;

  for (it0_type it0(m.begin()); it0 < m.end(); ++it0)
  {
    for (it1_type it1(it0->begin()); it1 < it0->end(); ++it1)
    {
      for (it2_type it2(it1->begin()); it2 < it1->end(); ++it2)
      {
        const element_type& e( *it2 );
        //  We could now use "e" to do whatever we want to do with the elements
        //  of the MultiArray. Again, unless we add some additional counters
        //  that are incremented at the same time as the iterators, we do not
        //  have access to the corresponding indices.

        //  Display the elements.
        cout << e << " ";
      }
    }
  }
  cout << endl << endl;
}

/*
 *  A MultiArray also provides operator[] that grants access to a MultiArray
 *  with one dimension less (except the last one, which grant access to the
 *  element). Again, the dimensionality of the multi_array has to be hard-coded
 *  through the number of for-loops required.
 */
template< typename MultiArray >
void multi_array_without_index_list_using_indices_3D(const MultiArray& m)
{
  cout << "Without index_list using indices and 3 for-loops." << endl;
  typedef typename MultiArray::index index_type;
  typedef typename MultiArray::element element_type;
  for (index_type i(m.index_bases()[0]);
       i < static_cast<index_type>(m.index_bases()[0] + m.shape()[0]); ++i)
  {
    for (index_type j(m.index_bases()[1]);
         j < static_cast<index_type>(m.index_bases()[1] + m.shape()[1]); ++j)
    {
      for (index_type k(m.index_bases()[2]);
           k < static_cast<index_type>(m.index_bases()[2] + m.shape()[2]); ++k)
      {
        const element_type& e( m[i][j][k] );
        //  We could now use "e" to do whatever we want to do with the elements
        //  of the MultiArray. This time, the indices are available!

        //  Display the elements together with the corresponding indices.
        cout << "(" << i << "," << j << "," << k << "):" << e << " ";
      }
    }
  }
  cout << endl << endl;
}

/*
 *  A "smarter" way to access the elements through indices. Note that the same
 *  code will work independently of the dimensionality (although it has to be
 *  known at compilation time from the very nature of MultiArrays).
 */
template< typename MultiArray >
void multi_array_without_index_list_using_indices_anyD(const MultiArray& m)
{
  cout << "Without index_list using indices (any-D version)." << endl;
  typedef typename MultiArray::index index_type;
  typedef typename MultiArray::size_type size_type;
  typedef typename MultiArray::element element_type;
  typedef boost::array<typename MultiArray::index, MultiArray::dimensionality>
    indices_type;

  //  Initialize the indices to the first element.
  indices_type indices;
  std::copy(m.index_bases(),m.index_bases()+m.num_dimensions(),indices.begin());

  //  Loop (if there are any elements to loop over).
  if (m.num_elements() > 0)
  {
    bool not_done_yet(true);
    while (not_done_yet)
    {
      const element_type& e( m(indices) );
      //  We could now use "e" to do whatever we want to do with the elements
      //  of the MultiArray. Of course, indices are also available.

      //  Display the elements together with the corresponding indices.
      display_indices(indices);
      cout << ":" << e << " ";

      //  The loop incrementing the indices.
      for (size_type d(m.num_dimensions()-1); true ; --d)
      {
        //  Increment the index at the current dimension.
        ++indices[d];
        //  If within range, this is the desired value.
        if (indices[d]<static_cast<index_type>(m.index_bases()[d]+m.shape()[d]))
          break;
        else
        {
          if (d > 0)
          {
            //  The index overshot. Reset it to its base value (and continue
            //  the loop with the next dimension).
            indices[d] = m.index_bases()[d];
          }
          else
          {
            //  The last dimension overshot. We are done.
            not_done_yet = false;
            break;
          }
        }
      }
    }
  }
  cout << endl << endl;
}

// ***************************************************************************
//  Boost.MultiArray examples using Boost.IndexList.

/*
 *  A simple code based on a box_domain. Notice the similarity with the code of
 *  multi_array_without_index_list_using_data_and_num_elements_anyD.
 */
template< typename MultiArray >
void multi_array_with_index_list_using_domains(const MultiArray& m)
{
  cout << "Use a Domain to browse a MultiArray." << endl;
  typedef typename boost::index_list::multi_array_types<MultiArray>::box utils;
  typedef typename MultiArray::index index_type;
  typedef typename MultiArray::element element_type;

  //  Get a box_domain adapted to our MultiArray.
  typename utils::domain_type box(m);

  // Now a single for-loop.
  for (index_type i(0); i < box.num_elements(); ++i)
  {
    const element_type& e1( m.data()[i] );
    const element_type& e2( m( box.expand(i) ) );
    //  Either of e1 or e2 could be used to do whatever we want to do with the
    //  elements of the MultiArray. The indices are available.

    //  Display the elements together with the corresponding indices.
    display_indices( box.expand(i) );
    cout << ":" << e1 << e2 << " ";
  }
  cout << endl << endl;
}

/*
 *  A similar code based on an iterator.
 */
template< typename MultiArray >
void multi_array_with_index_list_using_iterator(const MultiArray& m)
{
  cout << "Use an index_list iterator to browse a MultiArray." << endl;
  typedef typename boost::index_list::multi_array_types<MultiArray>::box utils;
  typedef typename MultiArray::element element_type;

  // The loop.
  for (typename utils::const_iterator it(utils::begin(m));
       it != utils::end(m); ++it)
  {
    const element_type& e( *it );
    //  We could now use "e" to do whatever we want to do with the elements of
    //  the MultiArray. The list itself is a collection of the indices.

    //  Display the elements together with the corresponding indices.
    display_indices(it.indices());
    cout << ":" << e << " ";
  }
  cout << endl << endl;
}

/*
 *  Using a special kind of IndexList that knows the domain it belongs to.
 */
template< typename MultiArray >
void multi_array_with_index_list_using_domain_index_list(const MultiArray& m)
{
  cout << "Use an IndexList to browse a MultiArray." << endl;
  typedef typename boost::index_list::multi_array_types<MultiArray>::box utils;
  typedef typename MultiArray::element element_type;

  //  Get a domain_index_list adapted to our MultiArray.
  typename utils::domain_index_list_type list(m);

  // The loop.
  for (; list.is_valid(); ++list)
  {
    const element_type& e( m(list) );
    //  We could now use "e" to do whatever we want to do with the elements of
    //  the MultiArray. The list itself is a collection of the indices.

    //  Display the elements together with the corresponding indices.
    display_indices(list);
    cout << ":" << e << " ";
  }
  cout << endl << endl;
}

// ***************************************************************************
//  Further (not as useful) examples for Boost.MultiArray using Boost.IndexList.

/*
 *  Initialize a MultiArray such that each of its elements contains a collection
 *  of indices corresponding to the position of the corresponding element.
 */
template< typename MultiArray >
void initialize_box_of_indices(MultiArray& m)
{
  typedef typename boost::index_list::multi_array_types<MultiArray>::box utils;
  for (typename utils::iterator it(m); it != utils::end(m); ++it)
  {
    typename utils::domain_type::index_list_type indices = it.indices();
    std::copy(indices.begin(),indices.end(),(*it).begin());
  }
}

/*
 *  Show the content of a MultiArray containing collections of indices.
 */
template< typename MultiArray >
void show_box_of_indices(const MultiArray& m)
{
  cout << "Indices of each element and the element itself (same)." << endl;
  typedef typename boost::index_list::multi_array_types<MultiArray>::box utils;
  for(typename utils::const_iterator it(m); it != utils::end(m); ++it)
  {
    display_indices(it.indices()); //  The indices of the current element.
    cout << ":";
    display_indices(*it); //  The current element.
    cout << " ";
  }
  cout << endl << endl;
}

/*
 *  As previous, except using a simplex_domain instead. Not showing the indices
 *  independently since they are the same as the elements.
 */
template< typename MultiArray >
void show_box_of_indices_going_away_from_origin(const MultiArray& m)
{
  cout << "Going outward from the origin, stoping at the largest simplex." << endl;
  typedef typename boost::index_list::multi_array_types<MultiArray>::simplex utils;
  for(typename utils::const_iterator it(m); it != utils::end(m); ++it)
  {
    display_indices(*it);
    cout << " ";
  }
  cout << endl << endl;
}

/*
 *  As previous, except restricting the domain to a single plane.
 */
template< typename MultiArray >
void show_box_of_indices_plane(const MultiArray& m, const int plane_index)
{
  cout << "A single plane." << endl;
  typedef typename boost::index_list::multi_array_types<MultiArray>::simplex utils;
  typename utils::domain_type plane(m,plane_index,plane_index);
  for(typename utils::const_iterator it(m,plane); it != utils::end(m,plane); ++it)
  {
    display_indices(*it);
    cout << " ";
  }
  cout << endl << endl;
}

// ***************************************************************************
//  Beyond Boost.MultiArray (i.e. what could be bundled into indexable_space).

/*
 *  Boost.MultiArray has the following two limitations.
 *    - Its dimensionality must be specified statically.
 *    - It is limited to "box" geometry.
 *
 *  We here overcome the first limitation, the static dimensionality, by using
 *  a box_domain that returns IndexLists whose size is dynamically decided (i.e.
 *  using boost::collection_maker::vector). The storage is provided by a vector
 *  of size prescribed by box_domain::num_elements(). The box_domain::reduce()
 *  and box_domain::expand() functions can then be used to access the elements
 *  of the vector through a MultiArray-like interface, except that the
 *  dimensionality can be decided dynamically.
 *
 *  If interest is shown, these functionalities could be bundled into the
 *  (currently non-existing) indexable_space, indexable_space_ref and
 *  const_indexable_space_ref classes.
 */
template< typename MultiArray >
void store_any_multi_array_in_vector(const MultiArray& m)
{
  cout << "MultiArray-like storage of dynamical dimensionality." << endl;
  typedef boost::index_list::box_domain< boost::collection_maker::vector >
    box_type;
  typedef box_type::index_type index_type;
  typedef typename MultiArray::element element_type;
  box_type box(m);
  std::vector< element_type > storage(box.num_elements());
  // Store the content of the MultiArray into the vector.
  for (index_type i(0); i < box.num_elements(); ++i)
    storage[i] = m( box.expand(i) );
  // Now access the content and display it.
  for (index_type i(0); i < box.num_elements(); ++i)
  {
    box_type::index_list_type indices( box.expand(i) );
    display_indices(indices);
    cout << ":" << storage[i] << " ";
  }
  cout << endl << endl;
}

/*
 *  The second limitation of Boost.MultiArray that was mentionned earlier was
 *  that it is limited to "box" geometry. Some readers may think that "you can
 *  always make a box bigger than your actual domain of interest and simply
 *  disregard the unused elements". However, proceeding this way may require 
 *  much more memory than is actually required.
 *
 *  Luckily, the vector-domain combination that was used earlier can again solve
 *  this problem by simply using a different domain. Note again that this could
 *  be considered by an eventual indexable_space family of classes.

 *  Before providing an actual example, we will first compare the number of
 *  elements contained in a simplex_domain and in the smallest box_domain that
 *  contains all of its elements. While the factor ~50% observed in two
 *  dimensions may not worth the effort, things are clearly different in
 *  higher-dimensional spaces.
 */
void compare_num_elements_in_simplex_and_box(const unsigned int dim,
                                             const unsigned int edge)
{
  cout << "Comparison of num_elements in different domains." << endl;
  typedef boost::index_list::box_domain< boost::collection_maker::vector >
    box_type;
  typedef boost::index_list::simplex_domain< boost::collection_maker::vector >
    simplex_type;
  typedef box_type::index_type index_type;
  typedef box_type::width_type width_type;
  box_type box(dim, std::vector< width_type >(dim,edge).begin());
  simplex_type simplex(dim, edge);
  cout << "There are " << simplex.num_elements() << " elements in a " << dim
    << " dimensional simplex of edge " << edge << "." << endl;
  cout << "The smallest box that contains all these elements has "
    << box.num_elements() << " elements." << endl;
  cout << "Using a simplex thus requires "
    << static_cast<double>(100 * simplex.num_elements()) /
    static_cast<double>(box.num_elements())
    << "% of the space required by the box." << endl << endl;
}

/*
 *  We here build then display Pascal's simplex of a given dimension. Although
 *  analytical results directly provide the entries in this simplex (multinomial
 *  coefficients), we here instead use the simple recurrence relationship
 *
 *  P_n(x_1,x_2,...,x_n) = P_n(x_1-1,x_2,...,x_n) + P_n(x_1,x_2-1,...,x_n) + ...
 *
 *  where P_n is the n-dimensional Pascal's simplex whose elements are specified
 *  through the indices (x_1,...,x_n). We define P(0,0,...,0) = 1 as a starting
 *  point and terms with at least one negative index have a zero contribution.
 *
 *  This last requirement, which corresponds to a boundary condition, will be
 *  considered by using is_valid(). This approach, i.e. using is_valid() to
 *  handle boundary conditions, has applications notably for such recurrence
 *  relationships, for Master equations etc.
 */
void pascal_simplex(const unsigned int dim, const unsigned int edge)
{
  cout << "Produce Pascal's simplex using a recurrence relation." << endl;
  cout << "Using " << dim << " dimensions and producing " << edge << " layers."
    << endl;
  typedef boost::index_list::simplex_domain< boost::collection_maker::vector >
    simplex_type;
  typedef simplex_type::index_type index_type;
  typedef simplex_type::width_type width_type;
  typedef simplex_type::dimension_type dimension_type;
  typedef simplex_type::index_list_type index_list_type;
  typedef unsigned int element_type;
  simplex_type simplex(dim, edge);
  std::vector< element_type > storage(simplex.num_elements(),0);
  //  Initial condition P(0,0,...,0) = 1.
  storage[simplex.reduce(index_list_type(dim,0))]=1; // Or simply storage[0]=1;
  //  Loop over each element outward from the origin.
  for (index_type i(1); i < simplex.num_elements(); ++i)
  {
    index_list_type indices(simplex.expand(i));
    //  Loop over each dimension.
    for (dimension_type d(0); d < dim; ++d)
    {
      //  The element just before the current one, along dimension d.
      index_list_type before(indices);
      --before[d];
      //  Add its contribution if it is valid (i.e. boundary condition).
      if (simplex.is_valid(before))
      {
        storage[simplex.reduce(indices)] // Or simply storage[i]
          += storage[simplex.reduce(before)]; // Here, reduce() is required!
      }
    }
  }
  //  Pascal's simplex is completed!
  //  Now show it, one layer at a time.
  for (width_type l(0); l < edge; ++l)
  {
    cout << "* Layer " << l << " *" << endl;
    //  A domain consisting exclusively of the current layer.
    simplex_type layer(dim,edge,l,l);
    for (index_type i(0); i < layer.num_elements(); ++i)
    {
      index_list_type indices(layer.expand(i));
      display_indices(indices);
      //  Notice the conversion between the "layer" and "simplex" domains.
      cout << ":" << storage[simplex.reduce(indices)] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

// ***************************************************************************

int main()
{
  //  Some MultiArrays that will be used in the following examples.
  boost::multi_array< char, 2 > a_to_x_2D(boost::extents[4][6]);
  boost::multi_array< char, 3 > a_to_x_3D(boost::extents[2][4][3]);
  const char a_to_x[] = "abcdefghijklmnopqrstuvwx";
  a_to_x_2D.assign(a_to_x, a_to_x + a_to_x_2D.num_elements());
  a_to_x_3D.assign(a_to_x, a_to_x + a_to_x_3D.num_elements());

  /*
   *  Some Boost.MultiArray examples showing how to proceed without
   *  Boost.IndexList. The resulting "ugliness" and "impracticality" justifies
   *  the need for Boost.IndexList (doesn't it?).
   */
  cout << "***** Examples for Boost.MultiArray without Boost.IndexList. *****";
  cout << endl;
  multi_array_without_index_list_using_data_and_num_elements_anyD(a_to_x_2D);
  multi_array_without_index_list_using_data_and_num_elements_anyD(a_to_x_3D);
  //  multi_array_without_index_list_using_iterators_3D(a_to_x_2D); //  Error!
  multi_array_without_index_list_using_iterators_3D(a_to_x_3D);
  //  multi_array_without_index_list_using_indices_3D(a_to_x_2D); //  Error!
  multi_array_without_index_list_using_indices_3D(a_to_x_3D);
  multi_array_without_index_list_using_indices_anyD(a_to_x_2D);
  multi_array_without_index_list_using_indices_anyD(a_to_x_3D);
  cout << endl;

  /*
   *  Examples showing how Boost.IndexList can be used as a complement to
   *  Boost.MultiArray. Note that the dimensionality is never a problem.
   */
  cout << "***** Examples for Boost.MultiArray using Boost.IndexList. *****";
  cout << endl;
  multi_array_with_index_list_using_domains(a_to_x_2D);
  multi_array_with_index_list_using_domains(a_to_x_3D);
  multi_array_with_index_list_using_iterator(a_to_x_2D);
  multi_array_with_index_list_using_iterator(a_to_x_3D);
  multi_array_with_index_list_using_domain_index_list(a_to_x_2D);
  multi_array_with_index_list_using_domain_index_list(a_to_x_3D);
  cout << endl;

  /*
   *  Further Boost.IndexList examples showing different ways to browse a
   *  Boost.MultiArray.
   *
   *  These examples are not as useful as the pervious ones and are thus
   *  commented out. Please skip them and step to the next bundle of examples
   *  right away. You may come back later if you really want to...
   */
//  boost::multi_array< boost::array< int, 2 >, 2 >
//    square_of_indices(boost::extents[5][5]);
//  boost::multi_array< boost::array< int, 3 >, 3 >
//    cube_of_indices(boost::extents[4][4][4]);
//  initialize_box_of_indices(square_of_indices);
//  initialize_box_of_indices(cube_of_indices);
//  cout << "***** Further examples concerning Boost.MultiArray. *****";
//  cout << endl;
//  show_box_of_indices(square_of_indices);
//  show_box_of_indices(cube_of_indices);
//  show_box_of_indices_going_away_from_origin(square_of_indices);
//  show_box_of_indices_going_away_from_origin(cube_of_indices);
//  show_box_of_indices_plane(square_of_indices,3);
//  show_box_of_indices_plane(cube_of_indices,3);

  /*
   *  We are now going beyond Boost.MultiArray and explore how Boost.IndexList
   *  provide storage of dynamically-chosen dimensionality that is not limited
   *  to a box-like domain.
   */
  cout << "***** Boost.IndexList as an alternative to Boost.MultiArray. *****";
  cout << endl;
  store_any_multi_array_in_vector(a_to_x_2D);
  store_any_multi_array_in_vector(a_to_x_3D);
  compare_num_elements_in_simplex_and_box(2,5);
  compare_num_elements_in_simplex_and_box(2,30);
  compare_num_elements_in_simplex_and_box(3,5);
  compare_num_elements_in_simplex_and_box(3,30);
  compare_num_elements_in_simplex_and_box(5,5);
  compare_num_elements_in_simplex_and_box(5,30);
  pascal_simplex(2,10);
  pascal_simplex(3,7);

  return 0;
}
