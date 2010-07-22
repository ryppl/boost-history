// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#ifndef BOOST_RANDOM_D_STAR_HEADER_INCLUDED
#define BOOST_RANDOM_D_STAR_HEADER_INCLUDED

#include <cmath> // std::pow, std::abs
#include <vector>
#include <stdexcept>
#include <boost/throw_exception.hpp>
#include <boost/noncopyable.hpp>
#include <boost/pool/pool.hpp>
#include <boost/shared_array.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/utility/result_of.hpp>

// The time required to compute the star discrepancy of a sequence of points
// in a multidimensional unit cube is prohibitive and the best known upper
// bounds for the star discrepancy of (t,s)-sequences and (t,m,s)-nets are
// useful only for sample sizes that grow exponentially with the dimension s.
//
// Here we implement an algorithm to compute upper bounds for the star discrepancy of an
// arbitrary set of n points in the s-dimensional unit cube as proposed in
// Eric Thiemard's paper: "Computing bounds for the star discrepancy", Computing  65 (2000) 2, 169-186.
// For an integer k > 1, this algorithm computes in O(ns log k+2^sk^s) time
// and O(k^s) space a bound that is no better than a function depending on s and k.

namespace detail {

// A basic tree node -- POD
struct tree_node
{
  int id;
  tree_node* node;
};

struct tree_handle
{
  template<typename Pool>
  explicit tree_handle(Pool& p, int sz)
    : lower_bound(1)
  {
    // allocate 1 bookkeeping node + sz data nodes
    children = (tree_node*) p.ordered_malloc(1 + sz);
    if( children == 0 )
      boost::throw_exception( std::bad_alloc() );

    // zero memory
    tree_node zero = { 0, 0 };
    std::fill(children, children + 1 + sz, zero);

    children[0].id = sz;
  }

  int& operator[](int n) { return data()[n].id; }
  int operator[](int n) const { return data()[n].id; }

  int low() const { return lower_bound - 1; }
  int high() const { return children_count() - 1; }
  int children_count() const { return children[0].id; }
  void low_bound(int sz) { lower_bound = sz; }

  tree_handle subtree(int n) const
  {
    return tree_handle(data()[n].node);
  }

  template<typename Pool, typename Range, typename Comparator>
  tree_handle get_subtree(Pool& p, Range range, Comparator comp)
  {
    if( !has_subtree(range.second) )
    {
      int n_elem = range.second - range.first + 1;

      tree_handle tree(p, n_elem);
      for(int i = 0; i < n_elem; ++i)
        tree[i] = (*this)[range.first + i]; // copy relevant data

      if( 1 < n_elem )
        sort_tree(tree, comp);

      attach_subtree(range.second, tree);

      return tree;
    }
    else
    {
      return subtree(range.second);
    }
  }

private:
   explicit tree_handle(tree_node* ptr)
    : lower_bound(1)
    , children(ptr)
  {}

  tree_node* data() const {
    return children + 1;  // !!!
  }

  bool has_subtree(int n) const
  {
    return data()[n].node != 0;
  }

  void attach_subtree(int n, const tree_handle& sub)
  {
    data()[n].node = sub.children;
  }

  // Never throws
  template<typename Comparator>
  inline friend void sort_tree(tree_handle& tree, Comparator comp) {
    std::sort(tree.data(), tree.data() + tree.children_count(), comp);
  }

private:
  int lower_bound;
  tree_node* children;
};


// An s-dimensional table of values that are stored consecutively in the
// memory area pointed by the given pointer.
// The table is strictly read-only.
template<typename T>
struct value_table
{
  value_table(const T* values, std::size_t width)
    : tbl(values), dim(width) {}
  std::size_t width() const { return dim; }
  T operator()(std::size_t i, std::size_t j) const {
    return tbl[i * dim + j];
  }
private:
  const T* tbl;
  std::size_t dim;
};


// Sorting predicate
template<typename T>
struct branch_comparator_func
{
  explicit branch_comparator_func(value_table<T> pt, std::size_t ofs)
    : table(pt), n_elem(ofs) {}
  bool operator()(const tree_node& lhs, const tree_node& rhs) const {
    return table(lhs.id, n_elem) < table(rhs.id, n_elem);
  }
private:
  value_table<T> table;
  std::size_t n_elem;
};

template<typename T>
inline branch_comparator_func<T> branch_compare(value_table<T> pt, std::size_t ofs = 0) {
  return branch_comparator_func<T>(pt, ofs);
}


// Functors that more or less do forwarding back to the LexTree.
// Mostly used to customize the behaviour of binary search.
// (Simpler than member function binding.)
struct do_nothing
{
  template<typename Range>
  void operator()(Range) const
  { /* do nothing at all */ }
};

struct cutoff_tree
{
  tree_handle* nd;
  cutoff_tree(tree_handle& n) : nd(&n) {}
  template<typename Range>
  void operator()(Range range)
  {
    nd->low_bound(range.first + 1);
  }
};

struct count_ranges
{
  template<typename LexTree, typename Range, typename BoundVector>
  int operator()(LexTree&, tree_handle&, Range range, BoundVector) const
  { return range.second - range.first + 1; }
};

struct explode_tree
{
  template<typename LexTree, typename Range, typename BoundVector>
  std::size_t operator()(LexTree& lex, tree_handle& nd, Range range, BoundVector rv) const
  {
    tree_handle b = subtree(lex, nd, range, rv);
    lex.insert(b, rv.range());
    return lex.explore(b, rv);
  }
};

struct explore_implicit_tree
{
  template<typename LexTree, typename Range, typename BoundVector>
  std::size_t operator()(LexTree& lex, tree_handle& nd, Range range, BoundVector rv) const
  {
    tree_handle b = subtree(lex, nd, range, rv);
    return lex.explore(b, rv);
  }
};

template<typename T>
class range_tree : private boost::noncopyable
{
private:
  typedef std::vector<T> vector_t;

  struct vector_range_pair
  {
    vector_range_pair deepen() const
    { return vector_range_pair(p_ptr, m_range + 1); }
    std::size_t range() const { return m_range; }
    T value() const { return (*p_ptr)[m_range]; }
    std::size_t next_range() { return ++m_range; }
    vector_range_pair(const vector_t& p,const std::size_t ofs)
       : m_range(ofs), p_ptr(&p)
     {}
   private:
    vector_range_pair(const vector_t* p, const std::size_t ofs)
      : m_range(ofs), p_ptr(p)
    {}
    std::size_t m_range;
    const vector_t* p_ptr;
  };

public:
  range_tree(value_table<T> values, const tree_handle& root, boost::pool<>& p)
    : table(values)
    , subtotal(0)
    , lex(values.width())
    , pool_(p)
  {
    insert(root, 0);
  }

  std::size_t explore(const vector_t& p, const std::size_t ofs)
  {
    BOOST_ASSERT( table.width() == p.size() );

    vector_range_pair rv(p, ofs);

    int total = 0;
    std::size_t sz = lex[ofs].size();
    if( ofs + 1 == table.width() )
    {
      while( sz-- != 0 )
      {
        tree_handle tree = lex[ofs][sz];
        int low  = tree.low();
        int high = tree.high();
        if( high < low )
        {
          lex[ofs][sz] = lex[ofs].back();
          lex[ofs].pop_back();
          subtotal += low;
        }
        else
        {
          total += low;
          total += bin_explore(tree, std::make_pair(low, high), rv, count_ranges(),
              cutoff_tree(lex[ofs][sz]));
        }
      }
      total += subtotal;
    }
    else
    {
      subtotal = 0;
      lex[ofs + 1].clear();
      for(std::size_t i = 0; i != sz; ++i)
      {
        tree_handle tree = lex[ofs][i];
        int start = tree.low();
        int low   = 0;
        int high  = tree.high();
        while( low != start )
        {
          int next = ((1 + low) + high) / 2;
          tree_handle branch = tree.subtree(next);
          insert(branch, ofs + 1);
          total += explore(branch, rv.deepen());
          low = next + 1;
        }
        total += bin_explore(tree, std::make_pair(low, high), rv, explode_tree(),
            cutoff_tree(lex[ofs][i]));
      }
    }
    return total;
  }

private:
  friend struct explode_tree;
  friend struct explore_implicit_tree;

  template<typename Range, typename BoundVector>
  inline friend tree_handle subtree(range_tree<T>& lex, tree_handle& nd, Range range, BoundVector rv) {
    // adds a new (or gets the existing) subtree, sorted at the specified level
    return nd.get_subtree(lex.pool_, range, branch_compare(lex.table, rv.range()));
  }

  void insert(const tree_handle& nd, std::size_t ofs)
  {
    lex[ofs].push_back(nd);
  }

  template<typename BoundVector>
  bool in_range(const tree_handle& nd, std::size_t i, BoundVector rv) const
  {
    return table(nd[i], rv.range()) < rv.value();
  }

  template<typename Range, typename BoundVector, typename Explore, typename Right>
  std::size_t bin_explore(tree_handle& nd, Range range,
      BoundVector rv, Explore explore, Right on_right)
  {
    bool right = true;
    std::size_t total = 0;
    while( range.first <= range.second )
    {
      int next = ((1 + range.first) + range.second) / 2;
      if( in_range(nd, next, rv) )
      {
        total += explore(*this, nd, std::make_pair(range.first, next), rv.deepen());
        range.first = next + 1;
        if( right ) {
          on_right(range);
        }
      }
      else
      {
        right = false;
        range.second = next - 1;
      }
    }
    return total;
  }

  template<typename BoundVector, typename Explore>
  std::size_t bin_explore(tree_handle& nd, BoundVector rv, Explore explore)
  {
    return bin_explore(nd, std::make_pair(0, nd.high()), rv, explore, do_nothing());
  }

  template<typename BoundVector>
  std::size_t explore(tree_handle& nd, BoundVector rv)
  {
    // how many children do we have?
    const int sz = nd.children_count();

    // quick case
    if( !in_range(nd, 0, rv) )
      return 0;

    // a single element case
    if( sz == 1 )
    {
      while( rv.next_range() != table.width() )
        if( !in_range(nd, 0, rv) )
          return 0;
      return 1;
    }

    // the last column to be explored
    if( rv.range() + 1 == table.width() )
    {
      // another quick case
      return in_range(nd, sz - 1, rv) ? sz:
        bin_explore(nd, rv, count_ranges());
    }

    return bin_explore(nd, rv, explore_implicit_tree());
  }

private:
  typedef std::vector<tree_handle> tree_t;
  typedef std::vector<tree_t> forest_t;

  value_table<T> table; // table of values
  int subtotal; // cached
  forest_t lex;

  boost::pool<>& pool_;
};


// D-star state, uses range trees for counting
template<typename T>
class star_discrepancy
{
private:
  struct range_tree_pair
  {
    range_tree<T> alpha, beta;
    template<typename Pool>
    range_tree_pair(value_table<T> values, const tree_handle& root, Pool& p)
      : alpha(values, root, p)
      , beta(values, root, p)
    {}
  };

public:
  typedef std::vector<T>    vector_t; // type shortcuts
  typedef std::pair<T, T>   bounds_t;

  // Initialize point table
  template<typename Engine, typename Distribution>
  star_discrepancy(Engine& eng, Distribution d, std::size_t dimension, std::size_t n, T eps)
    : dim(dimension) // for non-QRNGs q.dimension() is malformed
    , n_elem(n)
    , epsilon(eps)
    , points(new T[dim * n_elem])
  {
    // All points must be bounded by the unit hypercube.
    for(std::size_t i = 0; i != dim * n_elem; ++i)
    {
      T value = d(eng);
      if( value < T(0) || value > T(1) )
        boost::throw_exception( std::invalid_argument("d_star") );
      points[i] = value;
    }
  }

  // Initialize point table
  template<std::size_t Dimension, std::size_t N>
  star_discrepancy(T (&pt)[N][Dimension], T eps)
    : dim(Dimension)
    , n_elem(N)
    , epsilon(eps)
    , points(new T[dim * n_elem])
  {
    // All points must be bounded by the unit hypercube.
    for(std::size_t i = 0; i != n_elem; ++i)
    {
      for(std::size_t j = 0; j != dim; ++j)
      {
        T value = pt[i][j];
        if( value < T(0) || value > T(1) )
          boost::throw_exception( std::invalid_argument("d_star") );
        points[i * dim + j] = value;
      }
    }
  }

  // Computes lower and upper discrepancy bounds
  bounds_t compute() const
  {
    bounds_t bounds = std::make_pair(T(), T());

    boost::pool<> p(sizeof(tree_node));
    tree_handle root = create_root(p);
    range_tree_pair tree(get_table(), root, p);

    // Now we carry out sequence decomposition into subintervals
    vector_t alpha(dim, T(0)), beta(dim, T(1));
    decompose(alpha, beta, tree, 0, T(1), bounds);

    return bounds;
  }

private:

  T volume_diff(T volume, std::size_t n) const
  {
    return volume - static_cast<T>(n) / static_cast<T>(n_elem);
  }

  T abs_volume(T volume, std::size_t n) const
  {
    return std::abs( volume_diff(volume, n) );
  }

  static void upper_bound(T volume, bounds_t& bounds)
  {
    if( bounds.second < volume )
    {
      bounds.second = volume;
    }
  }

  T compute_delta(const vector_t& alpha, const vector_t& beta, std::size_t start) const
  {
    T pbetamin = T(1);
    for(std::size_t i = start; i != dim; ++i)
      pbetamin *= beta[i];
    T pbeta = pbetamin;

    T palpha = T(1);
    for(std::size_t i = 0; i != start; ++i) {
      pbetamin *= beta[i];
      palpha   *= alpha[i];
    }

    pbetamin -= epsilon;
    return std::pow( pbetamin / ( pbeta * palpha ), T(1) / T(dim - start) );
  }

  template<typename Predicate>
  T volume_bound(const vector_t& p, Predicate pred, const T init) const
  {
    BOOST_ASSERT( p.size() == dim );

    T volume = T(1);
    for(std::size_t j = 0; j != dim; ++j)
    {
      T u = init;
      for(std::size_t i = 0; i != n_elem; ++i)
      {
        T pt = points[i*dim + j];
        if( pred(u, pt) && !pred(p[j], pt) )
          u = pt;
      }
      volume *= u;
    }
    return volume;
  }

  void lower_bound(T volume, std::size_t npoints, const vector_t& p, bounds_t& b) const
  {
    if( b.first < abs_volume(volume, npoints) )
    {
      if( volume_diff(volume, npoints) < T() )
        volume = volume_bound(p, std::less<T>(), T());
      else
        volume = volume_bound(p, std::greater<T>(), T(1));
      // recompute lower bound approximation
      b.first = abs_volume(volume, npoints);
    }
  }

  void evaluate(const vector_t& alpha, const vector_t& beta, range_tree_pair& tree,
                std::size_t start,
                bounds_t& b) const
  {
    T va = T(1); // volume alpha
    T vb  = T(1); // volume beta
    for(std::size_t i = 0; i != dim; ++i) {
      va *= alpha[i];
      vb *= beta[i];
    }

    // Here we evaluate uppper bound
    std::size_t nalpha = tree.alpha.explore(alpha, start);
    std::size_t nbeta = tree.beta.explore(beta, start);
    upper_bound(-volume_diff(va, nbeta), b);
    upper_bound(volume_diff(vb, nalpha), b);

    // ...and here we evaluate lower bound
    lower_bound(va, nalpha, alpha, b);
    lower_bound(vb, nbeta,  beta, b);
  }

  // In the original Eric Thiemard's paper
  // "Computing bounds for the star discrepancy", Computing  65 (2000) 2, 169-186.
  // the decomposition function requires O(k^s) space. Here we implement an optimized
  // version of the function that requires O(k * (k-1) * (k-2) * .. * (k-s-1)) space.
  void decompose(vector_t& alpha, vector_t& beta, range_tree_pair& tree,
                 std::size_t start, // starting element
                 T threshold, // decomposition threshold
                 bounds_t& b) const
  {
    T delta = compute_delta(alpha, beta, start);

    // Here we preserve only those alpha values that are going to be changed.
    // This is more efficient memory-wise than doing a full copy of alpha;
    // the deeper we go the less elements we will have to preserve.
    const vector_t oalpha(alpha.begin() + start, alpha.end());

    T obeta = beta[start];
    beta[start] *= delta;

    threshold *= delta;
    if( epsilon < threshold )
    {
      decompose(alpha, beta, tree, start, threshold, b);
      for(std::size_t i = start + 1; i < dim; ++i)
      {
        alpha[i-1] = obeta * delta;
        beta[i-1]  = obeta;
        obeta      = beta[i];
        beta[i]   *= delta;
        decompose(alpha, beta, tree, i, threshold, b);
      }
    }
    else
    {
      std::size_t k = (start == 0) ? 0: start - 1;
      evaluate(alpha, beta, tree, k++, b);
      for(std::size_t i = start + 1; i < dim; ++i)
      {
        // evaluate member function is not modifying,
        // so we can save one multiplication here
        alpha[i-1] = beta[i-1];
        beta[i-1]  = obeta;
        obeta      = beta[i];
        beta[i]   *= delta;
        evaluate(alpha, beta, tree, k++, b);
      }
    }

    alpha[dim-1] = obeta * delta;
    beta[dim-1] = obeta; // restore the last changed beta value
    evaluate(alpha, beta, tree, dim - 1, b);

    // Restore alpha values
    std::copy(oalpha.begin(), oalpha.end(), alpha.begin() + start);
  }

  template<typename Pool>
  tree_handle create_root(Pool& p) const
  {
    tree_handle tree(p, n_elem);

    for(std::size_t i = 0; i != n_elem; ++i)
      tree[i] = i;

    sort_tree(tree, branch_compare(get_table()));

    return tree;
  }

  // The lifetime of the returned table must not exceed the lifetime of this object
  value_table<T> get_table() const
  {
    return value_table<T>(points.get(), dim);
  }

private:
  const std::size_t dim;
  const std::size_t n_elem;
  const T epsilon;
  boost::shared_array<T> points;
};

} // namespace detail


// Accepts a table of values
template<typename T, std::size_t Dimension, std::size_t N>
inline typename detail::star_discrepancy<T>::bounds_t
d_star(T (&pt)[N][Dimension], T epsilon)
{
  detail::star_discrepancy<T> discr(pt, epsilon);
  return discr.compute();
}

// Accepts a pseudo-random number generator
template<typename Engine, typename Distribution>
inline typename detail::star_discrepancy<
  typename boost::result_of<Distribution(Engine)>::type
>::bounds_t
d_star(Engine& eng, Distribution d, std::size_t dimension, std::size_t n,
    typename boost::result_of<Distribution(Engine)>::type epsilon)
{
  typedef typename boost::result_of<Distribution(Engine)>::type value_t;
  detail::star_discrepancy<value_t> discr(eng, d, dimension, n, epsilon);
  return discr.compute();
}

// Accepts a quasi-random number generator and distribution
template<typename QEngine, typename Distribution>
inline typename detail::star_discrepancy<
  typename boost::result_of<Distribution(QEngine)>::type
>::bounds_t
d_star(QEngine& q, Distribution d, std::size_t n,
    typename boost::result_of<Distribution(QEngine)>::type epsilon)
{
  return d_star(q, d, q.dimension(), n, epsilon);
}

// Accepts a quasi-random number generator; distribution is uniform_real<T>
template<typename QEngine, typename T>
inline typename detail::star_discrepancy<T>::bounds_t
d_star(QEngine& q, std::size_t n, T epsilon)
{
  return d_star(q, boost::uniform_real<T>(), n, epsilon);
}

#endif // BOOST_RANDOM_D_STAR_HEADER_INCLUDED
