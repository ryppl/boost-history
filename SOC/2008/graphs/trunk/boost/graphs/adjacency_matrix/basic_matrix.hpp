
#ifndef BOOST_GRAPHS_ADJMATRIX_BASIC_MATRIX_HPP
#define BOOST_GRAPHS_ADJMATRIX_BASIC_MATRIX_HPP

#include <vector>

#include <boost/optional.hpp>
#include <boost/none.hpp>
#include <boost/triple.hpp>

namespace boost { namespace graphs { namespace adjacency_matrix {

namespace detail
{
    // Metafunctions to generate the value type of a basic matrix.
    // TODO: This facility probably needs to be exposed to other matrix
    // implementations (e.g., triangular matrix, compressed matrix, etc.).
    template <typename T>
    struct matrix_element { typedef optional<T> type; };
    template <> struct matrix_element<none> { typedef bool type; };

    // Specialize the case of matrix references and various operations on them.
    // Note that the generic case is empty.
    template <typename T> struct column_traits { };
    template <typename T>
    struct column_traits<std::vector<optional<T>>>
    {
        typedef std::vector<optional<T>> container;
        typedef typename container::size_type size_type;
        typedef typename container::reference reference;
        typedef typename container::const_reference const_reference;

        typedef T const& get_result;

        static optional<T> make_value(T const& x)
        { return optional<T>(x); }

        // This can easily segfault c[i] is not valid.
        get_result get(container const& c, size_type i)
        { return c[i].get(); }

        static void set(container& c, size_type i)
        { c[i] = optional<T>(); }

        static void set(container&c, size_type i, T const& x)
        { c[i] = x; }

        static void clear(container& c, size_type i)
        { c[i].reset(); }
    };
    template <>
    struct column_traits<std::vector<bool>>
    {
        typedef std::vector<bool> container;
        typedef container::size_type size_type;
        typedef container::reference reference;
        typedef container::const_reference const_reference;

        typedef const_reference get_result;

        static bool make_value(none)
        { return false; }

        static get_result get(container const& c, size_type i)
        { return c[i]; }

        static void set(container& c, size_type i)
        { c[i] = true; }

        static void clear(container& c, size_type i)
        { c[i] = false; }
    };


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
 * optional elements T. Note that the underlying storage type is specialized
 * for the type 'none', because this is essentially already a boolean data
 * structure, and we don't need to store any additional information. Otherwise,
 * T is made to be optional<T>.
 *
 * This is not a basic matrix in the linear algebra sense of things. This is a
 * legitimate container, albeit arranged in a fairly rigid fashion. This class
 * cannot be used interchangeably with Matrix interface found in Boost.uBLAS.
 * The reason for this is that we can return optional values for all non-boolean
 * value types.
 *
 * This is currently implemented using std::vectors, but could potentially be
 * rewritten to use a simple dynamic array.
 */
template <typename T, template <typename> class Allocator = std::allocator>
struct basic_matrix
{
    typedef typename detail::matrix_element<T>::type value_type;
    typedef std::vector<value_type> columns_type;    // contains edges
    typedef std::vector<columns_type> rows_type;     // contains rows

    typedef detail::column_traits<columns_type> ct;

    // The reference type of these vectors may not be the same as value_type&.
    typedef typename ct::reference reference;
    typedef typename ct::const_reference const_reference;

    typedef typename rows_type::size_type size_type;

    /** @name Constructors/Destructor */
    //@{
    basic_matrix(size_type n, T const& x = T())
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
    void set(size_type i, size_type j, T const& x)
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

} } }

#endif
