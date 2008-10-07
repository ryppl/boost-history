
#ifndef BOOST_GRAPHS_ADJMATRIX_BASIC_MATRIX_HPP
#define BOOST_GRAPHS_ADJMATRIX_BASIC_MATRIX_HPP

#include <vector>

#include <boost/optional.hpp>
#include <boost/optional_value.hpp>
#include <boost/none.hpp>
#include <boost/triple.hpp>

#include <boost/graphs/adjacency_matrix/matrix_element.hpp>
#include <boost/graphs/adjacency_matrix/column_traits.hpp>

namespace boost { namespace graphs { namespace adjacency_matrix {

namespace detail
{
    // A little helper function for the template constructor.
    // TODO Write this for tuples also.
    template <typename Matrix, typename T1, typename T2>
    void expand_and_set(Matrix& m, std::pair<T1, T2> const& p)
    { m.set(p.first, p.second); }

    template <typename Matrix, typename T1, typename T2, typename T3>
    void expand_and_set(Matrix& m, triple<T1, T2, T3> const& t)
    { m.set(t.first, t.second, t.third); }
}

/**
 * An unoptimized implementation of the basic square matrix that contains
 * optional elements T.
 *
 * This is not a basic matrix in the linear algebra sense of things. This is a
 * legitimate container, albeit arranged in a fairly rigid fashion. This class
 * cannot be used interchangeably with Matrix interface found in Boost.uBLAS.
 * The reason for this is that we can return optional values for all non-boolean
 * value types.
 *
 * There are three important variations of this structure that depend on the
 * type T. If T is given as none, then the value type stored by this class is
 * boolean. If T is an optional_value<U,A> then this stores exactly objects of
 * exactly that type, buit the value_type is reinterpreted to be U. Otherwise,
 * this will store optional<T> and the value_type is T.
 *
 * This is currently implemented using std::vectors, but could potentially be
 * rewritten to use a simple dynamic array.
 *
 * @todo Invert the meanings of element_type and value_type.
 */
template <typename T, template <typename> class Allocator = std::allocator>
struct basic_matrix
{
    typedef typename detail::matrix_element<T>::value_type value_type;
    typedef typename detail::matrix_element<T>::element_type element_type;
    typedef std::vector<element_type> columns_type;     // contains edges
    typedef std::vector<columns_type> rows_type;        // contains rows

    typedef detail::column_traits<columns_type> ct;
    typedef typename ct::reference reference;
    typedef typename ct::const_reference const_reference;

    typedef typename rows_type::size_type size_type;

    /** @name Constructors/Destructor */
    //@{
    basic_matrix(size_type n)
        : _rows(n, columns_type(n))
    { }

    basic_matrix(size_type n, value_type const& x)
        : _rows(n, columns_type(n, ct::make_value(x)))
    { }

    basic_matrix(basic_matrix const& x)
        : _rows(x.rows)
    { }

    basic_matrix(basic_matrix&& x)
        : _rows(std::move(x._rows))
    { }

    template <typename Iter>
    basic_matrix(size_type n, Iter f, Iter l)
        : basic_matrix(n)
    { for( ; f != l; ++f) detail::expand_and_set(*this, *f); }

    ~basic_matrix()
    { _rows.clear(); }
    //@}

    /** @name Row Accessors */
    //@{
    reference operator()(size_type i, size_type j)
    { return _rows[i][j]; }

    const_reference const& operator()(size_type i, size_type j) const
    { return _rows[i][j]; }

    /** Test to see if the element at i, j is set. */
    bool test(size_type i, size_type j)
    {
        // Relies on the optional<T>::operator bool() overload.
        return _rows[i][j];
    }

    /**
     * Get the value held at the index i, j. Note that the gotten result is not
     * either const or an rvalue.
     */
    typename ct::get_result get(size_type i, size_type j) const
    { return _rows[i][j]; }

    /**
     * Make this matrix element set to true. This operation is only used if
     * T is none. Otherwise, the 3-parameter version of set is used. Using this
     * for non-none types T will probably result in a compiler errror.
     */
    void set(size_type i, size_type j)
    { ct::set(_rows[i], j); }

    /**
     * Set the value of this element to the given value.
     */
    void set(size_type i, size_type j, element_type const& x)
    { ct::set(_rows[i], j, x); }

    /**
     * Clear the value of this element. If T is none, this simply resets the
     * value to false. Otherwise, it actually clears the optional value.
     */
    void clear(size_type i, size_type j)
    { ct::clear(_rows[i], j); }
    //@}

    /**
     * Clear the contents of the matrix. This does not change the dimensions of
     * the data structure, nore reallocate memory.
     */
    void clear()
    {
        for(int i = 0; i < _rows.size(); ++i) {
            for(int j = 0; j < _rows.size(); ++j) {
                clear(i, j);
            }
        }
    }

    /** Return the square dimension of the matrix. */
    size_type size() const
    { return _rows.size(); }

private:
    rows_type _rows;
};

} } } /* namespace boost::graphs::adjacency_matrix */

#endif
